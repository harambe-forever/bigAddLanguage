#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

int main()
{
    FILE *filePointer;
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
        int comp;
        while (code != EOF)
        {
            comp = 0;
            code = fgetc(filePointer);
            printf("%c", code);
            comp = strcmp(&code, ".");
            if (comp == 0)
            {
                printf("zuhauhah");
            }
        }
    }
}