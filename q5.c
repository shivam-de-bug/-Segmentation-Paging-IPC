/*Write a C program to simulate a memory management system that demonstrates
  page faults and handles them using the clock algorithm for page replacement. Along
  with the use/reference bit, the dirty bit associated with each page should also be
  considered.
  Design
  ● Simulate a memory of N pages using a fixed-size page table.

  ● Input :
  ○ num_frames: Number of available page slots (frames).
  ○ page_requests: A string representing page requests (Eg. “0,1,1,2,4,2”)
  ● Output:
  ○ Total page faults
  ○ Total page hits
  ○ Hit rate
  ● Each page can be represented using a simple structure called struct page
  defined as follows
  struct page {
  uint8_t page_number;
  bool reference_bit;
  bool dirty_bit;
  };
  ● Memory frames can be represented using a simple array that holds the page
  number and any necessary bits.
  Algorithm :
  ● Initialize all memory frames to be empty (-1 or some other value).
  ● Service each of the page requests from the list passed as input to the program.
  Eg : [ 0 1 1 2 4 2 ]
  ○ For each page request, check if the page is in memory (check in the
  frames array).
  ○ If yes, it's a hit.
  ○ If no then use the clock algorithm to replace a page and update the
  reference bit of the corresponding page (struct page instance) if
  necessary.
  ○ After servicing all the requests, print out the total faults, total page hits and
  hit rate.*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

typedef struct page {
    int page_number;
    bool use_bit;
    bool dirtyBit;  //false->clean to evict first|true->dirty(expensive eviction)
}page;

int convertToIntegerArray(char *input, int *pages, int max_pages) {
    int count = 0;
    char *token = strtok(input, ","); //this gives first page
    while (token != NULL && count < max_pages) {
        pages[count++] = atoi(token); //convert numeric string to integer
        token = strtok(NULL, ",");
    }
    return count;
}

int checkEmptyFrame(page frames[],int num_frames,int *pointer){
    for(int i=0;i<num_frames;i++){
        if(frames[i].page_number==-1){
            return i;
            (*pointer)=(i+1)%num_frames;
        }
    }
    return -1;
}

bool checkIfPresentAndUpdate(int pageNumber,page frames[],int num_frames){
    for(int i=0;i<num_frames;i++){
        if(frames[i].page_number==pageNumber){  //hit!
            frames[i].use_bit=true;
            
            return true;
        }
    }
    return false;
}
//finds the victim frame in case of page fault
int clockReplacementAlgo(page frames[],int num_frames,int *pointer){
    int victimFrame=-1;
    //check for page with usebit and dirtyBit both 0.
    //even if there are no such pages this pass will traverse the main memory and utilise the second chance.
    for(int i=0;i<num_frames;i++){
        page x=frames[*pointer];
        if(x.use_bit==false&&x.dirtyBit==false){
            victimFrame=*pointer;
            *pointer=((*pointer) + 1)%num_frames;
            break;
        }
        //second chance if any is exhausted
        frames[*pointer].use_bit=false;
        *pointer=((*pointer)+1)%num_frames;
    }
    if(victimFrame!=-1){
        return victimFrame;
    }
    else{
        victimFrame=*pointer;
        *pointer=((*pointer)+1)%num_frames;
        return victimFrame;
    }
}

void calculatePageFaultsAndPageHits(page pages[],page frames[],int num_pages,int num_frames,int *pointer,int *pageFaults,int *hits){;
    
    for(int i=0;i<num_pages;i++){
        page currentPage=pages[i];
        int x=pages[i].page_number;
        int temp;
        //case of a page fault
        if(checkIfPresentAndUpdate(x,frames,num_frames)){
            (*hits)++;
        }
        else{
            (*pageFaults)++;
             int a=checkEmptyFrame(frames,num_frames,pointer);
            if(a!=-1){
                frames[a]=currentPage;
            }
            else{
                int victimFrame=clockReplacementAlgo(frames,num_frames,pointer);//find the index of the page to be replaced
                frames[victimFrame]=currentPage;
            }
        }
        for(int j=0;j<num_frames;j++){
            printf("\ni=%d page : %d use_bit:%d dirtyBit:%d pointer:%d hits:%d pageFaults:%d",i,frames[j].page_number,frames[j].use_bit,frames[j].dirtyBit,*pointer,*hits,*pageFaults);
        }
    }
}

int main()
{
    int num_frames;
    char page_requests[500];
    printf("Enter comma separated sequence of pages:");
    scanf("%[^\n]",page_requests);
    int pageInput[100];
    int num_pages;
    printf("Enter the number of available frames: ");
    scanf("%d", &num_frames);
    
    //initializing memory frames to be empty
    page frames[num_frames];
    for(int i=0;i<num_frames;i++){
        frames[i].page_number=-1;
        frames[i].use_bit=false;
        frames[i].dirtyBit=false;
    }
    
    num_pages = convertToIntegerArray(page_requests, pageInput, 100);
    
    if(num_pages == 0) {
        printf("No page requests provided.\n");
        return 1;
    }
    page pages[num_pages];
    srand(time(NULL));
    //initialize the array of pages
    for(int i=0;i<num_pages;i++){
        pages[i].page_number=pageInput[i];
        //initially no page is referenced
        pages[i].use_bit=false;  
        //initially 
        pages[i].dirtyBit=false;  
        pages[i].dirtyBit = rand() % 2;  // Randomly assigns 0 (false) or 1 (true)
    }
    int pointer=0;
    int pageFaults=0;
    int hits=0;
    calculatePageFaultsAndPageHits(pages,frames,num_pages,num_frames,&pointer,&pageFaults,&hits);
    double hitRate=(hits/(double)num_pages)*100.00;
    printf("\nNumber of page faults=%d",pageFaults);
    printf("\nNumber of page hits=%d",hits);
    printf("\nHit Rate=%f",hitRate);
    return 0;
}