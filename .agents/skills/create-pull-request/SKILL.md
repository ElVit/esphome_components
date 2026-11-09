---
name: create-pull-request
description: "Use when creating or preparing a GitHub pull request. Build the title and description only from commits and changes already present on the origin remote; exclude all local-only changes."
argument-hint: "Optionally provide a title, base branch, or draft preference"
---

# Create a Pull Request

## When to Use

- Create or prepare a GitHub pull request from the current branch.
- Check whether the GitHub MCP server is available and help configure it when needed.

## Procedure

1. **Verify GitHub access.** Call the GitHub MCP `get_me` tool first. If it succeeds, continue. If the GitHub tools are unavailable or authentication fails, follow [MCP setup and troubleshooting](#mcp-setup-and-troubleshooting) before retrying.
2. **Identify the repository and branches.** Determine the current branch, its push remote, and the repository's default base branch. Use the remote branch name as the PR head, not a local-only branch name or `owner:branch` value.
3. **Refresh remote references.** Fetch the push remote before comparing branches. Confirm that both the head and base exist on that remote.
4. **Use remote data only for PR content.** Derive the title, summary, and change list exclusively from commits and the diff between the remote base and remote head, for example `origin/main..origin/<head>` and `origin/main...origin/<head>`. Never use the working-tree diff, untracked files, staged changes, or local commits absent from the remote when writing the PR title or body. Do not claim that such changes are included. If no remote head branch or no commits ahead of the base exists, explain what is missing; do not silently push local work.
5. **Check for an existing PR.** Search for an open PR with the same repository, head, and base. Update or report the existing PR instead of creating a duplicate.
6. **Create the PR.** Write a concise, factual title and description based only on the remote comparison. Do not include `Co-authored-by`, AI attribution, or similar credit text in the PR title or body. Include relevant issue references only when supported by the remote commits or user-provided context. Default to a regular PR unless the user requests a draft or says the work is not ready.
7. **Report the result.** Return the PR URL and number, base branch, and note that local-only changes were excluded.

Do not commit, stage, or push changes unless the user explicitly asks. If asked to commit specific files, stage only those paths and leave unrelated local changes untouched. A local commit is not part of PR content until it is present on the remote.

## MCP Setup and Troubleshooting

This repository configures the GitHub MCP server in `.vscode/mcp.json`:

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
2. Open `.vscode/mcp.json` and start the `github` server from its inline Start control or the Chat MCP server controls. Approve the server if VS Code asks for trust.
3. Complete the GitHub sign-in/authorization prompt in VS Code. Do not add a personal access token or other secret to `mcp.json`.
4. Retry `get_me` to confirm that the server is connected and authenticated.

If it still fails, check that the JSON is valid and the URL is reachable, confirm the intended GitHub account is signed in and has access to the repository (including organization SSO when applicable), then restart the server or reload the VS Code window. Inspect the GitHub MCP server output in VS Code for the specific connection or authorization error. Do not claim that MCP works until a GitHub tool call succeeds.