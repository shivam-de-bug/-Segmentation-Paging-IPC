# Memory Management and Process Simulations

This repository contains C programs that simulate various memory management techniques and process-based sorting algorithms. These programs explore advanced concepts like segmentation, paging, hierarchical address translation, page replacement algorithms, and inter-process communication using pipes.

---

## Tasks

### Q1: Parallel Merge Sort

**Description:**  
This program performs a parallel merge sort on a 16-element array using child processes. The array is divided into smaller sub-arrays and passed to child processes for sorting. Pipes are used for inter-process communication to pass data between processes. Each process waits for its children to complete before merging the results.

**Features:**
- **Input:** A predefined 16-element unsorted array in the code.
- **Output:** The initial unsorted array and the final sorted array.

**Assumptions:**
- Each process operates only on its assigned sub-array.
- Pipes are used for communication between parent and child processes.
- The array is divided until it contains single elements, which are then merged.

---

### Q2: Segmentation-based Memory Management

**Description:**  
This program simulates memory management using segmentation. Logical addresses are translated to physical addresses using segment bases and limits. The system includes three segments: Code, Heap, and Stack. A segmentation fault is raised if a logical address exceeds a segment’s limit.

**Design:**
- **Memory Configuration:**
  - Code Segment: Base at 32KB, Size 2KB.
  - Heap Segment: Base at 34KB, Size 3KB.
  - Stack Segment: Base at 28KB (grows downward), Size 2KB.
- **Input:** 16-bit logical address in hexadecimal format.
- **Output:** Corresponding physical address or "Segmentation Fault".

**Assumptions:**
- Stack grows downward from its base address.
- The segmentation fault occurs if the offset exceeds the segment limit.
- The size and starting address of each segment are predefined and fixed.

---

### Q3: Paging with TLB

**Description:**  
This program demonstrates paging-based memory management using a Page Table and a Translation Lookaside Buffer (TLB). The TLB is used as a cache to speed up address translations. The program simulates TLB hits and misses, updating the TLB on misses.

**Design:**
- **Memory Configuration:**
  - Total Memory: 64KB.
  - Page Size: 4KB.
- **Process:**
  - TLB is checked for translations (hit/miss).
  - On a miss, the Page Table is accessed, and the TLB is updated.
- **Input:** 16-bit logical address in hexadecimal format.
- **Output:** Physical address and whether it was a TLB hit or miss.

**Assumptions:**
- The TLB can only store a limited number of entries (e.g., 4 or 8).
- Page Table is fully loaded into memory initially.
- Page numbers and frame numbers are mapped sequentially.

---

### Q4: Two-Level Page Table

**Description:**  
This program implements virtual-to-physical address translation using a two-level page table structure. The virtual address is divided into three parts: Page Directory Index, Page Table Index, and Offset. The program simulates memory access and handles page faults by updating the page directory and page table.

**Design:**
- **Virtual Address Configuration:**
  - Length: 32-bit.
  - Page Directory Index: 10 bits.
  - Page Table Index: 10 bits.
  - Offset: 12 bits.
- **Features:**
  - Tracks page hits and misses.
  - Simulates page faults and updates the page directory and tables.
  - Provides a summary of hit/miss ratios.
- **Input:** Load and store API with 32-bit virtual addresses.
- **Output:** Address translations, page faults, and hit/miss statistics.

**Assumptions:**
- Page Directory and Page Tables are dynamically allocated on page faults.
- Each Page Table has 1024 entries, and each entry is 4 bytes.
- Memory allocation is simulated and does not reflect actual physical memory.

---

### Q5: Clock Page Replacement Algorithm

**Description:**  
This program simulates a memory management system that handles page faults using the clock page replacement algorithm. It uses reference and dirty bits to optimize page replacements.

**Design:**
- **Memory Configuration:**
  - Number of frames specified by the user.
- **Features:**
  - Tracks page hits, page faults, and hit rates.
  - Implements the clock algorithm for page replacement.
- **Input:** 
  - Number of frames.
  - Page request sequence (e.g., `0,1,1,2,4,2`).
- **Output:**
  - Total page faults.
  - Total page hits.
  - Hit rate.

**Assumptions:**
- Pages are represented as structs containing reference and dirty bits.
- Frames are initially empty, represented by `-1` or another invalid value.
- The clock hand starts at the first frame and moves circularly.

---

## How to Run

### Prerequisites
- GCC compiler (Linux or Windows Subsystem for Linux recommended).

### Compilation
- Compile each program using GCC:
  ```bash
  gcc q1.c -o q1
  gcc q2.c -o q2
  gcc q3.c -o q3
  gcc q4.c -o q4
  gcc q5.c -o q5
