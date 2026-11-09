---
name: panasonic-heatpump-entities
description: 'Use when adding, modifying, reviewing, or debugging Panasonic Heatpump ESPHome entities: TOP telemetry, SET controls, sensor decoding, command encoding, entity IDs, schemas, tests, or documentation.'
---

# Panasonic Heatpump Entities

Use this skill for work in `components/panasonic_heatpump`. It describes how the component maps the Panasonic/HeishaMon protocol to ESPHome entities and how to add a read-only TOP or writable SET.

## Component Map

| Concern | Files | Role |
|---|---|---|
| Component registration and entity ownership | `components/panasonic_heatpump/__init__.py`, `panasonic_heatpump.h`, `panasonic_heatpump.cpp` | Registers the UART component, owns entity vectors, polls and publishes the response, mutates and queues command messages. |
| TOP telemetry | `sensor/`, `binary_sensor/`, `text_sensor/` | Python schema and C++ decoding for numeric, binary, and textual TOP entities. |
| Writable SET entities | `number/`, `select/`, `switch/` | Python schemas and C++ control/feedback paths for individual controls. |
| Combined controls | `climate/`, `water_heater/` | Higher-level entities that already own some SETs, including zone temperatures, DHW temperature, and operation modes. Check these before adding a duplicate standalone control. |
| Protocol conversions | `decode.h`, `decode.cpp` | Shared byte, bit, scaling, model, and text-state decoding helpers. Put TOP conversion logic here rather than embedding formulas in entity switches. |
| Command encoding | `commands.h`, `commands.cpp` | Converts user values/options into protocol bytes. |
| Examples and docs | `example_panasonic_heatpump.yaml`, `components/panasonic_heatpump/README.md` | User-facing configuration examples and supported entity lists. |
| Schema/integration coverage | `tests/panasonic_heatpump/test_panasonic_heatpump_full.yaml` | Full configuration that exercises component platforms and options. |

For protocol definitions, use the relevant current HeishaMon release documentation, especially [`MQTT-Topics.md`](https://github.com/heishamon/HeishaMon/blob/main/MQTT-Topics.md), [`ProtocolByteDecrypt.md`](https://github.com/heishamon/HeishaMon/blob/main/ProtocolByteDecrypt.md), and [`Manage-Topics.md`](https://github.com/heishamon/HeishaMon/blob/main/Manage-Topics.md). Confirm the topic name, byte offset, scaling, bit positions, unset encoding, units, and model availability; do not infer undocumented meanings.

## Add a TOP

1. Confirm the TOP's public meaning and wire encoding from the upstream topic and protocol tables. Protocol byte numbers are decimal. Match the component's existing indexing convention, which reads the complete response vector including its header; do not blindly add or subtract the header size.
2. Choose the ESPHome platform by value semantics:
   - `sensor` for numeric temperatures, counters, percentages, raw codes, and other measurements/configuration values.
   - `binary_sensor` only for a true two-state value. If the protocol has an unset code such as `-1`, map it deliberately; a binary entity cannot represent unknown.
   - `text_sensor` for enumerations and readable labels. Add a lookup array in `decode.h` and its out-of-class definition in `decode.cpp`, following existing tables.
3. In that platform's `__init__.py`, add the `CONF_TOP...` constant, append it to `TYPES`, and add a `cv.Optional(...)` schema with correct units, device class, state class, and options. `to_code()` assigns `set_id(index)` from `TYPES`.
4. Add the same ID at the matching position in the platform's C++ enum in its header. The Python `TYPES` order and C++ enum order must be identical. Prefer appending new IDs so existing positional IDs do not shift.
5. Add the C++ `publish_new_state()` case using the documented byte and centralized `PanasonicDecode` helpers. Put a new reusable conversion/bit helper in `decode.h` and `decode.cpp`; preserve sentinel values and avoid interpreting undocumented bit patterns. Text entities should use the existing `getTextState()` pattern.
6. Add the entity to the full test YAML, `example_panasonic_heatpump.yaml`, and the corresponding supported-entity section in `components/panasonic_heatpump/README.md`.

## Add a SET

1. Confirm the SET's writable range, command byte/bit field, encoding, and readback source from the upstream command/protocol documentation. Decide whether it belongs in `number`, `select`, or `switch`; if it is already part of a `climate` or `water_heater` workflow, extend that abstraction instead of adding a duplicate control.
2. In the chosen platform's `__init__.py`, add the `CONF_SET...` constant and schema. Add it to `TYPES` in a stable order. For `select`, keep the corresponding `CONF_SELECTS` options at the same index and in the exact protocol order.
3. Add the matching C++ enum entry in the same order as Python. Implement both directions in the platform C++ file: user control to command bytes and response bytes to entity state. For numbers, define limits, units, and step; for selects, validate option mapping; for switches, map the exact active/inactive encodings.
4. Use `PanasonicCommand` helpers for wire encodings (for example `setPlus128`, `setPlus1`, or a dedicated encoder when necessary). Use the component's `set_command_byte`, nibble, or curve mutator to send the command; these build the command packet, calculate its checksum, and queue it. Preserve neighboring bit fields when a command only changes part of a byte. `set_command_curve()` has special grouped behavior for curve values.
5. Keep optimistic `publish_state()` and `keep_state_` behavior consistent with neighboring controls, and implement response feedback separately using the documented response byte and `PanasonicDecode` helpers.
6. Add the SET to the full test YAML, the root example, and the README list. If the setting is also exposed through climate/water-heater control, test that path rather than registering a second competing entity.

## ID and Behavior Invariants

- Each platform has its own positional ID space. The index used by Python `TYPES` must match that platform's C++ enum; IDs are not globally shared across platforms.
- Changing the order of an existing `TYPES` list or enum can silently route entities to the wrong C++ `switch` case. Append new entries and update both sides together.
- A TOP is read-only unless a documented command exists. Do not create a SET from a similarly named TOP by assumption.
- Numeric scaling and protocol bit extraction belong in `PanasonicDecode`, not in individual entity cases. Keep entity code focused on selecting the byte/helper and publishing state.
- A command byte index includes the protocol header in the same way as the existing `set_command_*` calls. Verify offsets against nearby established controls and protocol documentation.
- Do not overwrite another field sharing a byte or nibble. Use the nibble mutators or preserve the other bits from the latest response before sending.

## Validation

- Check Python/C++ ID ordering and all input/output paths for the changed platform.
- Run `pytest tests/panasonic_heatpump -v` and validate/build `tests/panasonic_heatpump/test_panasonic_heatpump_full.yaml` with ESPHome when available.
- Check the user example and README for the new configuration key, then run `git diff --check`.
