#!/bin/bash

# Command-line arguments for optional author filtering
AUTHOR="$1"

# Define the date range for the past week
SINCE_DATE=$(date -d '7 days ago' +%Y-%m-%d)
UNTIL_DATE=$(date +%Y-%m-%d)

# Capture metadata
BRANCH=$(git symbolic-ref --short HEAD)
LATEST_TAG=$(git describe --abbrev=0 --tags 2>/dev/null)

# Header
echo "Weekly Git Summary for $SINCE_DATE to $UNTIL_DATE"
echo "Current Branch: $BRANCH"
if [ -n "$LATEST_TAG" ]; then
  echo "Latest Tag: $LATEST_TAG"
fi

# Time Analysis
echo -e "\n--- Time Analysis ---"
git log --pretty=format:"%ad" --date=short --since="$SINCE_DATE" --until="$UNTIL_DATE" --author="$AUTHOR" | sort | uniq -c

# Author Contributions
echo -e "\n--- Author Contributions ---"
git shortlog -sn --since="$SINCE_DATE" --until="$UNTIL_DATE" --author="$AUTHOR"

# File Changes (C and H files)
echo -e "\n--- File Changes (C and H files) ---"
git diff --name-only -- '*.c' '*.h' --since="$SINCE_DATE" --until="$UNTIL_DATE" --author="$AUTHOR" | sort | uniq -c

# Lines of Code
echo -e "\n--- Lines of Code ---"
git diff --shortstat --since="$SINCE_DATE" --until="$UNTIL_DATE" --author="$AUTHOR"

# File and Directory Organization
echo -e "\n--- File and Directory Organization ---"
git log --name-status --since="$SINCE_DATE" --until="$UNTIL_DATE" --author="$AUTHOR" | awk '/^R[0-9]*\t/ || /^A\t/ || /^D\t/'

# Summary of Last N Commits
echo -e "\n--- Summary of Last 10 Commits ---"
git log --oneline --since="$SINCE_DATE" --until="$UNTIL_DATE" --author="$AUTHOR" | head -n 10

# Summary of Line Changes in C and H Files
echo -e "\n--- Summary of Line Changes in C and H Files ---"
git log --pretty=tformat: --numstat --since="$SINCE_DATE" --until="$UNTIL_DATE" --author="$AUTHOR" -- '*.c' '*.h' | awk '{ add += $1; subs += $2; loc += $1 - $2 } END { printf "added lines: %s, removed lines: %s, total lines: %s\n", add, subs, loc }'

# Summary
TOTAL_COMMITS=$(git rev-list --count --since="$SINCE_DATE" --until="$UNTIL_DATE" HEAD)
echo -e "\n--- Summary ---"
echo "Total Commits: $TOTAL_COMMITS"

# Exit
exit 0

