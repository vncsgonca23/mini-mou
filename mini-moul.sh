#!/bin/bash

SCRIPT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)"
source "$SCRIPT_DIR/mini-moul/config.sh"

# assignment name
assignment=NULL
WORK_DIR="$(pwd)"

function handle_sigint {
  echo ""
  echo "${RED}Script aborted by user. Cleaning up...${DEFAULT}"
  rm -rf "$WORK_DIR/mini-moul"
  echo "${GREEN}Cleaning process done.${DEFAULT}"
  exit 1
}

run_norminette() {
  if command -v norminette &> /dev/null; then
    if [ "$assignment" = "C08" ]; then
      norminette -R CheckForbiddenSourceHeader -R CheckDefine
    else
      norminette -R CheckForbiddenSourceHeader
    fi
  else
    echo "norminette not found, skipping norminette checks"
  fi
}

normalize_assignment() {
  local raw="$1"
  local lower
  lower="$(printf '%s' "$raw" | tr '[:upper:]' '[:lower:]')"
  case "$lower" in
    c0[0-9]|c1[0-3])
      printf 'C%s' "${lower#c}"
      ;;
    shell0[0-1])
      printf 'Shell%s' "${lower#shell}"
      ;;
    rush00)
      printf 'Rush00'
      ;;
    *)
      printf '%s' "$raw"
      ;;
  esac
}

# Function to determine if argument or current directory matches a supported project
detect_assignment() {
  local raw
  if [ -n "$1" ]; then
    raw="$1"
  else
    raw="$(basename "$WORK_DIR")"
  fi
  assignment="$(normalize_assignment "$raw")"
  [[ $assignment =~ ^(C(0[0-9]|1[0-3])|Shell0[0-1]|Rush00)$ ]]
}

if detect_assignment "$1"; then
  if [[ $assignment =~ ^(C|Rush) ]]; then
    run_norminette
  fi
  rm -rf "$WORK_DIR/mini-moul"
  cp -R "$SCRIPT_DIR/mini-moul" "$WORK_DIR/mini-moul"
  trap handle_sigint SIGINT
  cd "$WORK_DIR/mini-moul" || exit 1
  ./test.sh "$assignment"
  status=$?
  cd "$WORK_DIR" || exit 1
  rm -rf "$WORK_DIR/mini-moul"
  exit "$status"
else
  printf "${RED}Current directory does not match expected pattern (C[00~13], Shell[00~01], or Rush00).${DEFAULT}\n"
  printf "${RED}Please navigate to an appropriate directory (or pass the project name, e.g. 'mini Shell00').${DEFAULT}\n"
  exit 1
fi
