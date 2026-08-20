# Example config (test fixture)

- `stock_dump.txt` — the stock `dump all` of THIS tree's SITL_LOCKSTEP
  build (Betaflight 2026.6.1 defaults). Nothing custom: it is what the
  firmware itself emits. Regenerate after a firmware change:

  ```bash
  obj/multi/betaflight_SITL_LOCKSTEP_MULTI --dump-cli \
      tools/lockstep_instancer/configs/example/stock_dump.txt
  ```

Keep real per-vehicle configs with the project that owns the vehicle,
never in cudaflight — this directory holds test fixtures only.

Render an eeprom image from CLI text at use time:

```python
from pathlib import Path
import cudaflight

image = cudaflight.render_eeprom(Path("stock_dump.txt"))
```

The render refuses any dump whose firmware (header: release + commit) is
not this wheel's firmware — build the matching wheel with
`build_for_base.sh <commit>`. At exact parity, rejected lines are hardware
features the SITL build compiles out: skipped and reported. Overrides are
strict, and a round-trip re-dump verifies every sim-known value. Inspect a
dump: `python -m cudaflight.config dump.txt`.

Never commit a rendered `.bin`: it is valid only for the exact firmware
build that wrote it. A stale image does not fail at boot — one
parameter-group version mismatch makes the firmware factory-reset the
whole config, silently. The render path fails loudly instead.
