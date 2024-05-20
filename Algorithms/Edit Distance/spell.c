#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "spell.h"

/*  Suggestions
- When you work with 2D arrays, be careful. Either manage the memory yourself, or
work with local 2D arrays. Note C99 allows parameters as array sizes as long as 
they are declared before the array in the parameter list. See: 
https://www.geeksforgeeks.org/pass-2d-array-parameter-c/

Worst case time complexity to compute the edit distance from T test words
 to D dictionary words where all words have length MAX_LEN:
Student answer:  Theta(T*D*MAX_LEN^2)
*/


/* Write helper functions here */
int min(int x, int y, int z) 
{
    if (x <= y && x <= z)
        return x;
    else if (y <= x && y <= z)
        return y;
    else
        return z;
}

void findminidx(int dist[], int searchCt, int minValue, int idx[])
{
    for (int i = 0; i < searchCt; i++) 
    {
        if (dist[i] == minValue) 
        {
            idx[i] = 1;
        }
    }
}

int *searchidx(int minValue, int dist[], int searchCt)
{
    int result[searchCt];
    int *ptr = result;
    int co = 0;
    for(int i = 0; i < searchCt; i ++)
    {
        if(dist[i] == minValue)
        {
            result[co] = i; 
            co++;
        }
    }
    return ptr;
}


/*
Parameters:
  - first_string - pointer to the first string (displayed vertical in the table)
  - second_string - pointer to the second string (displayed horizontal in the table)
  - print_table - If 1, the table with the calculations for the edit distance will be printed.
                  If 0 (or any value other than 1)it will not print the table
Return:  the value of the edit distance
*/
int edit_distance(char * first_string, char * second_string, int print_table)
{
    int row = strlen(first_string) + 2;
    int col = strlen(second_string) + 2;

    char oAr[row][col];

    for (int r = 0; r < row ; r++) //Initialize | and first str. in rows
    {
        if(r == 0 || r == 1)
        {
            oAr[r][0] = ' ';
        }
        else
        {
            oAr[r][0] = first_string[r - 2];
        }
    }
    
    for(int c = 0; c < col; c++) //Initialize | abd second str. in cols
    {
        if(c == 1 || c == 0)
        {
            oAr[0][c] = ' ';
        }
        else
        {
            oAr[0][c] = second_string[c - 2];
        }
    }
    
    for (int ar = 1; ar < row; ar++) //initialize 0,1,.... in rows
    {
        if(ar == 1)
        {
            oAr[ar][1] = '0';
        }
        else
        {
            oAr[ar][1] = '0' + ar - 1;
        }
    }
    
    for(int ac = 2; ac < col; ac++) //initialize 0,1,.... in cols
    {
        oAr[1][ac] = '0' + ac - 1;
    }

    for (int a = 2; a < row; a++) 
    {
        for (int b = 2; b < col; b++) 
        {
            if (first_string[a - 2] == second_string[b - 2]) 
            {
                oAr[a][b] = oAr[a - 1][b - 1];
            } 
            else 
            {
                oAr[a][b] = 1 + min(oAr[a][b - 1], oAr[a - 1][b], oAr[a - 1][b - 1]);
            }
        }
    }

    if(print_table == 1)
    {
        for (int i = 0; i < row; i++) 
        {
            for (int j = 0; j < col; j++) 
            {
                printf("%5c|", oAr[i][j]);
            }
            printf("\n");
            for(int k =0; k < col * 6; k++)
            {
                printf("-");
            }
            printf("\n");
        }
    }

    int value = oAr[row - 1][col - 1] - '0';
    int result = value;
    
    return result;
}

/*
Parameters:
  - testname - string containing the name of the file with the paragraph to spell check, includes file extenssion e.g. "text1.txt" 
  - dictname - name of file with dictionary words. Includes file extenssion, e.g. "dsmall.txt"
Behavior: If any of the files cannot be open displays a message and returns. (Does not exit the program.)
*/
void spell_check(char * testname, char * dictname)
{
    // Write your code here
    FILE *dic = fopen(dictname, "r");
    FILE *list = fopen(testname, "r");

    if(dic == NULL)
    {
        printf("Dictionary file cannot be opened returning...");
        return;
    }
    if(list == NULL)
    {
        printf("List file cannot be opened returning...");
        return;
    }
    
    char filesize[10];      //Dictionary file 
    fgets(filesize, sizeof(filesize), dic);
    int wordct = atoi(filesize);
    
    printf("\nLoaded dictionary %s has size %d\n\n", dictname, wordct);

    char **words = malloc(wordct * sizeof(char *));
    for(int i = 0; i < wordct; i++)
    {
        words[i] = malloc(wordct * sizeof(char *));
        fgets(words[i], sizeof(words[i]), dic);
        words[i][strcspn(words[i], "\n")] = '\0'; 
    }

    char listSize[10];      //List File
    fgets(listSize, sizeof(listSize), list);
    int listct = atoi(listSize);

    char **lWords = malloc(listct * sizeof(char *));
    for(int i = 0; i < listct; i++)
    {
        lWords[i] = malloc(listct * sizeof(char *));
        fgets(lWords[i], sizeof(lWords[i]), list);
        lWords[i][strcspn(lWords[i], "\n")] = '\0'; 
    }

    int counter = 0;
    int dist[wordct];
    int searchCt = 0;
    int idx[wordct];
    int minValue, choice;
    char inputStr[100]; 

    while(counter != listct)
    {
        printf("\n\n---> |%s|\n", lWords[counter]);

        char menu[] = {"-1  -  Type correction\n 0  -  Leave word as it is (do not fic the spelling)\n"};
        printf("%s", menu);

        
        for(int i = 0; i < wordct; i++)
        {
            dist[i]= edit_distance(lWords[counter], words[i], 0);
        }
      
        minValue = dist[0];
        searchCt = sizeof(dist) / sizeof(dist[0]);

        for(int i = 1; i < wordct; i++)
        {
            if(dist[i] < minValue)
            {
                minValue = dist[i];
            }
        }
        
        printf("        Minimum distance: %d\n", minValue);
        printf("        Words that give minimum distance: \n");
        
        int a = 1;
        int matchIdx[wordct];
        matchIdx[0] = '\0';
        char input[100]; 
        for(int i = 0; i < wordct; i++)
        {
            if(dist[i] == minValue)
            {
                printf("%8d  -  %s\n", a, words[i]);
                matchIdx[a] = i;
                a++;
            }    
        }

        printf("Enter your choice: ");
        scanf("%d", &choice);

        if(choice < -1 || choice >= a)
        {
            printf("Invalid choice original word will be kept\n");
            printf("The Corrected word is: %s", lWords[counter]);
        }
        else if(choice == -1)
        {
            printf("Enter correct word: ");
            scanf("%s", input);
        }
        else if(choice == 0)
        {
            printf("Leaving the word as it is\n");
            printf("The Corrected word is: %s", lWords[counter]);
        }
        else if(1 <= choice <= a)
        {
            printf("The Corrected word is: %s", words[matchIdx[choice]]);
        }

        counter++;
    }

    for (int i = 0; i < wordct; i++) {
        free(words[i]);
    }
   free(words);
    
    for (int i = 0; i < listct; i++) {
        free(lWords[i]);
    }
    free(lWords);

    fclose(dic);
    fclose(list);
}

