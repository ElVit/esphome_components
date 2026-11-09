---
name: git-branch-names
description: 'Use when choosing, creating, or reviewing Git branch names in this repository.'
---

# Git Branch Names

Use this skill when naming a new branch. Existing branch names are historical and should not be renamed just to match this convention.

## Convention

For new branches, use:

```text
<type>/<scope>-<short-topic>
```

Use a lowercase type prefix from this set:

- `feature` for new behavior or entities
- `fix` for bug fixes
- `docs` for documentation or skill changes
- `test` for test-only work
- `refactor` for behavior-preserving code changes
- `chore` for maintenance and tooling

Use a concise scope such as `panasonic`, `maidesite`, or `ci`, followed by a short description of the outcome. Write the scope and topic in lowercase kebab-case, using ASCII characters.

Examples:

- `feature/panasonic-top144-169`
- `fix/panasonic-pressure-units`
- `docs/panasonic-entity-skill`
- `test/panasonic-full-config`
- `refactor/maidesite-uart-parser`

Keep names short but specific. Avoid spaces, underscores, personal names, dates, vague labels such as `work` or `temp`, and ticket numbers without a descriptive topic. A ticket ID may be included when it helps locate the work, for example `fix/panasonic-issue-123-pressure-units`.
