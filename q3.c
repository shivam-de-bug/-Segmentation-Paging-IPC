/*Write a program that simulates paging-based memory management and address
  translation using a Page Table and Translation Lookaside Buffer (TLB). The program
  should demonstrate how logical addresses are translated into physical addresses using
  a page table and how the TLB serves as a cache to speed up this process. Add your
  assumptions, if any, to a ReadMe file.
  Design:
  ● Implement a page table that maps virtual page numbers (VPNs) to physical
  frame numbers (PFNs).
  ● Assume a fixed page size of 4KB and 64KB of total memory.
  ● Simulate a TLB that caches recent translations. The TLB size should be small
  enough that it cannot hold all the pages in the system.
  ● The TLB should be consulted first to check if the translation exists (TLB hit or
  miss).
  ● On a TLB miss, simulate accessing the page table to fetch the PFN and update
  the TLB.
  ● Use the pseudocode from Chapter 19 for the address translation.
  Input: The user can enter a 16-bit logical address in hex format. For example: a56f
  Output: The physical address in hex format and a string telling if this was a TLB hit or
  TLB miss.*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    int vpn;    
    int pfn;    
    bool valid; 
} TLBEntry;

typedef struct {
    int pfn;    
    bool valid; 
    int protectBits; //for access 
} PageTableEntry;

// function to see hit or miss in the TLB for a given VPN
int adrs_in_tlb(TLBEntry tlb[], int tlb_size, int vpn, bool *success) {
    int i = 0;
    while (i < tlb_size) {
        if (tlb[i].vpn == vpn && tlb[i].valid) {
            *success = true;
            return tlb[i].pfn;
        }
        i++;
    }
    *success = false;
    return -1;
}

// function to insert a new entry into the TLB
void Insert_in_tlb(TLBEntry tlb[], int *tlb_index, int tlb_size, int vpn, int pfn) {
    tlb[*tlb_index].vpn = vpn;
    tlb[*tlb_index].pfn = pfn;
    tlb[*tlb_index].valid = true;
    *tlb_index = (*tlb_index + 1) % tlb_size;  // for FIFO replacement policy
}

// function to check access permissions based on protection bits (as in the psedo code) 
int CanAccess(int protectBits) {
    return protectBits == 1;  
}

// function to perform memory management unit (MMU) address translation
int mmu(TLBEntry tlb[], int *tlb_index, int tlb_size, PageTableEntry pagetable[], int number_of_pages, int log_adrs) {
    int vpn = (log_adrs >> 12) & 0xFFFF;  
    int offset = log_adrs & 0xFFF;        

    bool success;
    int pfn = adrs_in_tlb(tlb, tlb_size, vpn, &success);  
    if (success) {  
        printf("TLB Hit! VPN: %x, PFN: %x\n", vpn, pfn);
        if (!CanAccess(pagetable[vpn].protectBits)) {
            printf("Protection Fault\n");
            exit(1);
        }
    } else { 
        printf("TLB Miss! VPN: %x\n", vpn);
        if (pagetable[vpn].valid) {
            pfn = pagetable[vpn].pfn;  
            if (CanAccess(pagetable[vpn].protectBits)) {
                Insert_in_tlb(tlb, tlb_index, tlb_size, vpn, pfn);  
            } else {
                printf("Protection Fault\n");
                exit(1);
            }
        } else {
            printf("Segmentation Fault\n");
            exit(1);
        }
    }

    int phyadrs = (pfn << 12) | offset;
    return phyadrs;
}

int main() {
    int page_size = 4096;            
    int memory_size = 65536;         
    int number_of_pages = memory_size / page_size;
    int tlb_size = 8;                
    int tlb_index = 0;       

    PageTableEntry pagetable[number_of_pages];
    bool assignedPFNs[number_of_pages];  
    int i = 0;
    
    while (i < number_of_pages) {
        assignedPFNs[i] = false;
        i++;
    }

    for (int i = 0; i < number_of_pages; i++) {
        int pfn;
        
        do {
            pfn = rand() % number_of_pages;  
        } while (assignedPFNs[pfn]);         
        assignedPFNs[pfn] = true;            

        pagetable[i].pfn = pfn;              
        pagetable[i].valid = true;
        pagetable[i].protectBits = 1;        
    }

    TLBEntry tlb[tlb_size];
    i = 0;
    while (i < tlb_size) {
        tlb[i].valid = false; 
        i++;
    }

    char log_adrs_hex[5]; 
    int log_adrs; 
    while (true) {
        // input logical address
        printf("Enter a 16-bit logical address in hex or type 'exit' to quit: ");
        scanf("%s", log_adrs_hex);

        // exit condition
        if (strcmp(log_adrs_hex, "exit") == 0) {
            printf("Exiting\n");
            break;
        }

       
        log_adrs = (int)strtol(log_adrs_hex, NULL, 16); 

        // address translation
        int phyadrs = mmu(tlb, &tlb_index, tlb_size, pagetable, number_of_pages, log_adrs); 
        printf("Physical Address: %x\n", phyadrs);
    }

    return 0;
}
