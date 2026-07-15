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
VA: 11000 01000 11100 [ VPN:10 | offset:5]

offset: 11100 -> 28

PDIndex: 11000 -> 24 -> PDE: 0xa1 -> 1   [VALID] | 0100001 -> 33
PTIndex: 01000 -> 8 -> PTE -> b5 -> 1 [VALID] | 011 0101 -> 53 
Valid = 1
PFN: 011 0101 -> Physical Adress: 53

PhysicalAdress = 0110 1011 1100 -> 0x6bc -> Value: 0x08

Virtual Address 3da8: Translates To What Physical Address (And Fetches what Value)? Or Fault?

01111 01101 01000 [ VPN:10 | offset:5]

offset: 8

PDIndex=15 -> PDE: 0xd6 -> 1 [VALID] | 101 0110 -> 86   
PTindex=13 -> 7f -> 0 [INVALID] |111 1111

Virtual Address 17f5: Translates To What Physical Address (And Fetches what Value)? Or Fault?
00101 11111 10101 

offset: 21

PDI=5 -> d4 -> 1101 0100 -> 1 [VALID] | [PFN] 101 0100 -> 84 
PTI=31 -> ce -> 1100 1110 -> 1 [VALID] | [PFN] 100 1110 -> Phyiscal Adress 78

PTE|offset = 1001 1101 0101 -> PA: 0x9d5 -> Value: 0x1c


3. Given your understanding of how cache memory works, how do
you think memory references to the page table will behave in the
cache? Will they lead to lots of cache hits (and thus fast accesses?)
Or lots of misses (and thus slow accesses)?
