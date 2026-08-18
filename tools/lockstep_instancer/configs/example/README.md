# Example config (test fixture)

- `stock_dump.txt` — the stock `dump all` of THIS tree's SITL_LOCKSTEP
  build (Betaflight 2026.6.1 defaults). Nothing custom: it is what the
  firmware itself emits. Regenerate after a firmware change:

  ```bash
  obj/multi/betaflight_SITL_LOCKSTEP_MULTI --dump-cli \
      tools/lockstep_instancer/configs/example/stock_dump.txt
  ```

Real per-drone configs live with the trainer that owns the drone (its
`drones/` directory), never in cudaflight.

Render an eeprom image from CLI text at use time:

```python
from pathlib import Path
import cudaflight

image = cudaflight.render_eeprom(Path("stock_dump.txt"))
```

Rendering is strict: any line the firmware rejects fails the render and
every rejected setting is named. Repair renamed settings with the
`overrides` argument (CLI text appended after the dump; last value wins).
List rejects without failing: `python -m cudaflight.config dump.txt`.

Never commit a rendered `.bin`: it is valid only for the exact firmware
build that wrote it. A stale image does not fail at boot — one
parameter-group version mismatch makes the firmware factory-reset the
whole config, silently. The render path fails loudly instead.
