#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/sendfile.h>

#define IN_FILE "in"
#define OUT_FILE "out1"

int main()
{
    int in_fd, out_fd;
    struct stat stat_buf;
    unsigned long int file_size;
    clock_t start, end;

    in_fd = open(IN_FILE, O_RDONLY, 00664);
    if (in_fd == -1) {
        puts("File doesn't exist or unknown error");
        return 1;
    }
    out_fd = open(OUT_FILE, O_WRONLY | O_CREAT, 00664);
    if (out_fd == -1) {
        puts("Unable to open/create the file");
        close(in_fd);
        return 1;
    }
    file_size = (fstat(in_fd, &stat_buf), stat_buf.st_size);

    start = clock();
    sendfile(out_fd, in_fd, 0, file_size);
    end = clock();

    printf("Time consumed: %ld\n", end - start);
    FILE *result_file;
    result_file = fopen("result.txt", "a");
    if (result_file == NULL) {
    perror("Error opening result file");
    return 1;
}

    fprintf(result_file, "%ld, ", end - start);
    fclose(result_file);
    close(out_fd);
    close(in_fd);
    return 0;
}
