#include <sys/types.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

int main(void)
{
    const char str1[] = "test";
    const char str2[] = "string 2";
    char *anon;

    // anon = (char*)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
    anon = (char *)VirtualAlloc(NULL, 4096, MEM_COMMIT, PAGE_READWRITE);

    if (anon == NULL)
    {
        printf("Error");
        return EXIT_FAILURE;
    }

    strcpy(anon, str1);

    printf("%s \n", anon);
    VirtualFree(anon, 4096, MEM_DECOMMIT);
    return EXIT_SUCCESS;
}