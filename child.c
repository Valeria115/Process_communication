#include <stddef.h>
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
    unsigned char c2 = (unsigned char)s[1];
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

    int fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        printf("Cannot open file \"%s\": %s\n", filename, strerror(errno));
        exit(1);
    }

    size_t current_size = 1;
    if (ftruncate(fd, current_size) == -1) {
        printf("Ftruncate failed: %s\n", strerror(errno));
        close(fd);
        exit(1);
    }

    char *memory = mmap(NULL, current_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (memory == MAP_FAILED) {
        printf("mmap failed: %s\n", strerror(errno));
        close(fd);
        exit(1);
    }

    size_t offset = 0;
    char line[MAX_LEN];

    while (fgets(line, MAX_LEN, stdin)) {
        if (line[0] == "\n") break;

        if (!first_letter(line)) {
            printf("Error: string \'%s' does not start with a capital letter.\n", line);
            continue;
        }

        size_t len = strlen(line);
        size_t new_size = offset + len + 1;

        if (new_size > current_size) {
            if (munmap(memory, current_size) == -1) {
                printf("munmap failed: %s\n", strerror(errno));
                close(fd);
                exit(1);
            }

            if (ftruncate(fd, new_size) == -1) {
                printf("Fruncate failed: %s\n", strerror(errno));
                close(fd);
                exit(1);
            }

            memory = mmap(NULL, new_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
            if (memory == MAP_FAILED) {
                printf("mmap failed: %s\n", strerror(errno));
                close(fd);
                exit(1);
            }

            current_size = new_size;
        }

        memcpy(memory + offset, line, len);
        offset += len;

        memory[offset] = '\0';
    }

    munmap(memory, current_size);
    close(fd);

    return 0;
    
}
