/*Write a program to simulate the process of address translation using a two level
  page table with a page directory for virtual memory management.
  Design :
  ● Implement a two-level page table structure. The first level is a Page Directory
  (PD) which points to the Page Tables.
  ● Assume that a virtual address is divided into three parts
  ○ Page Directory Index (10 bits): Top level index that maps to an entry in the
  Page Directory.
  ○ Page Table Index (10 bits): Second level index that maps to an entry in the
  page table.
  ○ Offset (12 bits): Offset within the actual page

  ● Use the following configuration
  ○ Virtual Address Length: 32bit ( Eg : 0xCCC0FFEE)
  ○ Page Size: 4KB
  ○ Page Table Entries: Each page table has 1024 entries (each entry is of
  4B).

  ● Address Translation:
  ○ Implement an API consisting of two functions, load and store with the
  following signature as shown below.
  uint8_t load(uint32_t va) {
  assert(0 && “TODO : SIMULATE READING FROM va”);
  }

  void store(uint32_t va) {
  assert(0 && “TODO : SIMULATE WRITING INTO va”);
  }
  ○ Implement these functions to simulate memory access. If the address can
  be successfully translated to a physical address, then load/store the value
  into the physical address. If the address cannot be translated i.e., PD or
  Page Table Entry is invalid (null), simulate a page fault and update the PD
  and Page Tables accordingly.

  Deliverables:
  ● Hook into your load and store API’s to track and display the total number of page
  hits and page misses.
  ● The page table and the page directory size.
  ● For page misses, show the updates made to the PD and Page Table.
  ● Provide a summary at the end showing the hit/miss ratio.*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define PS 4096
#define PDE_ENTRIES 1024
#define PTE_ENTRIES 1024
#define PHYS_MEM_SIZE (PS * PTE_ENTRIES)

uint8_t physMem[PHYS_MEM_SIZE];
uint32_t pd[PDE_ENTRIES];
uint32_t pt[PDE_ENTRIES][PTE_ENTRIES];
uint32_t pfn[PDE_ENTRIES][PTE_ENTRIES];
uint8_t value;
uint32_t pHits = 0;
uint32_t pMiss = 0;

void check_pde(uint32_t pde) {

    if (pd[pde] == 0) {
        printf("Page Fault: Invalid PDE at index %u\n", pde);
        pMiss++;
        pd[pde] = 1;
        int i = 0; 
        while (i < PTE_ENTRIES) {
        pt[pde][i] = 0; 
        i++; }  
        }
}

void check_pte(uint32_t pde, uint32_t pte) {
    if (pt[pde][pte] == 0) {
        printf("Page Fault: Invalid PTE at index %u\n", pte);
        pt[pde][pte] = 1;
        pfn[pde][pte] = pte;
        printf("Updated PTE at PD Index %u, PT Index %u\n", pde, pte);
        
    }else {
       
        pHits++;
    }
   
}

void adrs(uint32_t va, uint32_t *pde, uint32_t *pte, uint32_t *offset) {
    *pde = (va >> 22) & 0x3FF;
    *pte = (va >> 12) & 0x3FF;
    *offset = va & 0xFFF;
}

uint8_t load(uint32_t va) {
    uint32_t pde, pte, offset;
    adrs(va, &pde, &pte, &offset);
    
    check_pde(pde);
    check_pte(pde,pte);

    
    return physMem[(pfn[pde][pte] << 12) | offset];
}

void store(uint32_t va) {
    uint32_t pde, pte, offset;
    adrs(va, &pde, &pte, &offset);
    check_pde(pde);
    check_pte(pde,pte);
    printf("Enter value to store: ");
    scanf("%hhu", &value);
    physMem[(pfn[pde][pte] << 12) | offset] = value;
}

int main() {
    uint32_t vAddr;
    char op[10];
    
    while (1) {
        printf("\nEnter operation (load/store) or type 'exit' to quit: ");
        scanf("%s", op);
        if (strcmp(op, "exit") == 0) break;

        printf("\nEnter address (hex): ");
        scanf("%x", &vAddr);

        if (strcmp(op, "load") == 0) {
            uint8_t data = load(vAddr);
            printf("Loaded value: %u from virtual address: 0x%X\n", data, vAddr);
        } 
        else if (strcmp(op, "store") == 0) {
            store(vAddr);
            printf("\nStored value: %u at virtual address: 0x%X\n", value, vAddr);

        }
         else {
            printf("Invalid operation!\n");
        }

        printf("Total Page Hits: %u\nTotal Page Misses: %u\nHit/Miss Ratio: %.2f\n",
               pHits, pMiss, pHits == 0 ? 0 : (float)pHits / (pHits + pMiss));
    }
    printf("\nTotal Page Hits: %u\nTotal Page Misses: %u\nHit/Miss Ratio: %.2f\n",
               pHits, pMiss, pHits == 0 ? 0 : (float)pHits / (pHits + pMiss));
    printf("Page Directory Size: %u entries\nPage Table Size per PD Entry: %u entries\n",
           PDE_ENTRIES, PTE_ENTRIES);

    return 0;
}
