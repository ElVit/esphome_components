---
name: create-pull-request
description: "Use when creating, preparing, or updating a GitHub pull request, including changing an existing PR title or body. Build PR content only from commits and changes already present on the origin remote; exclude all local-only changes."
argument-hint: "Optionally provide a title, base branch, or draft preference"
---

# Create or Update a Pull Request

## When to Use

- Create or prepare a GitHub pull request from the current branch.
- Update the title or body of an existing GitHub pull request.
- Check whether the GitHub MCP server is available and help configure it when needed.

## Procedure

1. **Verify GitHub access.** Call the GitHub MCP `get_me` tool first. If it succeeds, continue. If the GitHub tools are unavailable or authentication fails, follow the MCP setup and troubleshooting section below before retrying.
2. **Resolve the target and operation.** For an update, use the PR number or URL supplied by the user. If neither is supplied, identify a unique open PR matching the current repository, remote head, and base. If the target is ambiguous, ask before editing. For creation, determine the current branch, push remote, and default base branch; use the remote branch name as the head, not a local-only branch name or `owner:branch` value.
3. **Read remote state.** For creation, fetch the push remote and confirm that the head and base exist there. For an update, read the selected PR first and confirm its repository, number, title, body, head, and base.
4. **Use remote data only for PR content.** Derive or revise the title, summary, and change list exclusively from commits and the diff between the remote base and remote head, for example `origin/main..origin/<head>` and `origin/main...origin/<head>`. A user-provided, approved title or body may be used as supplied, but do not add unsupported claims. Never use the working-tree diff, untracked files, staged changes, or local commits absent from the remote. If the remote head is missing or has no commits ahead of base, explain what is missing; do not silently push local work.
5. **Avoid duplicate PRs.** Before creating, search for an open PR with the same repository, head, and base. If one exists, do not create another. Update it only when the user requested an update; otherwise report the existing PR and ask how they want to proceed.
6. **Create or update.** For creation, write a concise, factual title and body from the remote comparison. For an existing PR, use the GitHub MCP `update_pull_request` tool with the repository owner, repository name, and PR number; include only the fields the user asked to change. Do not change state, base, draft status, or reviewers unless requested. Do not use an issue-comment tool to change the PR description. Do not include `Co-authored-by`, AI attribution, or similar credit text. Include issue references only when supported by remote commits or user-provided context. Default new PRs to regular (not draft) unless requested otherwise.
7. **Handle confirmation and verify.** If the update tool opens an interactive edit/confirmation form, stop and wait for the user to submit it; do not claim the PR changed before submission. After the operation succeeds, read the PR again and verify the requested fields. Report the PR URL and number, what changed, and that local-only changes were excluded.

Do not commit, stage, or push changes unless the user explicitly asks. If asked to commit specific files, stage only those paths and leave unrelated local changes untouched. A local commit is not part of PR content until it is present on the remote.

## MCP Setup and Troubleshooting

Configure the GitHub MCP server in the active VS Code MCP configuration for the user's environment. This may be a workspace or user-level configuration; do not assume a particular file path. The HTTP endpoint for GitHub's hosted MCP server is:

```json
{
  "servers": {
    "github": {
      "type": "http",
      "url": "https://api.githubcopilot.com/mcp/"
    }
  }
}
```

To enable it in VS Code:

1. Use a VS Code version and GitHub Copilot Chat installation that support MCP servers.
2. Add the endpoint to the active MCP configuration, then start or restart the server from the MCP server controls. Approve the server if VS Code asks for trust.
3. Complete the GitHub sign-in/authorization prompt in VS Code. Do not add a personal access token or other secret to `mcp.json`.
4. Retry `get_me` to confirm that the server is connected and authenticated.

If it still fails, check that the JSON is valid and the URL is reachable, confirm the intended GitHub account is signed in and has access to the repository (including organization SSO when applicable), then restart the server or reload the VS Code window. Inspect the GitHub MCP server output in VS Code for the specific connection or authorization error. Do not claim that MCP works until a GitHub tool call succeeds.