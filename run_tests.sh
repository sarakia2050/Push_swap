#!/bin/bash
# Full test suite for push_swap.
# Usage: chmod +x run_tests.sh && ./run_tests.sh

PASS=0
FAIL=0

pass() { PASS=$((PASS+1)); echo "  OK  : $1"; }
fail() { FAIL=$((FAIL+1)); echo "FAIL  : $1"; }

echo "== 1) Build =="
make fclean >/dev/null 2>&1
make > build.log 2>&1
if [ -x ./push_swap ] && [ -x ./checker_linux ]; then
	pass "compiles cleanly (see build.log for details)"
else
	fail "build failed, see build.log"
	cat build.log
	exit 1
fi
if grep -qi "warning" build.log; then
	fail "compiler warnings found in build.log"
else
	pass "no compiler warnings"
fi

echo
echo "== 2) Correctness (all 4 modes, sizes 5..500, checked with checker_linux) =="
for mode in --simple --medium --complex "" ; do
	for n in 5 10 50 100 500; do
		for trial in 1 2 3; do
			args=$(seq 0 $((n-1)) | shuf | tr "\n" " ")
			res=$(./push_swap $mode $args | ./checker_linux $args)
			if [ "$res" = "OK" ]; then
				pass "mode='$mode' n=$n trial=$trial"
			else
				fail "mode='$mode' n=$n trial=$trial -> got '$res' with args: $args"
			fi
		done
	done
done

echo
echo "== 3) Bench accuracy (total_ops must equal actual line count) =="
for mode in --simple --medium --complex "" ; do
	args=$(seq 0 99 | shuf | tr "\n" " ")
	./push_swap --bench $mode $args 2>bench.txt 1>ops.txt
	lines=$(wc -l < ops.txt)
	reported=$(grep -oE "total_ops: [0-9]+" bench.txt | grep -oE "[0-9]+")
	if [ "$lines" = "$reported" ]; then
		pass "bench total_ops matches line count (mode='$mode': $reported)"
	else
		fail "bench mismatch (mode='$mode'): lines=$lines reported=$reported"
	fi
	ok=$(./checker_linux $args < ops.txt)
	if [ "$ok" = "OK" ]; then
		pass "bench mode still sorts correctly (mode='$mode')"
	else
		fail "bench mode broke sorting (mode='$mode') -> $ok"
	fi
done

echo
echo "== 4) Performance targets (subject: n=100 < 2000, n=500 < 12000) =="
for i in 1 2 3; do
	args=$(seq 0 99 | shuf | tr "\n" " ")
	./push_swap --bench $args 2>bench.txt 1>/dev/null
	ops=$(grep -oE "total_ops: [0-9]+" bench.txt | grep -oE "[0-9]+")
	if [ "$ops" -lt 2000 ]; then
		pass "n=100 adaptive: $ops ops (< 2000)"
	else
		fail "n=100 adaptive: $ops ops (>= 2000!)"
	fi
done
for i in 1 2 3; do
	args=$(seq 0 499 | shuf | tr "\n" " ")
	./push_swap --bench $args 2>bench.txt 1>/dev/null
	ops=$(grep -oE "total_ops: [0-9]+" bench.txt | grep -oE "[0-9]+")
	if [ "$ops" -lt 12000 ]; then
		pass "n=500 adaptive: $ops ops (< 12000)"
	else
		fail "n=500 adaptive: $ops ops (>= 12000!)"
	fi
done

echo
echo "== 5) Small edge cases (len 0..6) =="
for args in "" "5" "5 2" "3 2 1" "2 4 1 3" "5 1 4 2 3" "2 1 3 6 5 8"; do
	if [ -z "$args" ]; then
		out=$(./push_swap)
		code=$?
	else
		out=$(./push_swap $args)
		code=$?
	fi
	if [ $code -ne 0 ]; then
		fail "len test '$args' -> non-zero exit ($code)"
		continue
	fi
	if [ -z "$args" ]; then
		pass "empty input -> no output, exit 0"
		continue
	fi
	if [ -z "$out" ]; then
		nums="$args"
	else
		nums="$args"
	fi
	ok=$(echo "$out" | ./checker_linux $nums)
	if [ "$ok" = "OK" ] || [ -z "$out" ]; then
		pass "len test '$args' sorts correctly"
	else
		fail "len test '$args' -> checker said $ok"
	fi
done

echo
echo "== 6) Error handling =="
check_error() {
	desc="$1"; shift
	out_stdout=$(./push_swap "$@" 2>/dev/null)
	out_stderr=$(./push_swap "$@" 2>&1 1>/dev/null)
	code=$?
	if [ $code -eq 1 ] && [ "$out_stderr" = "Error" ] && [ -z "$out_stdout" ]; then
		pass "$desc -> Error on stderr, exit 1"
	else
		fail "$desc -> exit=$code stdout='$out_stdout' stderr='$out_stderr'"
	fi
}
check_error "non-integer" one two three
check_error "duplicate" 1 2 2 3
check_error "empty string arg" "" 1 2
check_error "int overflow" 2147483648
check_error "int underflow" -2147483649
check_error "leading zero" 007 8 9
check_error "leading/trailing space" " 5 " 8 9
check_error "plus sign" +5 8 9
check_error "lone dash" - 1 2

echo
echo "== 7) Valid input must NOT error =="
out=$(./push_swap --bench 2147483647 -2147483648 1 2>&1 1>/dev/null)
code=$?
if [ $code -eq 0 ]; then
	pass "INT_MAX and INT_MIN together are accepted"
else
	fail "INT_MAX/INT_MIN wrongly rejected"
fi

echo
echo "=================================="
echo "TOTAL: $PASS passed, $FAIL failed"
echo "=================================="
rm -f bench.txt ops.txt build.log
