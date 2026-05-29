//  Inputs to the program should be: the number of pages to touch and the number of trials.
// What is the size of the pages? the number?


/*

bash script starts this loop in another loop, each iter is nubmber of pages accessed + time

if pages have to get accessed multiple times, increase index per individual page access
to use different VPNs and not load an adress into the tlb. 

int jump = PAGESIZE / sizeof(int); // calc size of jump based on page size
for (i = 0; i < NUMPAGES * jump; i += jump) // jump to new page every iteration
a[i] += 1; // update int in new page

*/
#include <stdio.h>

int main(int argc, char **argv) {
    int NUMPAGES = argv[1];
    int TRIALS = argv[2];
    printf("argument at index 1: %s\n", argv[1]);


    int jump = PAGESIZE / sizeof(int);
    for (int i = 0; i < NUMPAGES * jump; i += jump) {// jump to new page every iteration
        a[i] += 1; // update int in new page
    }


    return 0;
}

// getconf PAGE_SIZE = 4096,
// manpage: getpagesize