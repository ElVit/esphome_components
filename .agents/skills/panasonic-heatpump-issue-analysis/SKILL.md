---
name: panasonic-heatpump-issue-analysis
description: 'Use when analyzing, triaging, or documenting GitHub issues about the Panasonic Heatpump ESPHome component. Compare local code and protocol behavior, write analysis in the configured language, and prepare an English reply for the issue reporter.'
---

# Panasonic Heatpump Issue Analysis

Use this skill to analyze GitHub issues for `components/panasonic_heatpump`. It covers protocol decoding, command encoding, model-specific behavior, UART timing, entity state, and issue notes. Analysis does not imply implementation; only change code when the user asks for a fix or implementation.

## Procedure

1. Read the issue title, full body, labels, state, and comments. Record the model, firmware/hardware, configuration, expected behavior, observed behavior, and available logs. Treat typos and user hypotheses as unverified until checked.
2. Form one concrete, falsifiable hypothesis from the report. Identify the smallest relevant local code path and a cheap check that could disconfirm it.
3. Trace the behavior to the code that computes or mutates it. Check the owning component code, entity adapter, schema, nearby tests, and examples only as needed. Distinguish entity registration from actual decoding or command generation.
4. For protocol questions, compare with the current upstream HeishaMon sources and protocol docs, especially `ProtocolByteDecrypt.md`, `MQTT-Topics.md`, `decode.h`, `decode.cpp`, and `commands.cpp`. Cite the source file or relevant table in the analysis. Upstream `main` can differ from the protocol/component version used by the reporter; call out that limitation.
5. Verify packet offsets explicitly. This component indexes the complete response, including its four-byte header. Do not copy upstream offsets without confirming whether they refer to packet indexes, payload indexes, or TOP byte numbers.
6. Compare decode and encode separately:
   - Decode: raw byte, bit mask/shift, sentinel adjustment, lookup table size, and output for every possible field code.
   - Encode: entity option index, encoded value, target command byte, preserved neighboring bits, and checksum path.
   - Confirm round-trip mappings where the protocol supports them; account for reserved or unset codes that are not valid entity options.
7. When raw logs are supplied, calculate the relevant byte fields from the actual packet and state exactly what the current code would return. Do not infer unsupported model behavior from one packet or from an empty/unset value alone.
8. Distinguish optimistic entity publication from device feedback. A value published immediately by a SET control, or a temporary hold that skips incoming state, is not proof that the heat pump accepted or reported the setting.
9. Report findings with evidence and confidence. Separate confirmed code behavior, protocol documentation, user observation, and unresolved model-specific questions. Recommend the next discriminating log or test rather than proposing a speculative decoder change.
10. Before writing the analysis, read `analysis/panasonic_heatpump/issue-analysis.env` and use the `PANASONIC_ISSUE_ANALYSIS_LANGUAGE` value for the analysis prose and any analysis note. If the file or variable is missing, empty, unreadable, or unsupported, default to English. The setting is local to this workspace and gitignored.
11. Always prepare a concise, directly usable response to the issue reporter, even when the user asks only for analysis. The proposed public reply must always be in English, regardless of the analysis language. Write it in the user's personal voice: use first-person singular (`I`, `me`, `my`) for requests and commitments. Do not speak as a group; avoid `we`, `us`, and `our`. Ask for the smallest concrete missing evidence or test that would distinguish the leading hypotheses; do not imply that an unverified fix is established. Keep the analysis separate from the proposed public reply.
12. If asked to write an issue analysis, add a focused Markdown note under `analysis/panasonic_heatpump/issue-<number>-<topic>.md`. This directory is intentionally gitignored for local analysis notes; do not force-add these files. Include the issue link, concise report, local/upstream mappings, concrete calculations, conclusion, verification steps, and a suggested reporter response in English. Do not duplicate unrelated issue history or silently alter existing notes.

## Panasonic Component Invariants

- Python platform `TYPES` order must match the C++ entity ID enum; entity IDs are platform-specific.
- TOP conversion helpers belong in `decode.h`/`decode.cpp`; command encoders belong in `commands.h`/`commands.cpp` or the established command mutators.
- A lookup table's first entry may encode its valid option count. `getTextState()` treats the lookup index as zero-based and returns `UNDEFINED` for negative or out-of-range values.
- A SET's optimistic state and its response-derived state are separate paths. Validate both before claiming feedback works.
- A correct byte/bit mapping does not prove that every heat-pump model exposes or updates that field.

## Validation

- Prefer a focused unit or integration test for the affected component slice; use repository guidance in `components/panasonic_heatpump/README.md` and `tests/panasonic_heatpump/README.md`.
- For changes to entity mappings, verify Python/C++ ID order, both SET and feedback paths, examples, and documentation.
- When tests cannot run, say why and report which static checks or manual packet calculations were completed.
- Do not modify user data, unrelated files, branches, or commits as part of issue analysis.
