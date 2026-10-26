# Task 5 — provisional power budget

Using the estimates in `power_budget.csv`:
- Idle estimate = 80 + 25 + 5 = **110 mA** (motors stopped).
- Typical running estimate = 100 + 25 + 5 + (2 × 250) = **630 mA**.
- Illustrative peak/stall estimate = 200 + 50 + 10 + (2 × 1200) = **2660 mA (2.66 A)**.

These are not verified component ratings. The motor value is a placeholder, so the total must be recalculated from the exact motor datasheet before the battery or driver is finalized.

For a 3S LiPo with assumed capacity 1300 mAh and derating factor 0.75:
Runtime ≈ (1300 mAh / 630 mA) × 0.75 ≈ **1.55 hours** under the stated typical-current assumption. Real runtime can be lower due to acceleration, turning, battery ageing, voltage cutoff and motor load.

A LiPo pack usually offers high discharge capability and low mass, useful for motor current, but requires careful balance charging and protection against puncture/over-discharge. Cylindrical Li-ion cells are mechanically more robust but their discharge rating varies widely; a pack must be selected by cell datasheet and protection requirements, not chemistry name alone. Ensure the pack's discharge rating safely exceeds measured peak demand and use a compatible 3S charger.
