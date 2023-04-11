#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include "bigbag.h"
#include <errno.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>

struct bigbag_entry_s *entry_addr(void *hdr, uint32_t offset)
{
    if (offset == 0)
        return NULL;
    return (struct bigbag_entry_s *)((char *)hdr + offset);
}

uint32_t entry_offset(void *hdr, void *entry)
{
    return (uint32_t)((uint64_t)entry - (uint64_t)hdr);
}

int main(int argc, char *argv[])
{
    void *file_base;
    if (argc < 2)
    {
        printf("USAGE: ./bigbag [-t] filename\n");
        return 1;
    }
    int fileExist = 1;

    int fd = open(argv[1], O_RDONLY, S_IWUSR | S_IRUSR);
    if (fd == -1)
    {
        if (errno != EEXIST) // file does not  exist
        {
            fileExist = 0;
            fd = open(argv[1], O_RDONLY | O_CREAT, S_IWUSR | S_IRUSR);
            ftruncate(fd, BIGBAG_SIZE);
        }
    }

    file_base = mmap(0, BIGBAG_SIZE, PROT_READ, MAP_SHARED, fd, 0);
    if (file_base == MAP_FAILED)
    {
        perror("mmap");
        exit(3);
    }

    struct bigbag_hdr_s *hdr = file_base;
    if (fileExist != 1)
    {
        hdr->magic = be32toh(BIGBAG_MAGIC);
        hdr->first_element = 0;
        hdr->first_free = 12;
    }

    struct stat stat;
    fstat(fd, &stat);
    printf("size = %ld\n", stat.st_size);
    printf("magic = %08x\n", be32toh(hdr->magic));
    printf("first_free = %d\n", hdr->first_free);
    printf("first_element = %d\n", hdr->first_element);
    struct bigbag_entry_s *entry;
    int offset = sizeof(*hdr);
    // https://linuxhint.com/using_mmap_function_linux

    size_t size = 32;
    size_t charac;
    char *input;
    input = (char *)malloc(size * sizeof(char));
    charac = getline(&input, &size, stdin);
    while (charac != -1)
    {
       
        if (input[0] == 'a')
        {
        }
        else if (input[0] == 'd')
        {
        }
        else if (input[0] == 'l')
        {
        }
        else
        {
            printf("%c not used correctly\npossible commands:\na string_to_add\nd string_to_delete\nc string_to_check\nl", input[0]);
        }

        charac = getline(&input, &size, stdin);
    }
}
