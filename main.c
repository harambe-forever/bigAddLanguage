#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void parse(char *code);
void ignoreComments(char *code);
bool seperator(char *str);
bool variableCheck(char *str);
bool keywordCheck(char *str);
bool integerCheck(char *str);
bool integerVariableCheck(char *str);
void thatsAnInteger(char *word, int wordAmount);
void thatsMove(char *word1, char *word2, char *word3, int wordAmount);
void thatsAdd(char *word1, char *word2, char *word3, int wordAmount);
void thatsSub(char *word1, char *word2, char *word3, int wordAmount);
void thatsOutput(char **ptr, int wordAmount);
void thatsALoop(char **word, int wordAmount);
void codeBlock(char **word, int wordAmount);

char *variables[512];
int variableIndex = 0;
int integers[512] = {0};
int integerIndex = 0;

bool seperator(char *str)
{
    int len = strlen(str);
    if (!strcmp(&str[len - 1], &","))
        return true;
    return false;
}

bool variableCheck(char *str)
{
    if (strlen(str) > 20)
    {

        return false;
    }

    int i = 0;
    for (i = 0; i < strlen(str); i++)
    {
        if (!((str[i] >= 'a' && str[i] <= 'z') ||
              (str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] == '_')))
            return false;
        if (str[0] == "_")
            return false;
    }
    return true;
}

bool integerCheck(char *str)
{
    if (strlen(str) > 100)
        return false;
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (!(isdigit(str[i]) || str[i] == "-"))
            return false;
        if (i > 0)
        {
            if (str[i] == "-" || strlen(str) == 1)
                return false;
        }
    }
    return true;
}

bool integerVariableCheck(char *str)
{
    for (int i = 0; i < variableIndex; i++)
    {
        if (!strcmp(str, variables[i]))
        {
            return true;
        }
    }

    return false;
}

bool keywordCheck(char *str)
{
    if (!strcmp(str, "int") || !strcmp(str, "move") ||
        !strcmp(str, "to") || !strcmp(str, "add") ||
        !strcmp(str, "sub") || !strcmp(str, "from") ||
        !strcmp(str, "loop") || !strcmp(str, "times") ||
        !strcmp(str, "out"))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void parse(char *code)
{
    int lineAmount = 0;
    char *line[512];
    char *linesOfCode = strtok(code, ".");
    while (linesOfCode != NULL)
    {
        line[lineAmount++] = linesOfCode;
        linesOfCode = strtok(NULL, ".");
    }
    for (int i = 0; i < lineAmount; i++)
    {
        int wordAmount = 0;
        char *word[512];
        char *linesOfCode = strtok(line[i], " \n\r");
        while (linesOfCode != NULL)
        {
            word[wordAmount++] = linesOfCode;
            linesOfCode = strtok(NULL, " \n\r");
        }

        if (wordAmount == 0)
        {
            printf("No code to execute");
            return;
        }

        int operator= 0;
        if (!strcmp(word[0], "int"))
            operator= 1;
        else if (!strcmp(word[0], "move"))
            operator= 2;
        else if (!strcmp(word[0], "add"))
            operator= 3;
        else if (!strcmp(word[0], "sub"))
            operator= 4;
        else if (!strcmp(word[0], "out"))
            operator= 5;
        else if (!strcmp(word[0], "loop"))
            operator= 6;
        else if (!strcmp(word[0], "["))
            operator= 7;
        else if (!strcmp(word[0], "]"))
            operator= 8;
        else
            operator= 0;

        switch (operator)
        {
        case 0:
            printf("Character Not Recognized.\n");
            break;
        case 1: //int
            thatsAnInteger(word[1], wordAmount);
            break;
        case 2: //move
            thatsMove(word[1], word[2], word[3], wordAmount);
            break;
        case 3: //add
            thatsAdd(word[1], word[2], word[3], wordAmount);
            break;
        case 4: //sub
            thatsSub(word[1], word[2], word[3], wordAmount);
            break;
        case 5: //out
            thatsOutput(word, wordAmount);
            break;
        case 6: //loop
            thatsALoop(word, wordAmount);
            break;
        case 7:
            codeBlock(word, wordAmount);
            break;
        case 8:
            endBlock();
            break;
        }
    }
}
void endBlock()
{
    FILE *fp;
    fp = fopen("anewliz.txt", "a");
    fprintf(fp, "codeBlock'tan ciktik.\n\n");
}
void codeBlock(char **word, int wordAmount)
{
    FILE *fp;
    fp = fopen("anewliz.txt", "a");
    fprintf(fp, "codeBlock'a girdik.\n\n");

    int k = 1;
    char newCode[100] = "";
    while (word[k] != NULL)
    {
        char *str = word[k];
        k++;
        if (!strcmp(str, "int"))
        {
            printf("WERE IN INTEGER. wordAmount:%d\n", wordAmount);
            thatsAnInteger(word[2], wordAmount - 1);
        }
        else if (!strcmp(str, "move"))
        {
            printf("WERE IN MOVE.\n");
            thatsMove(word[k + 1], word[k + 2], word[k + 3], wordAmount);
        }
        else if (!strcmp(str, "add"))
        {
            printf("WERE IN ADD.\n");
            thatsAdd(word[k + 1], word[k + 2], word[k + 3], wordAmount);
        }
        else if (!strcmp(str, "sub"))
        {
            printf("WERE IN SUB.\n");
            thatsSub(word[k + 1], word[k + 2], word[k + 3], wordAmount);
        }
        else if (!strcmp(str, "loop"))
        {
            printf("WERE IN LOOP.\n");
            thatsALoop(word, wordAmount);
        }
        else if (!strcmp(str, "out"))
        {
            printf("WERE IN OUT.\n");
            thatsOutput(word, wordAmount);
        }
        else
        {
            printf("");
        }
    }
}

void thatsALoop(char **word, int wordAmount)
{
    FILE *fp;
    fp = fopen("anewliz.txt", "a");
    fprintf(fp, "loop is a keyword.\n");

    int i = 0; //loop amount
    if (integerVariableCheck(word[1]))
    {
        fprintf(fp, "%s is a variable.\n", word[1]);
        int j;
        for (j = 0; j < variableIndex; j++)
        {
            if (!strcmp(variables[j], word[1]))
            {
                i = integers[j];
                break;
            }
        }
    }
    else if (integerCheck(word[1]))
    {
        fprintf(fp, "%s is an integer.\n", word[1]);
        i = atoi(word[1]);
    }
    else if (keywordCheck(word[1]))
    {
        printf("Expected integer or variable, found keyword.\n");
        return 0;
    }
    else if (word[1] == NULL)
    {
        printf("Expected integer or variable, found NULL.\n");
        return 0;
    }
    else
    {
        printf("Expected integer or variable, found %s.\n", word[1]);
        return 0;
    }

    if (!strcmp(word[2], "times"))
    {
        fprintf(fp, "times is a keyword.\n");
    }
    else if (word[2] == NULL)
    {
        printf("Expected keyword \"times\", found NULL.\n");
        return 0;
    }
    else
    {
        printf("Expected keyword \"times\", found %s.\n", word[2]);
        return 0;
    }

    if (wordAmount == 3)
    {
        fprintf(fp, "'.' is end of line.\n\n");
    }
    else
    {
        printf("End of line is expected.\n");
        return 0;
    }
}

void thatsOutput(char **ptr, int wordAmount)
{
    FILE *fp;
    fp = fopen("anewliz.txt", "a");
    fprintf(fp, "out is a keyword.\n");

    bool flag = true;
    int counter = 1;

    while (counter != wordAmount)
    {
        char *word = ptr[counter];
        if (integerVariableCheck(word))
        {
            fprintf(fp, "%s is a variable.\n", word);
        }
        else if (integerCheck(word))
        {
            fprintf(fp, "%s is an integer.\n", word);
        }
        else if (seperator(word))
        {
            word[strlen(word) - 1] = '\0';
            if (!strcmp(word, "newline"))
            {
                fprintf(fp, "newline is a keyword.\n");
                fprintf(fp, "Seperator\n");
            }
            else if (integerVariableCheck(word))
            {
                fprintf(fp, "%s is a variable.\n", word);
                fprintf(fp, "Seperator.\n");
            }
            else if (integerCheck(word))
            {
                fprintf(fp, "%s is an integer.\n", word);
                fprintf(fp, "Seperator\n");
            }
            else if (strncmp(word, "\"", 1) == 0 && flag == true)
            {
                int counter2 = 0;
                int len;
                bool flag2 = false;
                while (ptr[counter2] != NULL)
                {
                    char *word = ptr[counter2];
                    strcat(word, ",");
                    len = strlen(word);
                    if (strncmp(word, "\"", 1) == 0)
                    {
                        flag2 = true;
                    }
                    if (flag2)
                    {
                        if (strncmp(&word[len - 2], "\"", 1) == 0)
                        {
                            word[len - 1] = '\0';
                            fprintf(fp, "%s is a string constant.\nSeperator.\n", word);
                            flag2 = false;
                            break;
                        }
                        fprintf(fp, "%s ", word);
                    }
                    counter2++;
                }
                flag = false;
            }
            else
            {
                printf("Expected string, integer or variable, found %s.\n", word);
                continue;
            }
        }
        else if (!strcmp(word, "newline"))
        {
            fprintf(fp, "newline is a keyword.\n");
        }
        else if (strncmp(word, "\"", 1) == 0 && flag == true)
        {
            int counter2 = 0;
            int len;
            bool flag2 = false;
            while (ptr[counter2] != NULL)
            {
                char *word = ptr[counter2];
                len = strlen(word);
                if (strncmp(word, "\"", 1) == 0)
                {
                    flag2 = true;
                }
                if (flag2)
                {
                    if (strncmp(&word[len - 2], "\"", 1) == 0 || strncmp(&word[len - 1], "\"", "") == 0)
                    {
                        word[len - 1] = '\0';
                        fprintf(fp, "%s\" is a string constant.\nSeperator.\n", word);
                        flag2 = false;
                        break;
                    }
                    fprintf(fp, "%s ", word);
                }
                counter2++;
            }
            flag = false;
        }
        else if (word == NULL)
        {
            printf("Integer, variable, or string are excepted.\n");
            return 0;
        }
        else
        {
            //printf("There may be problem with the output. Please check project report (page:)\n");
        }
        counter++;
    }

    if (counter == wordAmount)
    {
        fprintf(fp, "'.' is end of line.\n\n");
    }
    else
    {
        printf("End of line is expected.\n");
    }
}

void thatsSub(char *word1, char *word2, char *word3, int wordAmount)
{
    FILE *fp;
    fp = fopen("anewliz.txt", "a");
    fprintf(fp, "sub is a keyword.\n");

    int i = 0; //to downcast a str int to int int
    if (integerVariableCheck(word1))
    {
        fprintf(fp, "%s is a variable.\n");
        int j;
        for (j = 0; j < variableIndex; j++)
        {
            if (strcmp(variables[j], word1))
            {
                i = integers[j];
                break;
            }
        }
    }
    else if (integerCheck(word1))
    {
        printf("%s is a variable.\n");
        i = atoi(word1); //downcasting
    }
    else if (keywordCheck(word1))
    {
        printf("Expected integer or variable, found keyword.\n");
        return 0;
    }
    else if (word1 == NULL)
    {
        printf("Expected integer or variable, found NULL.\n");
        return 0;
    }
    else
    {
        printf("Expected integer or variable, found %s.\n", word1);
        return 0;
    }

    if (!strcmp(word2, "from"))
    {
        fprintf(fp, "from is a keyword.\n");
    }
    else if (word2 == NULL)
    {
        printf("Expected keyword \"from\", found NULL.\n");
        return 0;
    }
    else
    {
        printf("Expected keyword \"from\", found %s", word2);
        return 0;
    }

    if (integerVariableCheck(word3))
    {
        fprintf(fp, "%s is a variable.\n");
        int k;
        for (k = 0; k < variableIndex; k++)
        {
            if (!strcmp(variables[k], word3))
            {
                integers[k] -= i;
                break;
            }
        }
    }
    else if (keywordCheck(word3))
    {
        printf("Expected variable, found keyword.\n");
        return 0;
    }
    else if (word3 == NULL)
    {
        printf("Expected variable, found NULL.\n");
        return 0;
    }
    else
    {
        printf("Expected variable, found %s.\n", word3);
        return 0;
    }

    if (wordAmount == 4)
    {
        fprintf(fp, "'.' is end of line.\n\n");
    }
    else
    {
        printf("End of line is expected.\n");
        return 0;
    }
}

void thatsAdd(char *word1, char *word2, char *word3, int wordAmount)
{
    FILE *fp;
    fp = fopen("anewliz.txt", "a");
    fprintf(fp, "add is a keyword.\n");

    int i = 0; //to downcast a str int to int int
    if (integerVariableCheck(word1))
    {
        fprintf(fp, "%s is a variable.\n", word1);
        int j;
        for (j = 0; j < variableIndex; j++)
        {
            if (!strcmp(variables[j], word1))
            {
                i = integers[j];
                break;
            }
        }
    }
    else if (integerCheck(word1))
    {
        fprintf(fp, "%s is an integer.\n", word1);
        i = atoi(word1); //downcasting
    }
    else if (keywordCheck(word1))
    {
        printf("Expected integer or variable, found keyword.\n");
        return 0;
    }
    else if (word1 == NULL)
    {
        printf("Expected integer or variable, found NULL.\n");
        return 0;
    }
    else
    {
        printf("Expected integer or variable, found %s.\n", word1);
        return 0;
    }

    if (!strcmp(word2, "to"))
    {
        fprintf(fp, "to is a keyword.\n");
    }
    else if (word2 == NULL)
    {
        printf("Expected keyword \"to\", found NULL.\n");
        return 0;
    }
    else
    {
        printf("Expected keyword \"to\", found %s.\n", word2);
        return 0;
    }

    if (integerVariableCheck(word3))
    {
        fprintf(fp, "%s is a variable.\n", word3);
        int k;
        for (k = 0; k < variableIndex; k++)
        {
            if (strcmp(variables[k], word3))
            {
                integers[k] += i;
                break;
            }
        }
    }
    else if (keywordCheck(word3))
    {
        printf("Expected variable, found keyword.\n");
        return 0;
    }
    else if (word3 == NULL)
    {
        printf("Expected variable, found NULL.\n");
        return 0;
    }
    else
    {
        printf("Expected variable, found %s.\n", word3);
        return 0;
    }
    if (wordAmount == 4)
    {
        fprintf(fp, "'.' is end of line.\n\n");
    }
    else
    {
        printf("End of line is expected.\n");
        return 0;
    }
}

void thatsMove(char *word1, char *word2, char *word3, int wordAmount)
{
    FILE *fp;
    fp = fopen("anewliz.txt", "a");
    fprintf(fp, "move is a keyword.\n");

    int i = 0; //to see if what's coming next is integer or variable
    if (integerCheck(word1))
    {
        fprintf(fp, "%s is an integer.\n", word1);
        i = atoi(word1);
    }
    else if (integerVariableCheck(word1))
    {
        fprintf(fp, "%s is a variable.\n", word1);
        int j;
        for (j = 0; j < variableIndex; j++)
        {
            if (!strcmp(variables[j], word1))
            {
                i = integers[j];
                break;
            }
        }
    }
    else if (keywordCheck(word1))
    {
        printf("%s Expected integer or variable, found keyword.\n");
        return 0;
    }
    else if (word1 == NULL)
    {
        printf("Expected integer or variable, found NULL.");
        return 0;
    }
    else
    {
        printf("Expected integer or variable, found %s.\n", word1);
    }

    if (!strcmp(word2, "to"))
    {
        fprintf(fp, "to is a keyword.\n");
    }
    else if (word2 == NULL)
    {
        printf("Expected keyword \"to\", found NULL.");
        return 0;
    }
    else
    {
        printf("Expected keyword \"to\", found %s.\n", word2);
        return 0;
    }

    if (integerVariableCheck(word3))
    {
        fprintf(fp, "%s is a variable.\n", word3);
        int k;
        for (k = 0; k < variableIndex; k++)
        {
            if (!strcmp(variables[k], word3))
            {
                integers[k] = i;
                break;
            }
        }
    }
    else if (keywordCheck(word3))
    {
        printf("Expected variable, found keyword.\n");
        return 0;
    }
    else if (word3 == NULL)
    {
        printf("Expected variable, found NULL.");
        return 0;
    }
    else
    {
        printf("Expected variable, found %s.\n", word3);
        return 0;
    }

    if (wordAmount == 4)
    {
        fprintf(fp, "'.' is end of line.\n\n");
    }
    else
    {
        printf("End of line is expected.\n");
        return 0;
    }
}

void thatsAnInteger(char *word, int wordAmount)
{
    FILE *fp;
    fp = fopen("anewliz.txt", "a");
    fprintf(fp, "int is a keyword.\n");
    if (variableCheck(word))
    {
        fprintf(fp, "%s is an integer variable.\n", word);
        variables[variableIndex++] = word;
        integers[integerIndex++] = 0;
    }
    else if (keywordCheck(word))
    {
        printf("%s Expected variable, found keyword.\n", word);
        return 0;
    }
    else if (word == NULL)
    {
        printf("Expected variable, found NULL.\n");
        return 0;
    }
    else
    {
        printf("Expected variable, found %s.\n", word);
        return 0;
    }

    if (wordAmount == 2)
    {
        fprintf(fp, "'.' is end of line.\n\n");
    }
    else
    {
        printf("End of line is expected.\n");
        return 0;
    }
}

int main()
{
    FILE *fp;
    fp = fopen("anewliz.txt", "w");
    fprintf(fp, "");
    FILE *filePointer;
    long lineSize;
    char *code;

    filePointer = fopen("zort.txt", "rb");
    if (!filePointer)
        perror("zort.txt"), exit(1);

    fseek(filePointer, 0L, SEEK_END);
    lineSize = ftell(filePointer);
    rewind(filePointer);

    code = calloc(1, lineSize + 1);
    if (!code)
        fclose(filePointer), fputs("memory alloc fails", stderr), exit(1);

    if (1 != fread(code, lineSize, 1, filePointer))
    {
        fclose(filePointer), free(code), fputs("entire read fails", stderr), exit(1);
    }

    parse(code);
}