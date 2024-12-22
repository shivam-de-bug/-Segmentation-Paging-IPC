/*Perform parallel merge sort on a 16-element array. The 16-element array would be
  divided into two 8-element arrays, and each one would be passed to a child process.
  Similarly, the children would pass 4-element arrays to their children and so on. Each
  process will wait for its children and then merge the resulting arrays using a merge
  operation. Use pipes to pass data among processes.
  Input: Define a 16-element array in the code. The array must be unsorted.
  Output: Print the initial unsorted array and the final sorted array.*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    i = 0;
    while (i < n1) {
        L[i] = arr[l + i];
        i++;
    }
    j = 0;
    while (j < n2) {
        R[j] = arr[m + 1 + j];
        j++;
    }

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        int lpipe[2], rpipe[2];
        pipe(lpipe),pipe(rpipe);  

        pid_t lpid = fork();
        if (lpid == 0) {
            
            close(lpipe[0]); 
            mergeSort(arr, l, m); 
            write(lpipe[1], arr + l, (m - l + 1) * 4); 
            close(lpipe[1]);  
            exit(0);
        }

        pid_t rpid = fork();
        if (rpid == 0) {
            close(rpipe[0]);  
            mergeSort(arr, m + 1, r); 
            write(rpipe[1], arr + m + 1, (r - m) * 4);  
            close(rpipe[1]);  
            exit(0);
        }

       
        wait(NULL); 
        wait(NULL);
        
        close(lpipe[1]);  
        close(rpipe[1]); 
        
        read(lpipe[0], arr + l, (m - l + 1) * 4);  
        read(rpipe[0], arr + m + 1, (r - m) * 4);  
        
        close(lpipe[0]);
        close(rpipe[0]);
        
        merge(arr, l, m, r);
    }
}

int main() {
     int arr[16] = {12, 11, 13, 5, 6, 7, 19, 16, 15, 1, 9, 10, 2, 4, 8, 14};
    //int arr[16]={ 14,12,56,23,78,93,75,98,89,12,11,45,23,45,67,12};
    int i = 0;

    printf("Unsorted array: \n");
    while (i < 16) {
        printf("%d ", arr[i]);
        i++;
    }
    printf("\n");

    mergeSort(arr, 0, 15);
    printf("Sorted array: \n");
    i = 0;
    while (i < 16) {
        printf("%d ", arr[i]);
        i++;
    }
    printf("\n");

    return 0;
}
