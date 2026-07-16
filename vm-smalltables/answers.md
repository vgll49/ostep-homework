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


SEED 1: 


Virtual Address 6c74: Translates To What Physical Address (And Fetches what Value)? Or Fault?

VA: 11011 00011 10100 [VPN:10 | offset: 5]

Offset = 20

PDINdex= 27 -> 0xa0 -> 1 [VALID] | [PFN] 010 0000 -> 32 
PTIndex= 3 -> 0xe1 -> 1 [VALID] | [PFN] 110 0001 -> PA 96

PTE|offset = 1100 0011 0100 -> 0xc34 -> Value: 0x06


Virtual Address 6b22: Translates To What Physical Address (And Fetches what Value)? Or Fault?

11010 11001 00010 [VPN:10 | offset:5]

Offset = 4
PDI=26 -> d2 -> 1 | PFN: 101 0010  -> 82
PTI=25 -> c7 -> 1 | PFN: 100 0111 -> 71 

PTE|offset= 1000 1110 0010 -> 0x8e2 -> Value: 0x1a

Virtual Address 03df: Translates To What Physical Address (And Fetches what Value)? Or Fault?

00000 11110 11111

PDI=0 -> da -> 1101 1010 -> 1 VALID | 101 1010 -> 90
PTI=30 -> 85 -> 1000 0101 -> 1 VALID | 000 0101 -> 5
offset=31

PTE|offset = 0000 1011 1111 -> 0x0bf -> Value: 0x0f

3. Given your understanding of how cache memory works, how do
you think memory references to the page table will behave in the
cache? Will they lead to lots of cache hits (and thus fast accesses?)
Or lots of misses (and thus slow accesses)?
