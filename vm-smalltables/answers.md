# Answers
1. With a linear page table, you need a single register to locate the
page table, assuming that hardware does the lookup upon a TLB
miss. How many registers do you need to locate a two-level page
table? A three-level table?

A: Its still only onne PDBR needed, because from the Base i can get to every entry, no matter the levels, eg:

PDBR -> Page Directory -> Page Table -> PTE
PDBR -> Page Directory -> Level 1 -> Level 2 -> ... -> Level N -> PTE.

2. Use the simulator to perform translations given random seeds 0,
1, and 2, and check your answers using the -c flag. How many
memory references are needed to perform each lookup?

SEED 0:
PDBR: 108  (decimal) [This means the page directory is held in this page]

Virtual Address 611c: Translates To What Physical Address (And Fetches what Value)? Or Fault? 
VA: 1100001000 11100 [ VPN:10 | offset:5]


Virtual Address 3da8: Translates To What Physical Address (And Fetches what Value)? Or Fault?
Virtual Address 17f5: Translates To What Physical Address (And Fetches what Value)? Or Fault?

3. Given your understanding of how cache memory works, how do
you think memory references to the page table will behave in the
cache? Will they lead to lots of cache hits (and thus fast accesses?)
Or lots of misses (and thus slow accesses)?
