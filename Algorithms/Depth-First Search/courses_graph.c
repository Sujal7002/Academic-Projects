//Sujal More 1001926735

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define FN_MAX 30
#define CN_MAX 30
#define FL_MAX 1000
#define FR_MAX 50


int findIndex(char cpyAr[], char startdata[][FL_MAX], int count);
void topologicalSort(char startdata[][FL_MAX], int count, int cpyoutputArr[][count], int res[]);
void dfsVisit(int count, int cpyoutputArr[][count], char *color, char *pre, int u, int *result);

int findIndex(char cpyAr[], char startdata[][FL_MAX], int count)
{
    for(int i = 0; i < count; ++i)
    {
        if(!(strcmp(startdata[i], cpyAr)))
        {
            return i;
        }
    }
}

void topologicalSort(char startdata[][FL_MAX], int count, int cpyoutputArr[][count], int res[])
{
    char color[count];
    char pre[count];
    int result[count];
    int i, j, k = 0;
    int u, v = 0;

    //W = white, G = gray, B = Black
    for(u = 0; u < count; ++u)
    {
        color[u] = 'W';
        pre[u] = '\0';
        result[u] = -1;
    }

    for(u = 0; u < count; ++u)
    {
        if(color[u] == 'W')
        {
            dfsVisit(count, cpyoutputArr, color, pre, u, result);
        }

    }

    for(i = 0, j = count - 1; i < count, j >= 0; ++i, --j)
    {
        printf("%d.\t-\t%s\t(corresponds to graph vertex %d)\n", i + 1, startdata[result[j ]], result[j]);
    }


}

void dfsVisit(int count, int cpyoutputArr[][count], char *color, char *pre, int u, int *result)
{
    int i, j, k = 0;
    
    color[u] = 'G';    
    
    for(i = 0; i < count; ++i)
    {
        if(cpyoutputArr[u][i] == 1)
        {
            if(color[i] == 'W')
            {
                pre[i] = u;
                dfsVisit(count, cpyoutputArr, color, pre, i, result);
            }
                
        }
    }
    color[u] = 'B';
    
    for(i = 0; i < count; ++i)
    {
        if(result[i] == -1)
        {
            result[i] = u;
            break;
        }
    }


}



int main(int argc, char *argv[])
{
    //Data Arrays
    char filename[FN_MAX];
    char courseName[CN_MAX];
    char firstC[30];
    char startdata[FR_MAX][FL_MAX];
    char allData[FL_MAX];

    //Counters
    int i, j, k = 0;

    //Pointers 
    FILE *input = NULL;
    
    printf("\n\nThis program will read, from a file, a list of courses and their \nprerequisites and will print the list in which to take courses. \n\n");
    printf("Enter filename: ");
    scanf("%s", filename);

    input = fopen(filename, "r");
    if(input == NULL)
    {
        printf("Failed to read from file. Program will terminate. \n");
        exit(-1);
    }

    i = 0;
    while(fscanf(input, "%s", firstC) != EOF)
    {
        
        strcpy(startdata[i], firstC);
        i++;

        char restOfLine[1000];
        fgets(restOfLine, sizeof(restOfLine), input);
    }
    int count = i;
    i = 0;

    printf("\nNumber of vertices in built graph: N = %d", count);
    printf("\nVertex - course name correspondence \n");
    for(j = 0; j < count; ++j)
    {
        printf("%d - %s", j, startdata[j]);
        printf("\n");
    }
    printf("\n\n");

    rewind(input);

    int outputArr[count + 1][count + 1];

    //Initialize array to all zeros
    for(j = 0; j < count + 1; ++j)
    {
        for(k = 0; k < count + 1; ++k)
        {
            outputArr[j][k] = 0;
        }
    }

    //Format array
    for(k = 0; k < count + 1; ++k)
    {
        if(k == 0)
        {
            outputArr[0][k] = 0;
        }
        else
        {
            outputArr[0][k] = k - 1;
        }
    }

    for(j = 1; j < count + 1; ++j)
    {
        outputArr[j][0] = j - 1;
    }

    int idx1, idx2 = 0;
    char cpyAr[CN_MAX];
    char cpyAr2[CN_MAX];

    while(fgets(allData, sizeof(allData), input))
    {
        char *newlineD = strchr(allData, '\n');
        if (newlineD != NULL)
        {
            // Replace the newline character with a null character
            *newlineD = '\0';
        }

        char *token = strtok(allData, " ");
        strcpy(cpyAr, token);

        char *newline = strchr(cpyAr, '\n');
        if (newline != NULL)
        {
            // Replace the newline character with a null character
            *newline = '\0';
        }

        idx1 = findIndex(cpyAr, startdata, count);

        while (token != NULL)
        {
            token = strtok(NULL, " ");
            if(token != NULL)
            {
                strcpy(cpyAr2, token);

                char *newline2 = strchr(cpyAr2, '\n');
                if (newline2 != NULL)
                {
                    // Replace the newline character with a null character
                    *newline2 = '\0';
                }

                idx2 = findIndex(cpyAr2, startdata, count);

                outputArr[idx2 + 1][idx1 + 1] = 1;
            }
        }
    }

    printf("Adjacency matrix: \n\n");

    for(j = 0; j < count + 1; ++j)
    {
        for(k = 0; k < count + 1; ++k)
        {
            if(k == 0)
            {
                printf("%6d|", outputArr[j][k]);
            }
            else
            {
                printf("%6d", outputArr[j][k]);
            }   
        }
        printf("\n");
        if(j == 0)
        {
            printf("-");
            for(int a = 0;  a < (count + 1) * 6; ++a)
            {
                printf("-");
            }        
        }
        printf("\n");
    }
     

    bool decision = false;

    for(j = 1; j < count + 1; ++j)
    {
        for(k = 1; k < count + 1; ++k)
        {
            if(j != 1 || k != 1)
            {
                if(outputArr[j][k] == 1 && outputArr[k][j] == 1)
                {
                    decision = true;
                }
            }
        }
    }
    
    if(decision)
    {
        printf("\nThere was at least one cycle. There is no possible ordering of the courses. \n");
        fclose(input);
        exit(-2);
    }

    printf("\nOrder in which to take courses: \n");

    //Make a copy output Arr without the formating to do topological sort
    int cpyoutputArr[count][count];
    for(j = 1; j < count + 1; ++j)
    {
        for(k = 1; k < count + 1; ++k)
        {
            cpyoutputArr[j - 1][k - 1] = outputArr[j][k];
        }
    }

    fclose(input);

    int res[count];
    topologicalSort(startdata, count, cpyoutputArr, res);

    return 0;
}