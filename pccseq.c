#include <stdio.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("USAGE: ./pccseq filename ...\n");
        return 1;
    }
   int *longest_sequence = mmap(NULL, 4096, PROT_READ | PROT_WRITE,
                          MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *longest_sequence = 0;
    for (int i = 1; i < argc; i++) {
        char *current_arg = argv[i];
        if (access(current_arg, F_OK) != 0) {
            perror(current_arg);
            exit(2);
        }
        pid_t rc = fork();
        if (rc == 0) {
            int unset_count = 0;
            int longest_zero = 0;
            int fd = open(current_arg, O_RDONLY, S_IRUSR | S_IWUSR);
            struct stat sb;
            if (fstat(fd, &sb) == -1) {
                perror(current_arg);
                exit(2);
            }
            char *mapped_file = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
            if (mapped_file == MAP_FAILED) {
                perror(current_arg);
                exit(2);
            }
            for (int j = 0; j < sb.st_size; j++) {
                for (int m = 7; m >= 0; m--) // 00000010
                {
                    int bit = (mapped_file[j] >> m) & 1;
                    if (bit == 0)
                    {
                        unset_count++;
                        if (unset_count > longest_zero)
                        {
                            longest_zero = unset_count;
                        }
                    }
                    else
                    {
                        unset_count = 0;
                    }
                }
            }
            if (longest_zero > *longest_sequence) {
                *longest_sequence = longest_zero;
            }
            munmap(mapped_file, sb.st_size);
            close(fd);
            exit(0);
        }
    }
    while (wait(NULL) != -1) {}
    printf("%d\n", *longest_sequence);
    return 0;
}