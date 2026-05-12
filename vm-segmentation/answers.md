# Segmentation Exercises – Answers

---

# 1. Address Translation with Tiny Address Spaces

## Command

```bash
segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 0
```

### Answer

```text
VA 0: SEG1 → dec 492
VA 1: SEG1 → SEGFAULT
VA 2: SEG0 → SEGFAULT
VA 3: SEG0 → SEGFAULT
VA 4: SEG1 → SEGFAULT
```

---

## Command

```bash
segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 1
```

### Answer

```text
VA 0: SEG0 → dec 17
VA 1: SEG1 → negative offset = 108 - 128 = -20 → dec 492
VA 2: SEG1 → SEGFAULT
VA 3: SEG0 → SEGFAULT
VA 4: SEG0 → SEGFAULT
```

---

## Command

```bash
segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2
```

### Answer

```text
VA 0: SEG1 → negative offset = 122 - 128 = -6  → dec 506
VA 1: SEG1 → negative offset = 121 - 128 = -7  → dec 505
VA 2: SEG0 → dec 7
VA 3: SEG0 → dec 10
VA 4: SEG1 → negative offset = 106 - 128 = -22 → SEGFAULT
```

---

# 2. Legal and Illegal Virtual Addresses

Using:

```bash
segmentation.py -a 128 -p 512 -b 0 -l 20 -B 512 -L 20 -s 2
```

## Answer

- Highest legal VA in Segment 0:
  
  ```text
  19
  ```

- Lowest legal VA in Segment 1:
  
  ```text
  108
  ```

- Lowest illegal VA:
  
  ```text
  20
  ```

- Highest illegal VA:
  
  ```text
  107
  ```

## Example Validation

```bash
python3 -m segmentation -a 128 -p 512 -A 19,20,107,108 -b 0 -l 20 -B 512 -L 20 -c
```

---

# 3. Generate Specific Translation Results

## Goal

Generate:

```text
valid, valid, violation, ..., violation, valid, valid
```

## Command

```bash
segmentation.py -a 16 -p 128 \
-A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 \
--b0 ? --l0 ? --b1 ? --l1 ?
```

## Answer

```text
--b0 0
--l0 2
--b1 128
--l1 2
```

## Validation

```bash
python3 -m segmentation -a 16 -p 128 \
-A 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15 \
--b0 0 --l0 2 --b1 128 --l1 2 -c
```

---

# 4. Generate ~90% Valid Virtual Addresses

## Answer

```bash
python3 -m segmentation -a 90 -p 100 -n 100 \
--b0 0 --l0 40 --b1 100 --l1 40 -c
```

## Important Parameters

- Address space size (`-a`)
- Segment limits (`--l0`, `--l1`)
- Number of generated addresses (`-n`)

The segment sizes should cover roughly 90% of the virtual address space.

---

# 5. Generate Only Invalid Virtual Addresses

## Option 1

Generate addresses outside all segment bounds.

```bash
python3 -m segmentation -a 90 -A 41 -p 100 \
--b0 0 --l0 40 --b1 100 --l1 40 -c
```

---

## Option 2

Configure both segment lengths to `0`.

```text
--l0 0
--l1 0
```

This makes every generated virtual address invalid.