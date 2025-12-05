#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <locale.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define MAX_LEN 256


int first_letter(const char *s) {
    if (!s || s[0] == '\0' || s[0] == '\n') {
        return 0;
    }

    unsigned char c1 = (unsigned char)s[0];
    unsigned char c2 = (unsigned char)s[1]; //русские буквы кодируются двумя байтами

    if (isupper(c1)) {
        return 1;
    }

    if (c1 == 0xD0 && c2 >= 0x90 && c2 <= 0xAF) {
        return 1;
    }

    return 0;
}

int main() {

    setlocale(LC_ALL, "C.UTF-8");

    char filename[MAX_LEN];

    if (fgets(filename, MAX_LEN, stdin) == NULL) {
        fprintf(stderr, "Couldn't get the file name.\n");
        exit(1);
    }
    filename[strcspn(filename, "\n")] = '\0';

    char *buffer = NULL;
    size_t total = 0;
    char line[MAX_LEN];

    while (fgets(line, MAX_LEN, stdin)) {
        if (line[0] == '\n') break;

        if (first_letter(line)) {
            size_t len = strlen(line);

            char *tmp = realloc(buffer, total + len + 1);
            if (!tmp) {
                fprintf(stderr, "Memory allocation failed\n");
                free(buffer);
                exit(1);
            }
            buffer = tmp;
            
            memcpy(buffer + total, line, len);
            total += len;
            buffer[total] = '\0';
            
        } else {
            printf("Error: string \"%s\" does not start with a capital letter.\n", line);
        }
    }

    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Cannot open file \"%s\": %s\n", filename, strerror(errno));
        free(buffer);
        exit(1);
    }

    if (ftruncate(fd, total + 1) == -1) {
        printf("Ftruncate failed: %s\n", strerror(errno));
        close(fd);
        free(buffer);
        exit(1);
    }

    char *memory = mmap(NULL, total + 1, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    if (memory == MAP_FAILED) {
        printf("mmap failed: %s\n", strerror(errno));
        close(fd);
        free(buffer);
        exit(1);
    }

    if (buffer) {
        memcpy(memory, buffer, total);
    }
    memory[total] = '\0';

    munmap(memory, total + 1);
    close(fd);
    free(buffer);
    
    return 0;
}
