#!/bin/bash

SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
PROJ_DIR=$SCRIPT_DIR/..

format_dir () {
  find "$PROJ_DIR"/"$1" -name "*.cpp" -o -name "*.hpp" -o -name "*.h" | while read file; do
    clang-format -i --style=Mozilla "$file"
  done
}

format_dir "app"
format_dir "core"
format_dir "test"
