#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

void parse(char *code);
void ignoreComments(char *code);
bool variableCheck(char *str);
bool keywordCheck(char *str);
bool integerCheck(char *str);
bool integerVariableCheck(char *str);
void thatsAnInteger(char *word, int wordAmount);

char *variables[100];
int variableIndex = 0;
int integers[100] = {0};
int integerIndex = 0;

bool variableCheck(char *str)
{
    int len;
    len = strlen(str);
    if (len > 20)
    {

        return false;
    }

    int i = 0;
    for (i = 0; i < len; i++)
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
    char *line[1000];
    char *linesOfCode = strtok(code, ".");
    while (linesOfCode != NULL)
    {
        line[lineAmount++] = linesOfCode;
        linesOfCode = strtok(NULL, ".");
    }
    for (int i = 0; i < lineAmount; i++)
    {
        int wordAmount = 0;
        char *word[1000];
        char *linesOfCode = strtok(line[i], " \n\r[]\"");
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

        int operator;
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
        else
            operator= 0;

        switch (operator)
        {
        case 1: //int
            thatsAnInteger(word[1], wordAmount);
        }
    }
}

void thatsAnInteger(char *word, int wordAmount)
{
    FILE *fp;
    fp = fopen("anewliz.txt", "a");
    fprintf(fp, "int is a keyword\n");
    printf("weord: %s", word);
    if (variableCheck(word))
    {
        printf("%s is an integer variable\n", word);
        fprintf(fp, "%s is an integer variable\n", word);
        variables[variableIndex++] = word;
        integers[integerIndex++] = 0;
    }
    else if (keywordCheck(word))
    {
        printf("%s is not a variable but keyword.\n", word);
        return 0;
    }
    else if (word == NULL)
    {
        printf("Expected variable, found NULL\n");
        return 0;
    }
    else
    {
        printf("Expected variable, found %s\n", word);
        return 0;
    }

    if (wordAmount == 2)
    {
        fprintf(fp, "'.' end of line\n");
    }
    else
    {
        printf("End of line is expected\n");
        return 0;
    }
}

int main()
{
    /*FILE *filePointer;
    long lineSize;
    char *code;

    if ((filePointer = fopen("zort.txt", "r")) == NULL)
    {
        printf("Dosya acilamadi");
        exit(1);
    }
    else
    {
        fseek(filePointer, 0, SEEK_END);
        lineSize = ftell(filePointer);
        rewind(filePointer);
        code = calloc(1, lineSize + 1);
        while (code != EOF)
        {
            code = fgetc(filePointer);
        }
        parse(code);*/
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
