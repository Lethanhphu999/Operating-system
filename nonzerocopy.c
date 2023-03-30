#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define IN_FILE "in"
#define OUT_FILE "out"

int main()
{
    FILE *in, *out;
    char *c;
    unsigned long int file_size;
    clock_t start, end;

    in = fopen(IN_FILE, "r");
    if (!in) {
        puts("File doesn't exist or unknown error");
        return 1;
    }
    out = fopen(OUT_FILE, "w");
    if (!out) {
        puts("Unable to open/create the file");
        fclose(in);
        return 1;
    }
    fseek(in, 0, SEEK_END);
    file_size = ftell(in);
    rewind(in);
    c = malloc(file_size);
    if (!c) {
        puts("Unable to allocate memory");
        fclose(out);
        fclose(in);
        return 1;
    }

    start = clock();
    fread(c, 1, file_size, in);
    fwrite(c, 1, file_size, out);
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
    free(c);
    fclose(out);
    fclose(in);
    return 0;
}
