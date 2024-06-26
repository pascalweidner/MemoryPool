#include <stdio.h>
#include <windows.h>
#include <sys/types.h>

typedef struct header header;
struct header
{
    int size;
    int free;
    header *next;
    header *prev;
};

size_t META_SIZE = sizeof(header);

header *glob_head;

void initPool()
{
    void *mem = (void *)VirtualAlloc(NULL, (64 * 100), MEM_COMMIT, PAGE_READWRITE);
    if (mem == NULL)
    {
        printf("Error");
        exit(EXIT_FAILURE);
    }

    glob_head = (header *)mem;
    glob_head->size = 64 * 100 - META_SIZE;
    glob_head->free = 1;
    glob_head->next = NULL;
    glob_head->prev = NULL;
}

void *my_malloc(size_t size)
{
    header *curr = glob_head;
    while (curr != NULL)
    {
        if (curr->free == 1 && curr->size >= size)
        {
            break;
        }
        curr = curr->next;
    }
    header *next = (header *)((int)curr + size + META_SIZE);
    if (curr->next != next)
    {
        next->prev = curr;
        next->next = curr->next;
        next->free = 1;
        next->size = curr->size - size - META_SIZE;
    }

    if (curr->next != NULL)
    {
        curr->next->prev = next;
    }

    curr->size = size;
    curr->free = 0;
    curr->next = next;

    return (void *)((int)curr + META_SIZE);
}

void my_free(void *address)
{
    header *curr = (header *)((int)address - META_SIZE);
    curr->free = 1;
    if (curr->prev != NULL && curr->prev->free == 1)
    {
        curr->next->prev = curr->prev;
        curr->prev->next = curr->next;
        curr->prev->size = curr->size + curr->prev->size + META_SIZE;

        curr = curr->prev;
    }

    if (curr->next != NULL && curr->next->free == 1)
    {
        header *cache = curr->next->next;
        curr->size = curr->size + META_SIZE + curr->next->size;
        curr->next = cache;
        if (cache != NULL)
        {
            cache->prev = curr;
        }
    }
}

int main()
{
    initPool();
    int *p = (int *)my_malloc(5000);
    printf("mem_p=%p \n", (int)p);
    int *p2 = (int *)my_malloc(1000);

    VirtualFree((void *)glob_head, 64 * 100, MEM_DECOMMIT);

    return EXIT_SUCCESS;
}