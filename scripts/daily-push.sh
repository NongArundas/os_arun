#!/usr/bin/env bash

set -euo pipefail

repo_root=$(git rev-parse --show-toplevel)
cd "$repo_root"

git add -A

if git diff --cached --quiet; then
    exit 0
fi

git commit -m "Daily progress $(date +%F)"
git push origin "$(git branch --show-current)"