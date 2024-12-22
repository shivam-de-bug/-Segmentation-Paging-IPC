/*Write a program that simulates an operating system's memory management using
  segmentation. Your task is to simulate the process of translating logical addresses to
  physical addresses using the concept of segmentation. Additionally, you will simulate
  conditions where a segmentation fault occurs. Add your assumptions, if any, to a
  ReadMe file.
  Design:
  ● The system has three segments: Code, Heap, and Stack.
  ● Each segment has a base and a limit (bounds), which define where the segment
  starts in memory and its length.
  ● Assume the system has a 64KB physical memory, and memory is reserved as
  follows:
  ○ Code Segment: Starts at 32KB, size 2KB.
  ○ Heap Segment: Starts at 34KB, size 3KB.
  ○ Stack Segment: Starts at 28KB and grows downwards, size 2KB.

  ● A segmentation fault should be raised if the offset exceeds the segment's limit.
  ● Use the pseudocode from Chapter 16 for the address translation.
  Input: The user can enter a 16-bit logical address in hex format. For example: a56f
  Output: 16-bit physical address in hex format or segmentation fault.*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct segment{
    int base;
    int bound;
}segment;
//Code Segment: Starts at 32KB, size 2KB.
segment code={0x8000,0x0800};
//Heap Segment: Starts at 34KB, size 3KB
segment heap={0x8800,0x0C00};
//Stack Segment: Starts at 28KB and grows downwards, size 2KB.
segment stack={0x7000,0x0800};

void getPA(int VA,segment segments[]){
    int PA;
    int maxSegmentSize=0x4000;      //2^14=16kb(since max 14 bits are available for offset)
    int segment=(VA&0xC000)>>14;    //to get the top two bits for segment identification
    int offset=VA&0x3FFF;           //to get the lower 14 bits to get offset
    //segments are defined as 0 for code,1 for heap,and 2 for stack
    if(segment==3){   
        printf("SEGMENT DOES NOT EXIST!");
        return;
    }
    if(segment==2){
        offset-=maxSegmentSize;
    }
    if(abs(offset)>=segments[segment].bound){
        printf("SEGMENTATION FAULT!");
        return;
    }
    PA=offset+segments[segment].base;
    printf("\nPhysical Address:%x",PA);
}

int main()
{
    segment segments[3]={code,heap,stack};
    int VA;
    printf("Enter Virtual Address:");
    scanf("%x",&VA);
    getPA(VA,segments);
    return 0;
}