---
name: git-commit-messages
description: 'Use only when the user explicitly asks to commit or to prepare a commit message. Never commit implicitly and omit AI attribution and Co-authored-by trailers.'
---

# Git Commit Messages

Use this skill when drafting or creating a Git commit in this repository.

## Rules

- Only commit when the user explicitly asks for a commit in the current turn. Requests to implement, change, fix, refactor, review, or clean up code are not permission to commit, stage, amend, or push.
- When asked to commit, commit only the intended files and leave unrelated working-tree changes untouched.
- Before committing, identify and run the exact lint and formatting checks from the workflow that applies to the changed files. Do not rely on a vague check or a "likely equivalent" command.
- For this repository, the C++ check is the workflow job `lint-code` in `.github/workflows/test_panasonic_heatpump.yml` and the required command is:
  `find components/panasonic_heatpump -name '*.cpp' -o -name '*.h' | xargs clang-format -style=file --dry-run --Werror`
- If the changed files are C++, run the exact clang-format command above before the commit and fix any reported formatting issues before proceeding.
- If the changed files are Python, run the relevant workflow checks (for example `black --check` and `flake8`) before the commit.
- If the changed logic is in the component and a focused unit test is relevant, run that targeted validation before committing.
- If a required check cannot be run, disclose that before committing and do not claim the change is validated.
- Commit only after the applicable check passes.
- Keep the entire commit message short and concise. Prefer one brief subject line; add a short body only when essential.
- Do not identify the assistant, an AI, or a model as an author or contributor.
- Never add `Co-authored-by` or similar AI attribution trailers.
- Do not add generated-by, assisted-by, or other AI-credit lines.
- Preserve any existing authorship metadata when working with existing commits; do not rewrite history unless explicitly requested.
