#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int count = 0;
    int seq = 0;
    if (argc != 2)
    {
        printf("USAGE: ./ccseq filename\n");
        return 1;
    }
    char *filename = argv[1];
    int fd = open(filename, O_RDONLY);

    if (fd == -1)
    { // cannot open the file
        perror(filename);
        return 2;
    }

    int size = 1024;
    char buffer[size];
    int rc;
    do
    {
        rc = read(fd, buffer, size);
        for (int i = 0; i < rc; i++)
        {
            char currentChar = buffer[i];
            for (int j = 7; j >= 0; j--)
            {
                int bit = (buffer[i] >> j) & 1;
                if (bit == 0)
                {
                    count++;
                    if (count > seq)
                    {
                        seq = count;
                    }
                }
                else
                {
                    count = 0;
                }
            }
        }
    } while (rc != 0);
    printf("%d\n", seq);
    close(fd);
    return 0;
}