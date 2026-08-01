*This project has been created as part of the 42 curriculum by fkia, kwaku.*

# push_swap

## Description

`push_swap` sorts a stack of integers using two stacks (`a` and `b`) and a
restricted set of stack operations (`sa`, `sb`, `ss`, `pa`, `pb`, `ra`, `rb`,
`rr`, `rra`, `rrb`, `rrr`), aiming to use as few operations as possible. Thechmod +x checker_linux
make fclean
make
./push_swap 2 1 3 6 5 8
./push_swap 3 2 1
./push_swap 5 1 4 2 3
./push_swap --simple 5 2 9 1 8 3 7 6 4 0
./push_swap --medium 5 2 9 1 8 3 7 6 4 0
./push_swap --complex 5 2 9 1 8 3 7 6 4 0
./push_swap --bench --medium 5 2 9 1 8 3 7 6 4 0 2>&1 1>/dev/null
./push_swap 3 2 1 5 4 8 7 6 9 0 | ./checker_linux 3 2 1 5 4 8 7 6 9 0
./push_swap one two three
./push_swap 1 2 2 3
./push_swap ""
./push_swap --bench $(seq 0 49 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench $(seq 0 99 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench $(seq 0 499 | shuf | tr '\n' ' ') 2>&1 1>/dev/null

./push_swap --bench --simple $(seq 0 49 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench --simple $(seq 0 99 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench --simple $(seq 0 499 | shuf | tr '\n' ' ') 2>&1 1>/dev/null

./push_swap --bench --simple $(seq 0 49 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench --medium $(seq 0 49 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench --complex $(seq 0 49 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
program measures how disordered the input is and picks one of four sorting
strategies accordingly (or a strategy can be forced with a flag). An optional
`--bench` mode reports the disorder, the strategy used, and a breakdown of
every operation type.

## Instructions

### Build

```sh
make        # build push_swap
make re     # rebuild from scratch
make clean  # remove object files
make fclean # remove object files and the binary
make debug  # rebuild with -g
```

### Usage

```sh
./push_swap 2 1 3 6 5 8
./push_swap "2 1 3 6 5 8"
```

Numbers must be valid, non-duplicate integers within `int` range. Input is
capped at 600 numbers. On invalid input, the program prints `Error` and
exits with status 1.

### Strategy selectors

| Flag         | Effect                                            |
|--------------|----------------------------------------------------|
| *(none)*     | Adaptive: strategy is chosen from the disorder      |
| `--simple`   | Forces the O(n²) strategy                           |
| `--medium`   | Forces the O(n·√n) strategy                         |
| `--complex`  | Forces the O(n·log n) strategy                      |
| `--bench`    | Also prints benchmark stats to stderr (can be combined with any of the above) |

```sh
./push_swap --bench --medium 5 2 9 1 8 3 7 6 4 0
```

## Algorithms and disorder thresholds

Disorder is computed once, before any move, as the fraction of
out-of-order pairs in the input (0 = already sorted, 1 = reverse sorted).
In adaptive mode (no strategy flag), it is used to pick the cheapest
strategy that is known to behave well for that amount of disorder:

| Disorder range     | Strategy used | Complexity   |
|---------------------|---------------|--------------|
| `< 0.001`            | none (already sorted) | O(1)  |
| `< 0.2`              | Simple        | O(n²)        |
| `0.2` – `< 0.5`      | Medium        | O(n·√n)      |
| `>= 0.5`             | Complex       | O(n·log n)   |

Stacks of 5 elements or fewer always go through a small dedicated
`sorting_manual_sort` (direct case analysis for 2–3 elements, then a
rank-based insertion for up to 5), since a general-purpose strategy adds
overhead a fixed, tiny case doesn't need.

The `0.2` / `0.5` thresholds were chosen empirically: below `0.2` the input
is close enough to sorted that a straightforward O(n²) selection pass
finishes in very few operations; above `0.5` the input is close to a full
random shuffle, where only an O(n·log n) approach keeps the operation count
within the subject's performance targets for large `n` (500 elements). The
`0.2`–`0.5` band is handled by the O(n·√n) bucket strategy, which is cheaper
than the O(n log n) one when the input isn't fully scrambled.

**Simple — O(n²) (`sorting_turkish_sort`)**
A selection-sort adaptation: repeatedly scan stack `a` for its current
minimum, rotate it to the top (`ra`/`rra`, whichever is shorter), and push
it to `b`. Once `a` is empty, `b` — which holds the values in descending
order — is drained back onto `a` with `pa`, producing an ascending stack.
Each of the `n` selections costs up to `O(n)` (scan + rotate), giving
`O(n²)` overall.

**Medium — O(n·√n) (`sorting_chunk_sort`)**
The value range is split into buckets of size `~√(2n)`. Starting from the
highest bucket, a single linear pass over the remaining stack `a` moves
every element in that bucket's range into `b` (elements that don't match are
just rotated past, not removed — so the pass costs `O(remaining length)`,
not more). The small bucket collected in `b` is then sorted in place
(repeated "find max, rotate to top, push to `a`") and drained onto `a`.
Working from the highest bucket down to the lowest means each bucket's
sorted group lands under the previous one, and the whole stack ends up
ascending. With `~√n` buckets, each costing `O(n)` to collect and `O(bucket
size²) ≈ O(n)` to internally sort, the total is `O(n·√n)`.

**Complex — O(n·log n) (`sorting_radix_sort`)**
A stack-based radix sort (LSD, binary): for each bit position from the
lowest to the highest needed bit, every element of `a` is inspected; if the
bit is 0 it's pushed to `b`, otherwise it's rotated to the back of `a` so it
is revisited after the elements with a 0 bit; once a full pass over `a` is
done, `b` is drained back onto `a`. After `log₂(n)` passes (one per bit),
the stack is sorted, for a total of `O(n·log n)` operations.

## Benchmark output (`--bench`)

Printed to **stderr** only, so it doesn't interfere with the operation
stream on stdout (which is what a `checker` validates):

```
[bench] disorder: 49.82%
[bench] strategy: Medium / O(n*sqrt(n))
[bench] total_ops: 987
[bench] sa: 0 sb: 0 ss: 0 pa: 6 pb: 6
[bench] ra: 6 rb: 1 rr: 0 rra: 0 rrb: 0 rrr: 0
```

`strategy` shows the selector name (`Adaptive` if no flag was given, or the
forced one) together with the complexity class of whichever sorting
function actually ran.

## Resources

- 42 subject PDF for this project (algorithm requirements, disorder metric,
  performance targets)
- Wikipedia: [Radix sort](https://en.wikipedia.org/wiki/Radix_sort),
  [Selection sort](https://en.wikipedia.org/wiki/Selection_sort)
- **AI usage:** Claude (Anthropic) was used to help implement and debug the
  Simple (`sorting_simple_algo.c`) and Medium (`sorting_medium_algo.c`)
  sorting strategies and the `--bench` output (`bench.c`), and to review
  norm compliance (line/function/parameter limits) on those files. All
  AI-suggested code was tested against `checker_linux` and reviewed before
  being kept.

## Layout

| File                     | Purpose                                             |
|--------------------------|------------------------------------------------------|
| `main.c`                 | Entry point, input parsing, orchestration             |
| `helper_funcs_1.c`       | `ft_strncmp`, `ft_atoi`, `ft_itoa`, `ft_strlen`        |
| `helper_funcs_2.c`       | Duplicate check, rank normalization, `ft_putstr`      |
| `helper_funcs_3.c`       | Stack operation primitives (`px`, `rx`, `sx`, ...)    |
| `helper_funcs_4.c`       | `compute_disorder`, `populate_stack`, `perform_rotations` |
| `dl_list_utils.c`        | Doubly linked list implementation                     |
| `sorting_manual.c`       | Sorting for small stacks (≤5)                         |
| `sorting_simple_algo.c`  | Simple / O(n²) strategy                               |
| `sorting_medium_algo.c`  | Medium / O(n·√n) strategy                             |
| `sorting_complex_algo.c` | Complex / O(n·log n) strategy, `rec_op` op counter     |
| `bench.c`                | `--bench` stats output (to stderr)                    |
| `push_swap.h`            | Shared types and prototypes                           |

## Contributions

- **kwaku**: argument parsing/validation, doubly linked list and stack
  primitives, manual sort (≤5 elements), Complex/radix strategy, operation
  counter (`rec_op`)
- **fkia**: Simple and Medium sorting strategies, `--bench` mode, this
  README





  chmod +x checker_linux
make fclean
make
./push_swap 2 1 3 6 5 8
./push_swap 3 2 1
./push_swap 5 1 4 2 3
./push_swap --simple 5 2 9 1 8 3 7 6 4 0
./push_swap --medium 5 2 9 1 8 3 7 6 4 0
./push_swap --complex 5 2 9 1 8 3 7 6 4 0
./push_swap --bench --medium 5 2 9 1 8 3 7 6 4 0 2>&1 1>/dev/null
./push_swap 3 2 1 5 4 8 7 6 9 0 | ./checker_linux 3 2 1 5 4 8 7 6 9 0
./push_swap one two three
./push_swap 1 2 2 3
./push_swap ""
./push_swap --bench $(seq 0 49 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench $(seq 0 99 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench $(seq 0 499 | shuf | tr '\n' ' ') 2>&1 1>/dev/null

./push_swap --bench --simple $(seq 0 49 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench --simple $(seq 0 99 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench --simple $(seq 0 499 | shuf | tr '\n' ' ') 2>&1 1>/dev/null

./push_swap --bench --simple $(seq 0 49 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench --medium $(seq 0 49 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
./push_swap --bench --complex $(seq 0 49 | shuf | tr '\n' ' ') 2>&1 1>/dev/null
