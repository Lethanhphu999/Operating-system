#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <time.h>
#include<string.h>

#define IN_FILE "in"
#define OUT_FILE "out"

int main()
{
    int in_fd, out_fd;
    char *in_data, *out_data;
    struct stat in_stat;
    clock_t start, end;

    in_fd = open(IN_FILE, O_RDONLY);
    if (in_fd == -1) {
        perror("Unable to open input file");
        return 1;
    }

    if (fstat(in_fd, &in_stat) == -1) {
        perror("Unable to get input file information");
        pclose(in_fd);
        return 1;
    }

    out_fd = open(OUT_FILE, O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (out_fd == -1) {
        perror("Unable to open output file");
        close(in_fd);
        return 1;
    }

    if (lseek(out_fd, in_stat.st_size - 1, SEEK_SET) == -1) {
        perror("Unable to set output file size");
        close(in_fd);
        close(out_fd);
        return 1;
    }

    if (write(out_fd, "", 1) != 1) {
        perror("Unable to write output file");
        close(in_fd);
        close(out_fd);
        return 1;
    }

    in_data = mmap(NULL, in_stat.st_size, PROT_READ, MAP_PRIVATE, in_fd, 0);
    if (in_data == MAP_FAILED) {
        perror("Unable to map input file to memory");
        close(in_fd);
        close(out_fd);
        return 1;
    }

    out_data = mmap(NULL, in_stat.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, out_fd, 0);
    if (out_data == MAP_FAILED) {
        perror("Unable to map output file to memory");
        munmap(in_data, in_stat.st_size);
        close(in_fd);
        close(out_fd);
        return 1;
    }

    start = clock();
    memcpy(out_data, in_data, in_stat.st_size);
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


    munmap(in_data, in_stat.st_size);
    munmap(out_data, in_stat.st_size);
    close(in_fd);
    close(out_fd);
    return 0;
}
