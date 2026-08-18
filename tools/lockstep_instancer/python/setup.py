# SPDX-License-Identifier: GPL-3.0-or-later
"""Setuptools shim for the wheel tag.

The wheel ships prebuilt ``.so``s, ``fw.fatbin`` and the OSD font as
package data, so it is bound to Linux x86_64 + the CUDA driver but
contains no Python C extensions — the right tag is
``py3-none-linux_x86_64``: Python-version-agnostic ABI, platform-locked.
Achieved by:

1. Overriding ``has_ext_modules`` so setuptools emits a platform-specific
   wheel (otherwise it'd be ``py3-none-any``).
2. Overriding ``bdist_wheel.get_tag`` to drop the CPython-specific
   ``cp3XX-cp3XX`` slot in favor of ``py3-none``.
"""

from setuptools import setup
from setuptools.dist import Distribution

try:  # setuptools >= 70.1 vendors bdist_wheel; the wheel import is deprecated
    from setuptools.command.bdist_wheel import bdist_wheel
except ImportError:
    from wheel.bdist_wheel import bdist_wheel


class _PlatformDist(Distribution):
    def has_ext_modules(self) -> bool:
        return True


class _PlatBdistWheel(bdist_wheel):
    def get_tag(self) -> "tuple[str, str, str]":
        _, _, plat = super().get_tag()
        return "py3", "none", plat


setup(distclass=_PlatformDist, cmdclass={"bdist_wheel": _PlatBdistWheel})
