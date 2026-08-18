# SPDX-License-Identifier: GPL-3.0-or-later
"""Betaflight firmware runtime: pre-built fatbin + host library + Python wrappers.

The torch and JAX environment classes live in submodules (``torch_env``, ``jax_env``)
and are not imported by default so the base wheel has no heavy dependencies.
"""

from importlib.metadata import PackageNotFoundError, version

from .config import render_eeprom
from .lib import (
    default_cpu_lib_path,
    default_fatbin_path,
    default_lib_path,
    load,
    load_cpu,
)

__all__ = [
    "default_cpu_lib_path",
    "default_fatbin_path",
    "default_lib_path",
    "load",
    "load_cpu",
    "render_eeprom",
]

try:
    __version__ = version("cudaflight")
except PackageNotFoundError:  # running from a source tree, not an installed wheel
    __version__ = "0.0.0"
