"""Betaflight firmware runtime: pre-built fatbin + host library + Python wrappers.

The torch and JAX environment classes live in submodules (``bfgym``, ``bfgym_jax``)
and are not imported by default so the base wheel has no heavy dependencies.
"""

from .bfgym_lib import default_fatbin_path, default_lib_path, load

__all__ = ["default_fatbin_path", "default_lib_path", "load"]
__version__ = "0.1.0"
