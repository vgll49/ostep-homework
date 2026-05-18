# Freespace Exercises – Answers

---

# 1. BEST Fit Policy

Run with:

```bash
python3 malloc.py -n 10 -H 0 -p BEST -s 0 -c
```

## Output

```text
ptr[0] = Alloc(3) returned 1000
Free List [ Size 1 ]: [ addr:1003 sz:97 ]

Free(ptr[0]) returned 0
Free List [ Size 2 ]: [ addr:1000 sz:3 ] [ addr:1003 sz:97 ]

ptr[1] = Alloc(5) returned 1003 (searched 2 elements)
Free List [ Size 2 ]: [ addr:1000 sz:3 ] [ addr:1008 sz:92 ]

Free(ptr[1]) returned 0
Free List [ Size 3 ]: [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:92 ]

ptr[2] = Alloc(8) returned 1008
Free List [ Size 3 ]: [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1016 sz:84 ]

Free(ptr[2]) returned 0
Free List [ Size 4 ]: [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:84 ]

ptr[3] = Alloc(8) returned 1008 (searched 4 elements)
Free List [ Size 3 ]: [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1016 sz:84 ]

Free(ptr[3]) returned 0
Free List [ Size 4 ]: [ addr:1000 sz:3 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:84 ]

ptr[4] = Alloc(2) returned 1000 (searched 4 elements)
Free List [ Size 4 ]: [ addr:1002 sz:1 ] [ addr:1003 sz:5 ] [ addr:1008 sz:8 ] [ addr:1016 sz:84 ]

ptr[5] = Alloc(7) returned 1010
Free List [ Size 4 ]: [ addr:1002 sz:1 ] [ addr:1003 sz:5 ] [ addr:1015 sz:1 ] [ addr:1016 sz:84 ]
```

## Observation

The free list grows quickly after repeated allocations and frees and becomes fragmented with many small blocks (e.g. size 1). This happens because no coalescing is used.

---

# 2. WORST Fit Policy

## Observation

```text
WORST fit always allocates from the largest available free block.
```

This repeatedly splits the biggest free region and can increase fragmentation over time.

Example:

```text
ptr[3] = Alloc(8) returned 1016
```

instead of reusing the existing `8-byte` free block at `1008`.

---

# 3. FIRST Fit Policy

## Answer

Using `FIRST` fit speeds up allocation because the allocator stops searching as soon as the first fitting free block is found.

Unlike `BEST` or `WORST`, it does not need to scan the entire free list every time.

---

# 4. Free List Ordering

Different free list orderings:

```text
ADDRSORT
SIZESORT+
SIZESORT-
```

affect how quickly fitting blocks are found.

## Observation

- `BEST` and `WORST` still scan most or all entries, so ordering matters less.
- `FIRST` fit is affected much more by ordering.
- `SIZESORT+` can help small allocations find fitting blocks earlier.
- `SIZESORT-` tends to allocate repeatedly from large regions first.

---

# 5. Coalescing (`-C`)

With many allocations (e.g. `-n 1000`):

- Without `-C`:
  - the free list becomes heavily fragmented
  - many tiny blocks appear
  - larger allocations fail more often over time

- With `-C`:
  - adjacent free blocks merge again
  - the free list stays smaller
  - larger allocations succeed more reliably

## Observation

Coalescing significantly reduces fragmentation.

---

# 6. Allocated Fraction (`-P`)

## Observation

- If `-P > 50`:
  - allocations happen more often than frees
  - the heap gradually fills up

- As `-P → 100`:
  - very little memory is freed
  - allocation failures become more common

- As `-P → 0`:
  - frees dominate
  - the heap stays mostly empty

---

# 7. Generating Heavy Fragmentation

## Example

```bash
python3 malloc.py -H 0 -p FIRST -l SIZESORT- \
-A +1,+1,+1,+1,+1,-0,-1,-2,-3,-4 -c
```

## Observation

`SIZESORT-` repeatedly allocates from the largest free block, which can fragment that region over time.

Using non-address-sorted free lists can also reduce effective coalescing and contribute to fragmentation.