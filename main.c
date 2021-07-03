#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

bool seperator(char *str);
bool variableCheck(char *str);
bool keywordCheck(char *str);
bool integerCheck(char *str);
bool integerVariableCheck(char *str);
void parse(char *code);
void ignoreComments(char *str);
void thatsAnInteger(char *word, int wordAmount, int line);
void thatsMove(char *word1, char *word2, char *word3, int wordAmount, int line);
void thatsAdd(char *word1, char *word2, char *word3, int wordAmount, int line);
void thatsSub(char *word1, char *word2, char *word3, int wordAmount, int line);
void thatsOutput(char **word, int wordAmount, int line);
void thatsALoop(char **word, int wordAmount, int line);
void codeBlock(char **word, int wordAmount, int line);
void endBlock(char **word, int wordAmount, int line);
int substring(char *source, int from, int n, char *target);

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
    if (!strncmp(str, "[", 1) || !strncmp(str, "]", 1))
    {
        return false;
    }
    if (strlen(str) > 100)
        return false;
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (i == 0)
        {
            if (atoi(str) < 0 && isdigit(str[1]) && strlen(str) > 1)
            {
                return true;
            }
        }
        if (i > 0)
        {
            if (str[i] == "-" || strlen(str) == 1 || !isdigit(str[1]))
            {
                return false;
            }
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
/*void loopParse(char *newCode)
{
}*/
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
            printf("Line: %d started with wrong character: %s\n", i + 1, word[0]);
            break;
        case 1: //int
            thatsAnInteger(word[1], wordAmount, i + 1);
            break;
        case 2: //move
            thatsMove(word[1], word[2], word[3], wordAmount, i + 1);
            break;
        case 3: //add
            thatsAdd(word[1], word[2], word[3], wordAmount, i + 1);
            break;
        case 4: //sub
            thatsSub(word[1], word[2], word[3], wordAmount, i + 1);
            break;
        case 5: //out
            thatsOutput(word, wordAmount, i + 1);
            break;
        case 6: //loop
            thatsALoop(word, wordAmount, i + 1);
            break;
        case 7:
            codeBlock(word, wordAmount, i + 1);
            break;
        case 8:
            endBlock(word, wordAmount, i + 1);
            break;
        }
    }
}
void endBlock(char **word, int wordAmount, int line)
{
    FILE *fp;
    fp = fopen("myscript.lx", "a");
    fprintf(fp, "Exit from codeBlock.\n");
    bool flag = false;
    int k = 1;

    while (word[k] != NULL)
    {
        char *str = word[k];
        k++;
        if (!strcmp(str, "int"))
        {
            //printf("WERE IN INTEGER.\n", wordAmount);
            thatsAnInteger(word[2], wordAmount - 1, line);
        }
        else if (!strcmp(str, "move"))
        {
            //printf("WERE IN MOVE.\n");
            thatsMove(word[k], word[k + 1], word[k + 2], wordAmount - 1, line);
        }
        else if (!strcmp(str, "add"))
        {
            //printf("WERE IN ADD.\n");
            thatsAdd(word[k], word[k + 1], word[k + 2], wordAmount - 1, line);
        }
        else if (!strcmp(str, "sub"))
        {
            //printf("WERE IN SUB.\n");
            thatsSub(word[k], word[k + 1], word[k + 2], wordAmount - 1, line);
        }
        else if (!strcmp(str, "loop"))
        {
            //printf("WERE IN LOOP.\n");
            thatsALoop(word, wordAmount - 1, line);
        }
        else if (!strcmp(str, "out"))
        {
            //printf("WERE IN OUT.\n");
            thatsOutput(word, wordAmount, line);
        }
        else if (!keywordCheck(word[1]))
        {
            if (flag)
                continue;
            printf("Line:%d. %s is not a keyword. Rest of the line can not be parsed.\n", line, word[1]);
            flag = true;
        }
        else
        {
            printf("");
        }
    }
}

void codeBlock(char **word, int wordAmount, int line)
{
    FILE *fp;
    fp = fopen("myscript.lx", "a");
    fprintf(fp, "Entering codeBlock.\n");

    bool flag = false;
    int k = 1;

    while (word[k] != NULL)
    {
        char *str = word[k];
        k++;
        if (!strcmp(str, "int"))
        {
            //printf("WERE IN INTEGER.\n", wordAmount);
            thatsAnInteger(word[2], wordAmount - 1, line);
        }
        else if (!strcmp(str, "move"))
        {
            //printf("WERE IN MOVE.\n");
            thatsMove(word[k], word[k + 1], word[k + 2], wordAmount - 1, line);
        }
        else if (!strcmp(str, "add"))
        {
            //printf("WERE IN ADD.\n");
            thatsAdd(word[k], word[k + 1], word[k + 2], wordAmount - 1, line);
        }
        else if (!strcmp(str, "sub"))
        {
            //printf("WERE IN SUB.\n");
            thatsSub(word[k], word[k + 1], word[k + 2], wordAmount - 1, line);
        }
        else if (!strcmp(str, "loop"))
        {
            //printf("WERE IN LOOP.\n");
            thatsALoop(word, wordAmount - 1, line);
        }
        else if (!strcmp(str, "out"))
        {
            //printf("WERE IN OUT.\n");
            thatsOutput(word, wordAmount, line);
        }
        else if (!keywordCheck(word[1]))
        {
            if (flag)
                continue;
            printf("Line:%d. %s is not a keyword. Rest of the line can not be parsed.\n", line, word[1]);
            printf("See report for more information.(page:)\n");
            flag = true;
        }
        else
        {
            printf("");
        }
    }
}

void thatsALoop(char **word, int wordAmount, int line)
{
    FILE *fp;
    fp = fopen("myscript.lx", "a");
    fprintf(fp, "loop is a keyword.\n");
    char *word1 = word[1];
    char *word2 = word[2];
    int loopAmount = 0; //loop amount
    if (integerVariableCheck(word1))
    {
        fprintf(fp, "%s is a variable.\n", word1);
        int j;
        for (j = 0; j < variableIndex; j++)
        {
            if (!strcmp(variables[j], word1))
            {
                loopAmount = integers[j];
                break;
            }
        }
    }
    else if (integerCheck(word1))
    {
        fprintf(fp, "%s is an integer.\n", word1);
        loopAmount = atoi(word1);
    }
    else if (keywordCheck(word1))
    {
        printf("Line:%d. Expected integer or variable, found keyword.\n", line);
        return 0;
    }
    else if (word1 == NULL)
    {
        printf("Line:%d. Expected integer or variable, found NULL.\n", line);
        return 0;
    }
    else
    {
        printf("Line:%d. Expected integer or variable, found %s.\n", line, word1);
        return 0;
    }

    if (!strcmp(word2, "times"))
    {
        fprintf(fp, "times is a keyword.\n");
    }
    else if (word2 == NULL)
    {
        printf("Line:%d. Expected keyword \"times\", found NULL.\n", line);
        return 0;
    }
    else
    {
        printf("Line:%d. Expected keyword \"times\", found %s.\n", line, word2);
        return 0;
    }
    //printf("loopAmount: %d\n", loopAmount);
    for (int i = 0; i < loopAmount; i++)
    {
        char newCode[100] = "";
        for (int i = 3; i < wordAmount; i++)
        {
            strcat(newCode, " ");
            strcat(newCode, word[i]);
        }
        strcat(newCode, ".");
        parse(newCode);
    }
    /*char *newCode[256];
    int index = 0;
    for (int i = 3; i < wordAmount; i++)
    {
        char *singleWord;
        int wordLen;
        char firstChar, lastChar;
        singleWord = word[i];
        //printf("singleWord: %s\n", singleWord);
        wordLen = strlen(singleWord);
        firstChar = singleWord[0];
        lastChar = singleWord[wordLen - 1];
        newCode[index] = singleWord;
        index++;
    }
    //strtok ile bir seyler yap
    //printf("index:%d\n", index);
    char *newCodeFW = newCode[0];
    int newWordAmount = strlen(newCode);
    for (int i = 0; i < index; i++)
    {
        printf("%s\n", newCode[i]);
    }
    if (!strncmp(newCodeFW, "out", 1))
    {
        for (int i = 0; i < loopAmount; i++)
        {
            thatsOutput(newCode, newWordAmount, line);
        }
    }*/
}
void thatsOutput(char **word, int wordAmount, int line)
{
    FILE *fp;
    fp = fopen("myscript.lx", "a");
    fprintf(fp, "out is a keyword.\n");
    bool strConstStart = false;
    bool strConstEnd = false;
    for (int i = 0; i < wordAmount; i++)
    {
        bool sep = false;
        char *singleWord;
        int wordLen;
        char firstChar, lastChar;
        singleWord = word[i];
        wordLen = strlen(singleWord);
        firstChar = singleWord[0];
        lastChar = singleWord[wordLen - 1];
        if (!strncmp(&lastChar, ",", 1) || !strncmp(&lastChar, ".", 1))
        {
            sep = true;
            singleWord[strlen(singleWord) - 1] = '\0';
        }

        if (!strncmp(&firstChar, "\"", 1))
        {
            strConstStart = true;
        }
        if (strConstStart == true && strConstEnd == false)
        {
            printf("%s ", singleWord);
            if (!strncmp(&singleWord[wordLen - 1], "\"", 1) || !strncmp(&singleWord[wordLen - 2], "\"", 1))
            {
                strConstEnd = true;
            }
        }
        if (singleWord == NULL)
        {
            printf("Integer, variable or string are expected.\n");
            return 0;
        }
        else if (!strncmp(singleWord, "newline", 1))
        {
            printf("\n");
        }
        else if (integerCheck(singleWord))
        {
            //printf("'%s' is an integer.\n", singleWord);
            printf("Integer Value: %s\n", singleWord);
        }
        else if (integerVariableCheck(singleWord))
        {
            //printf("'%s' is a variable.\n", singleWord);
            for (int i = 0; i < variableIndex; i++)
            {
                if (!strcmp(variables[i], singleWord))
                {
                    printf("'%s' variable value: %i\n", singleWord, integers[i]);
                    break;
                }
            }
        }
    }
    if (strConstStart == true && strConstEnd == false)
    {
        printf("String Constant Not Terminated.\n");
    }
}

void thatsSub(char *word1, char *word2, char *word3, int wordAmount, int line)
{
    FILE *fp;
    fp = fopen("myscript.lx", "a");
    fprintf(fp, "sub is a keyword.\n");

    int i = 0; //to downcast a str int to int int
    if (integerVariableCheck(word1))
    {
        fprintf(fp, "%s is a variable.\n", word1);
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
        fprintf(fp, "%s is an integer.\n", word1);
        i = atoi(word1); //downcasting
    }
    else if (keywordCheck(word1))
    {
        printf("Line:%d. Expected integer or variable, found keyword.\n", line);
        return 0;
    }
    else if (word1 == NULL)
    {
        printf("Line:%d. Expected integer or variable, found NULL.\n", line);
        return 0;
    }
    else
    {
        printf("Line:%d. Expected integer or variable, found %s.\n", line, word1);
        return 0;
    }

    if (!strcmp(word2, "from"))
    {
        fprintf(fp, "from is a keyword.\n");
    }
    else if (word2 == NULL)
    {
        printf("Line:%d. Expected keyword \"from\", found NULL.\n", line);
        return 0;
    }
    else
    {
        printf("Line:%d. Expected keyword \"from\", found %s", line, word2);
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
                integers[k] -= i;
                break;
            }
        }
    }
    else if (keywordCheck(word3))
    {
        printf("Line:%d. Expected variable, found keyword.\n", line);
        return 0;
    }
    else if (word3 == NULL)
    {
        printf("Line:%d. Expected variable, found NULL.\n", line);
        return 0;
    }
    else
    {
        printf("Line:%d. %s is not a declared variable.\n", line, word3);
        return 0;
    }

    if (wordAmount == 4)
    {
        fprintf(fp, "'.' is end of line.\n\n");
    }
    else
    {
        printf("Line:%d. End of line is expected.\n", line);
        return 0;
    }
}

void thatsAdd(char *word1, char *word2, char *word3, int wordAmount, int line)
{
    FILE *fp;
    fp = fopen("myscript.lx", "a");
    fprintf(fp, "add is a keyword.\n");

    int i = 0;
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
        printf("Line:%d. Expected integer or variable, found keyword.\n", line);
        return 0;
    }
    else if (word1 == NULL)
    {
        printf("Line:%d. Expected integer or variable, found NULL.\n", line);
        return 0;
    }
    else
    {
        printf("Line:%d. Expected integer or variable, found %s.\n", line, word1);
        return 0;
    }

    if (!strcmp(word2, "to"))
    {
        fprintf(fp, "to is a keyword.\n");
    }
    else if (word2 == NULL)
    {
        printf("Line:%d. Expected keyword \"to\", found NULL.\n", line);
        return 0;
    }
    else
    {
        printf("Line:%d. Expected keyword \"to\", found %s.\n", line, word2);
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
                integers[k] += i;
                break;
            }
        }
    }
    else if (keywordCheck(word3))
    {
        printf("Line:%d. Expected variable, found keyword.\n", line);
        return 0;
    }
    else if (word3 == NULL)
    {
        printf("Line:%d. Expected variable, found NULL.\n", line);
        return 0;
    }
    else
    {
        printf("Line:%d. %s is not a declared variable.\n", line, word3);
        return 0;
    }
    /*if (wordAmount == 4)
    {
        fprintf(fp, "'.' is end of line.\n\n");
    }
    else
    {
        printf("Line:%d. End of line is expected.\n", line);
        return 0;
    }*/
}

void thatsMove(char *word1, char *word2, char *word3, int wordAmount, int line)
{
    FILE *fp;
    fp = fopen("myscript.lx", "a");
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
        printf("Line:%d. %s Expected integer or variable, found keyword.\n", line, word1);
        return 0;
    }
    else if (word1 == NULL)
    {
        printf("Line:%d. Expected integer or variable, found NULL.", line);
        return 0;
    }
    else
    {
        printf("Line:%d. Integer not declared in a correct way: %s.\n", line, word1);
        return 0;
    }

    if (!strcmp(word2, "to"))
    {
        fprintf(fp, "to is a keyword.\n");
    }
    else if (word2 == NULL)
    {
        printf("Line:%d. Expected keyword \"to\", found NULL.", line);
        return 0;
    }
    else
    {
        printf("Line:%d. Expected keyword \"to\", found %s.\n", line, word2);
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
        printf("Line:%d. Expected variable, found keyword.\n", line);
        return 0;
    }
    else if (word3 == NULL)
    {
        printf("Line:%d. Expected variable, found NULL.", line);
        return 0;
    }
    else
    {
        printf("Line:%d. %s is not a declared variable.\n", line, word3);
        return 0;
    }

    if (wordAmount == 4)
    {
        fprintf(fp, "'.' is end of line.\n\n");
    }
    else
    {
        printf("Line:%d. End of line is expected.\n", line);
        return 0;
    }
}

void thatsAnInteger(char *word, int wordAmount, int line)
{
    FILE *fp;
    fp = fopen("myscript.lx", "a");
    //fprintf(fp, "int is a keyword.\n");
    if (variableCheck(word))
    {
        //fprintf(fp, "%s is an integer variable.\n", word);
        variables[variableIndex++] = word;
        integers[integerIndex++] = 0;
    }
    else if (keywordCheck(word))
    {
        printf("Line:%d. %s Expected variable, found keyword.\n", line, word);
        return 0;
    }
    else if (word == NULL)
    {
        printf("Line:%d. Expected variable, found NULL.\n", line);
        return 0;
    }
    else
    {
        printf("Line:%d. Expected variable, found a wrongly declared variable or variable with unrecognized character:%s.\n", line, word);
        return 0;
    }

    if (wordAmount == 2)
    {
        //fprintf(fp, "'.' is end of line.\n\n");
    }
    else
    {
        printf("Line:%d. End of line is expected.\n", line);
        return 0;
    }
}

char commentLine[100];
char a[100];
char b[100];
void ignoreComments(char *str)
{
    bool trueComment = false;
    int j;
    for (int i = 0; i < strlen(str); i++)
    {
        if (str[i] == '{')
        {
            for (j = i; j < strlen(str); j++)
            {
                if (str[j] == '}')
                {
                    substring(str, i + 1, j - i - 1, commentLine);
                    printf("Comment Line: '%s'\n", commentLine);

                    substring(str, 0, i, a);
                    substring(str, j + 1, strlen(str), b);
                    strcat(a, b);
                    strcpy(str, a);

                    trueComment = true;

                    ignoreComments(str);
                }
            }
            if (!trueComment)
            {
                printf("Comment Line is not completed.\n");
            }
        }
    }
}
int substring(char *source, int from, int n, char *target)
{
    int length, i;
    for (length = 0; source[length] != '\0'; length++)
        ;
    if (from > length)
    {
        printf("Starting index is invalid.\n");
        return 1;
    }
    if ((from + n) > length)
    {
        n = (length - from);
    }
    for (i = 0; i < n; i++)
    {
        target[i] = source[from + i];
    }
    target[i] = '\0';
    return 0;
}

int main()
{
    char *fileName;
    fileName = malloc(300 * sizeof(char));

    printf("Give a file name to open(without extension):\n");
    scanf("%s", fileName);
    strcat(fileName, ".ba");

    FILE *fp;
    fp = fopen("myscript.lx", "w");
    fprintf(fp, "");
    FILE *filePointer;
    long lineSize;
    char *code;

    filePointer = fopen(fileName, "rb");
    if (!filePointer)
        perror(fileName), exit(1);

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

    ignoreComments(code);
    parse(code);
    printf("\ndone.press enter to exit.\n");
    getchar();
    return 0;
}
