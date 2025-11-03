#!/usr/bin/env bash
# Compile and run all testcases in the testcases/ directory
# Runs from the project root directory (where this script lives)

set -euo pipefail
shopt -s nullglob

# Color codes for terminal output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
CYAN='\033[0;36m'
BOLD='\033[1m'
NC='\033[0m' # No Color

ROOT_DIR="$(cd "$(dirname "$0")" && pwd)"
TESTDIR="$ROOT_DIR/testcases"
BIN_DIR="$ROOT_DIR/bin"
EXPECTED_DIR="$ROOT_DIR/expected_output"
OUTPUT_DIR="$ROOT_DIR/bin/outputs"

mkdir -p "$BIN_DIR"
mkdir -p "$OUTPUT_DIR"

if [ ! -d "$TESTDIR" ]; then
  echo "Error: testcases directory not found at $TESTDIR"
  exit 1
fi

# Ensure simplefs source files exist
for f in simplefs-ops.c simplefs-disk.c; do
  if [ ! -f "$ROOT_DIR/$f" ]; then
    echo "Error: required file '$f' not found in project root"
    exit 1
  fi
done

echo -e "${CYAN}${BOLD}Found testcases:${NC}" 
for src in "$TESTDIR"/*.c; do
  echo -e "  ${BLUE}→${NC} $(basename "$src")"
done

echo
echo -e "${YELLOW}════════════════════════════════════════════════════════════${NC}"
echo -e "${BOLD}Starting Test Execution${NC}"
echo -e "${YELLOW}════════════════════════════════════════════════════════════${NC}"
echo

# Track test results
PASSED_TESTS=()
FAILED_TESTS=()
TOTAL=0

# Compile & run each testcase
for src in "$TESTDIR"/*.c; do
  name=$(basename "$src" .c)
  bin="$BIN_DIR/$name"
  expected_out="$EXPECTED_DIR/$name.out"
  actual_out="$OUTPUT_DIR/$name.out"
  
  TOTAL=$((TOTAL + 1))
  
  echo -e "${CYAN}╔════════════════════════════════════════════════════════════╗${NC}"
  echo -e "${CYAN}║${NC} ${BOLD}Test: $name${NC}"
  echo -e "${CYAN}╚════════════════════════════════════════════════════════════╝${NC}"
  
  echo -e "${BLUE}Compiling${NC} $src → $bin"
  gcc -I "$ROOT_DIR" "$src" "$ROOT_DIR/simplefs-ops.c" "$ROOT_DIR/simplefs-disk.c" -o "$bin" 2>&1
  rc=$?
  if [ $rc -ne 0 ]; then
    echo -e "${RED}✗ Compilation failed${NC} for $src (exit $rc)"
    FAILED_TESTS+=("$name (compilation failed)")
    echo
    continue
  fi
  echo -e "${GREEN}✓ Compilation successful${NC}"
  echo

  echo -e "${BLUE}Running${NC} $bin..."
  echo -e "${YELLOW}────────────────────────────────────────────────────────────${NC}"
  "$bin" > "$actual_out" 2>&1
  echo -e "${YELLOW}────────────────────────────────────────────────────────────${NC}"
  echo
  
  # Check if expected output exists
  if [ ! -f "$expected_out" ]; then
    echo -e "${YELLOW}⚠ No expected output file found${NC}: $expected_out"
    echo -e "${YELLOW}⚠ Test status: UNKNOWN${NC}"
    echo
    continue
  fi
  
  # Diff the outputs
  if diff -q "$actual_out" "$expected_out" > /dev/null 2>&1; then
    echo -e "${GREEN}${BOLD}✓ PASS${NC} - Output matches expected"
    PASSED_TESTS+=("$name")
  else
    echo -e "${RED}${BOLD}✗ FAIL${NC} - Output differs from expected"
    echo -e "${YELLOW}Showing diff (expected vs actual):${NC}"
    diff -u "$expected_out" "$actual_out" | head -50 || true
    echo
    FAILED_TESTS+=("$name")
  fi
  
  echo
  echo -e "${CYAN}Finished $name${NC}"
  echo
done

echo -e "${YELLOW}════════════════════════════════════════════════════════════${NC}"
echo -e "${BOLD}Test Summary${NC}"
echo -e "${YELLOW}════════════════════════════════════════════════════════════${NC}"
echo
echo -e "${BOLD}Total Tests:${NC} $TOTAL"
echo -e "${GREEN}${BOLD}Passed:${NC} ${#PASSED_TESTS[@]}"
echo -e "${RED}${BOLD}Failed:${NC} ${#FAILED_TESTS[@]}"
echo

if [ ${#PASSED_TESTS[@]} -gt 0 ]; then
  echo -e "${GREEN}${BOLD}Passed Tests:${NC}"
  for test in "${PASSED_TESTS[@]}"; do
    echo -e "  ${GREEN}✓${NC} $test"
  done
  echo
fi

if [ ${#FAILED_TESTS[@]} -gt 0 ]; then
  echo -e "${RED}${BOLD}Failed Tests:${NC}"
  for test in "${FAILED_TESTS[@]}"; do
    echo -e "  ${RED}✗${NC} $test"
  done
  echo
fi

echo -e "${YELLOW}════════════════════════════════════════════════════════════${NC}"
if [ ${#FAILED_TESTS[@]} -eq 0 ] && [ $TOTAL -gt 0 ]; then
  echo -e "${GREEN}${BOLD}🎉 All tests passed!${NC}"
  exit 0
else
  echo -e "${RED}${BOLD}❌ Some tests failed${NC}"
  exit 1
fi
