2. Run with these flags: -s 0 -n 10. What value do you have to set
-l (the bounds register) to in order to ensure that all the generated
virtual addresses are within bounds?

A: l = max(VA) + 1

3. Run with these flags: -s 1 -n 10 -l 100. What is the maximum value that base can be set to, such that the address space still
fits into physical memory in its entirety?

A: base_max = physical_memory - 100

4. Run some of the same problems above, but with larger address
spaces (-a) and physical memories (-p).

A:
Same rules apply:

VA < l
PA = base + VA
Must still fit: base + l ≤ physical memory

Larger -a or -p just scales the ranges.

5. What fraction of randomly-generated virtual addresses are valid, as a function of the value of the bounds register? Make a graph
from running with different random seeds, with limit values ranging from 0 up to the maximum size of the address space.


A: Fraction of valid addresses increases linearly with l.

fraction valid ≈ l / address_space_size