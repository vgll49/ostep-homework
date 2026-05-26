#1

1. 1024  
2. 2048  
3. 4096  

---

#2

Then, let’s understand how linear page table size changes as page
size grows. Before running any of these, try to think about the expected trends. How should page-table size change as the address
space grows? As the page size grows? Why not use big pages in general?

• -P 1k -a 1m -p 512m -v -n 0 -> 1024  
• -P 2k -a 1m -p 512m -v -n 0 -> 512  
• -P 4k -a 1m -p 512m -v -n 0 -> 256  

A: The page-table size should get less as the Page size increases. Dont use big pages because of Internal Fragmentation.

---

#3

Now let’s do some translations. Start with some small examples,
and change the number of pages that are allocated to the address
space with the -u flag. What happens as you increase the percentage of pages that are allocated in each address space?

• -P 1k -a 16k -p 32k -v -u 0  

A: All invalid bc nothing is allocated  

• -P 1k -a 16k -p 32k -v -u 25  

Only PFN 0 is valid. All invalid, because nothing is mapped to PFN 0.  

• -P 1k -a 16k -p 32k -v -u 50  

VPN0 -> PFN 0x6 -> dec 6  
VPN3 -> PFN 0x3 -> dec 3  

VA 0x00003229 (decimal: 12841) -> VALID, 0x6229  
VA 0x00001369 (decimal: 4969) -> Invalid  
VA 0x00001e80 (decimal: 7808) -> Invalid  
VA 0x00002556 (decimal: 9558) -> Invalid  
VA 0x00003a1e (decimal: 14878) -> VALID, 0x03a1e  

• -P 1k -a 16k -p 32k -v -u 75  

All PTEs Valid.  
VPN0 -> PFN 0x6  
VPN1 -> PFN 0x2  
VPN2 -> PFN 0x3  
VPN3 -> PFN 0x4  

VA 0x00003a1e (decimal: 14878) -> VALID, 0x4a1e  
VA 0x0000204c (decimal: 8268) -> VALID, 0x304c  
VA 0x00001209 (decimal: 4617) -> VALID, 0x2209  
VA 0x0000305f (decimal: 12383) -> VALID, 0x405f  
VA 0x00002793 (decimal: 10131) -> VALID, 0x3793  

• -P 1k -a 16k -p 32k -v -u 100  

Same as 75, because every PTE is valid

---

#4

Now let’s try some different random seeds, and some different (and
sometimes quite crazy) address-space parameters, for variety. Which
of these parameter combinations are unrealistic? Why?

• -P 8 -a 32 -p 1024 -v -s 1 VA: 5 bits, VPN 2 Bit.  
• -P 8k -a 32k -p 1m -v -s 2 VA: 12 bis, VPN 2 Bit,  

A: Internal fragmentation / unrealistic scale  

• -P 1m -a 256m -p 512m -v -s 3  

A: The last one is very unrealistic because the page size is 1MB. This would be a lot of waste and very inefficient page tables. 4KB pages are typical.

---

#5

Use the program to try out some other problems. Can you find the
limits of where the program doesn’t work anymore? For example,
what happens if the address-space size is bigger than physical memory?

A: If the address space is bigger than physical memory it won’t work anymore, because physical memory must be large enough to contain all mapped pages.