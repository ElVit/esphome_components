---
name: git-commit-messages
description: 'Use when preparing or creating Git commits and commit messages. Keep messages focused on the change and omit AI attribution and Co-authored-by trailers.'
---

# Git Commit Messages

Use this skill when drafting or creating a Git commit in this repository.

## Rules

- Before committing, identify and run the lint and formatting checks in the workflow that applies to the changed files. If a check fails, fix the reported issues and rerun it; commit only after the applicable checks pass. If a required check cannot be run, disclose that before committing.
- Keep the entire commit message short and concise. Prefer one brief subject line; add a short body only when essential.
- Do not identify the assistant, an AI, or a model as an author or contributor.
- Never add `Co-authored-by` or similar AI attribution trailers.
- Do not add generated-by, assisted-by, or other AI-credit lines.
- Preserve any existing authorship metadata when working with existing commits; do not rewrite history unless explicitly requested.
