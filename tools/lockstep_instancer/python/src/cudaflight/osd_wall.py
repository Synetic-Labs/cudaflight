# SPDX-License-Identifier: GPL-3.0-or-later
"""GPU renderer for walls of Betaflight OSDs.

Font: the authentic Betaflight MAX7456 font, parsed straight out of the
firmware tree (src/platform/PICO/osd/font_betaflight.c — 256 glyphs,
12x18 px, 2 bits per pixel: black outline / white / transparent).

Rendering is pure torch on the same GPU the firmware fleet runs on: the
[K, 16, 30] uint8 character grids (zero-copy views of the instances'
screens) are gathered through the glyph atlas into one tiled mosaic
image. The pixels that prove the firmware is real never leave the card
until the final blit to the window.
"""

import re
from importlib.resources import as_file, files
from pathlib import Path

import numpy as np
import torch


def _packaged_font() -> Path:
    ref = files("cudaflight._data") / "font_betaflight.c"
    with as_file(ref) as p:
        return Path(p)


GLYPH_W, GLYPH_H = 12, 18
GLYPHS = 256

# pixel classes in the index image
_PX_TRANSPARENT, _PX_BLACK, _PX_WHITE, _PX_BORDER = 0, 1, 2, 3

_GLYPH_SPACE = 0x20


def load_font_atlas(path: "str | Path | None" = None) -> np.ndarray:
    """Parse the firmware's font table into a [256, 18, 12] uint8 array
    of pixel classes (0 transparent, 1 black, 2 white)."""
    font = Path(path) if path else _packaged_font()
    text = font.read_text()
    tokens = re.findall(r"0b([01]{8})", text)
    data = np.array([int(t, 2) for t in tokens], dtype=np.uint8)
    expected = GLYPHS * GLYPH_H * 3  # 3 bytes per 12px row, 2bpp
    if data.size < expected:
        raise ValueError(f"font table too small: {data.size} < {expected} bytes")
    data = data[:expected].reshape(GLYPHS, GLYPH_H, 3)

    # unpack 2-bit pixels (this table is LSB-first within each byte —
    # mcm2h.py reorders them). PICO fb convention, NOT raw MCM:
    # 00 background/skip, 10 white, 01/11 black outline
    shifts = np.array([0, 2, 4, 6], dtype=np.uint8)
    px = (data[..., :, None] >> shifts) & 0b11          # [256, 18, 3, 4]
    px = px.reshape(GLYPHS, GLYPH_H, GLYPH_W)
    atlas = np.full(px.shape, _PX_BLACK, dtype=np.uint8)
    atlas[px == 0b00] = _PX_TRANSPARENT
    atlas[px == 0b10] = _PX_WHITE
    return atlas


class OsdWall:
    """Tile K OSD character grids into one RGB mosaic on the GPU."""

    def __init__(self, device: torch.device, rows: int = 16, cols: int = 30,
                 gap: int = 2, bg: "tuple[int, int, int]" = (24, 28, 24),
                 border: "tuple[int, int, int]" = (70, 74, 70),
                 font_path: "str | Path | None" = None) -> None:
        self.device = device
        self.rows, self.cols = rows, cols
        self.gap = gap
        self.atlas = torch.as_tensor(load_font_atlas(font_path), device=device)
        self.lut = torch.tensor([list(bg),        # transparent -> background
                                 [0, 0, 0],       # glyph black outline
                                 [255, 255, 255], # glyph white
                                 list(border)],   # tile border
                                dtype=torch.uint8, device=device)
        self.tile_h = rows * GLYPH_H + 2 * gap
        self.tile_w = cols * GLYPH_W + 2 * gap

    def mosaic(self, osd: torch.Tensor, attrs: "torch.Tensor | None" = None,
               grid: "tuple[int, int] | None" = None,
               blink_phase: bool = False,
               background: "torch.Tensor | None" = None) -> torch.Tensor:
        """osd: [K, rows, cols] uint8 CUDA tensor of font indices.
        attrs: optional matching attribute grids (bit 7 = blink).
        grid: (tiles_y, tiles_x), default a near-square layout.
        background: optional [K, rows*18, cols*12, 3] uint8 per-tile
        image composited where the OSD is transparent (the camera feed
        a real MAX7456 would overlay).
        Returns an RGB uint8 [H, W, 3] CUDA tensor."""
        k = osd.shape[0]
        if grid is None:
            tx = int(np.ceil(np.sqrt(k)))
            grid = (int(np.ceil(k / tx)), tx)
        ty, tx = grid
        if ty * tx < k:
            raise ValueError(f"grid {grid} too small for {k} tiles")

        osd = osd.long()
        if attrs is not None and blink_phase:
            osd = torch.where(attrs & 0x80 != 0, _GLYPH_SPACE, osd)

        glyphs = self.atlas[osd]                              # [K, R, C, 18, 12]
        tiles = glyphs.permute(0, 1, 3, 2, 4).reshape(
            k, self.rows * GLYPH_H, self.cols * GLYPH_W)
        tiles = torch.nn.functional.pad(
            tiles, (self.gap,) * 4, value=_PX_BORDER)         # [K, th, tw]
        if ty * tx > k:                                       # ragged last row
            pad = tiles.new_full((ty * tx - k, *tiles.shape[1:]), _PX_BORDER)
            tiles = torch.cat([tiles, pad])
        idx = (tiles.view(ty, tx, self.tile_h, self.tile_w)
                    .permute(0, 2, 1, 3)
                    .reshape(ty * self.tile_h, tx * self.tile_w))
        out = self.lut[idx.long()]                            # [H, W, 3]
        if background is not None:
            bg = torch.nn.functional.pad(
                background.permute(0, 3, 1, 2), (self.gap,) * 4)
            bg = bg.permute(0, 2, 3, 1)                       # [K, th, tw, 3]
            if ty * tx > k:
                bg = torch.cat([bg, bg.new_zeros((ty * tx - k, *bg.shape[1:]))])
            bgm = (bg.view(ty, tx, self.tile_h, self.tile_w, 3)
                     .permute(0, 2, 1, 3, 4)
                     .reshape(ty * self.tile_h, tx * self.tile_w, 3))
            out = torch.where((idx == _PX_TRANSPARENT)[..., None], bgm, out)
        return out

    def frame(self, osd: torch.Tensor, attrs: "torch.Tensor | None" = None,
              grid: "tuple[int, int] | None" = None, blink_phase: bool = False,
              scale: float = 1.0,
              background: "torch.Tensor | None" = None) -> torch.Tensor:
        """mosaic() plus optional area-resampling to scale, as HWC uint8."""
        img = self.mosaic(osd, attrs, grid, blink_phase, background)
        if scale != 1.0:
            f = img.permute(2, 0, 1)[None].float()
            f = torch.nn.functional.interpolate(
                f, scale_factor=scale, mode="area", recompute_scale_factor=False)
            img = f[0].round().clamp(0, 255).to(torch.uint8).permute(1, 2, 0)
        return img
