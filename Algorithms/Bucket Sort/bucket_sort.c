// Sujal More
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <limits.h>

#include "list.h"

// Opens a new file in read mode with the given file name
FILE *openFile(char *fileName)
{
	FILE *fp = NULL;

    fp = fopen(fileName, "r");
    if(fp == NULL)
    {
        printf("Could not open file. Exiting. \n");
        exit(0);
    }

    return fp;
}

// Reads a single integer from file.
int readSizeFromFile(FILE *fp)
{
    int arrSize = 0;
    fscanf(fp, "%d", &arrSize);
    
    return arrSize;
}

// Dynamically allocates an array of N integers, populates it from the file and returns it
int *readArrayFromFile(FILE *fp, int N)
{
    int *arr = (int *)malloc(N * sizeof(int));

    for (int i = 0; i < N; i++)
    {
        fscanf(fp, "%d", &arr[i]);
    }

    return arr;
}

// Finds the minimum element
int min_element(int *arr, int N)
{
    int min = 0;
    if(arr != NULL)
    {
        min =  arr[0];
        for(int i = 1; i < N; ++i)
        {
            if(arr[i] < min)
            {
                min = arr[i];
            }
        }
    }
    else
    {
        printf("The array is empty. Cannot find minimum. Exiting...\n");
        exit(-1);
    }
    
    return min;
}

// Finds the maximum element
int max_element(int *arr, int N)
{
    int max = 0;
    if(arr != NULL)
    {
        max =  arr[0];
        for(int i = 1; i < N; ++i)
        {
            if(arr[i] > max)
            {
                max = arr[i];
            }
        }
    }
    else
    {
        printf("The array is empty. Cannot find maximum. Exiting...\n");
        exit(-1);
    }

    return max;
}

int bucket_index(int element, int min, int max, int N)
{
    int idx = 0;

    if (min == max) 
    {
        printf("min == max. Couldn't calculate bucket_index. Exiting....\n");
        exit(-1);
    }
        
    // Calculate the index using the formula
    double numtr = ((double)element - (double)min) * N;     //Numerator calculation
    double dentr = (double)1 + (double)max - (double)min;   //Denominator calculation  
    double result = numtr / dentr;

    idx = (int)result;

    return idx;
}



nodePT *putIntoBuckets(int *arr, int N)
{
    // Find the minimum and maximum elements in the array using the functions implemented
    int min, max = 0;
    min = min_element(arr, N);
    max = max_element(arr, N);
    
    // Dynamically allocate an array of linked lists representing the buckets
    nodePT *buckets = (nodePT *)malloc(N * sizeof(nodePT));

    // Initialize each bucket to NULL
    for(int i = 0; i < N; ++i) 
    {
        buckets[i] = NULL;
    }

    // For each element in the array, find the bucket index using the function implemented
    // and insert the element into the corresponding bucket
    for(int i = 0; i < N; i++) 
    {
        int idx = bucket_index(arr[i], min, max, N);
        nodePT newNode = (nodePT)malloc(sizeof(struct node));
        newNode->data = arr[i];
        newNode->next = NULL;

        if (buckets[idx] == NULL) 
        {
            buckets[idx] = newNode;
        } 
        else 
        {
            nodePT curr = buckets[idx];
            nodePT prePtr = NULL;
            while (curr != NULL && curr->data < newNode->data) 
            {
                prePtr = curr;
                curr = curr->next;
            }
            if (prePtr == NULL) 
            {
                newNode->next = buckets[idx];
                buckets[idx] = newNode;
            }
            else 
            {
                newNode->next = curr;
                prePtr->next = newNode;
            }
                
        }
    }


    // return the buckets array
    return buckets;
}

int *run_bucket_sort(char *fileName)
{
    // Call openFile to open a new file.
    FILE *fp = NULL;
    fp = fopen(fileName, "r");
    if(fp == NULL)
    {
        printf("Cannot open file from function run_bucket_sort. Exiting....\n");
        exit(-1);
    }

    // Call readSizeFromFile to read the size of the array.
    int arrSize = readSizeFromFile(fp);

    // Call readArrayFromFile to read the array from the file.
    int *arr = readArrayFromFile(fp, arrSize);

    // Call putIntoBuckets to put the elements of the array into buckets
    nodePT *bucket = NULL;
    bucket = putIntoBuckets(arr, arrSize);

    // Rewrite the original array with the elements from the buckets to get the sorted array
    int *sortedArr = (int *)malloc(arrSize * sizeof(int));
    int i = 0;
    for(int j = 0; j < arrSize; ++j) 
    {
        if(bucket[j] != NULL) 
        {
            nodePT curr = bucket[j];
            while(curr != NULL) 
            {
                sortedArr[i++] = curr->data;
                curr = curr->next;
            }
        }
    }

    // Destroy the buckets and the array containing them
    for(int j = 0; j < arrSize; ++j) 
    {
        nodePT curr = bucket[j];
        while(curr != NULL) 
        {
            nodePT temp = curr;
            curr = curr->next;
            free(temp);
        }
    }
    free(bucket);
    free(arr);
    fclose(fp);

    // Return the sorted array
    return sortedArr;
}
