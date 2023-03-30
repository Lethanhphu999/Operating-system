#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#define _GNU_SOURCE


#define BUFSIZE 8192
#define INFILE "in"
#define OUTFILE "outfile"
#define SPLICE_F_MOVE 0x02

int main() {
    int infd, outfd;
    off_t offset = 0;
    ssize_t ret;
    clock_t start, end;

    infd = open(INFILE, O_RDONLY);
    if (infd == -1) {
        perror("open input file");
        return 1;
    }

    outfd = open(OUTFILE, O_WRONLY | O_CREAT | O_TRUNC, 0664);
    if (outfd == -1) {
        perror("open output file");
        close(infd);
        return 1;
    }

    char buf[BUFSIZE];
    start = clock();
    while ((ret = splice(infd, &offset, outfd, NULL, BUFSIZE, SPLICE_F_MOVE)) > 0) {}
    end = clock();

    if (ret == -1) {
        perror("splice");
        close(infd);
        close(outfd);
        return 1;
    }

    printf("Time consumed: %ld\n", end - start);
 //   FILE *result_file;
 //   result_file = fopen("result.txt", "a");
 //   if (result_file == NULL) {
  //      perror("Error opening result file");
  //      return 1;
  //  }

 //   fprintf(result_file, "%ld, ", end - start);
 //   fclose(result_file);
    close(infd);
    close(outfd);
    return 0;
}
