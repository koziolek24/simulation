#!/bin/bash

if ! command -v clang-format &>/dev/null; then
  echo "Error: clang-format is not installed"
  exit 1
fi

find ./src ./include ./tests -type f \( -name "*.cc" -o -name "*.h" \) -exec clang-format -i {} \;

echo "Code formatting complete"
