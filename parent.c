#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>

#define MAX_LEN 256

int main() {
    int pipe1[2], pipe2[2];
    pid_t pid;

    if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
        perror("pipe failed");
        exit(1);
    }

    pid = fork();
    if (pid < 0) {
        perror("fork failed");
        exit(1);
    }

    if (pid == 0) {

        dup2(pipe1[0], STDIN_FILENO);
        dup2(pipe2[1], STDOUT_FILENO);

        close(pipe1[1]);
        close(pipe2[0]);
        close(pipe1[0]);
        close(pipe2[1]);

        execl("./child", "child", NULL);
        perror("exec failed");
        
        exit(1);
    } else {

        close(pipe1[0]);
        close(pipe2[1]);

        char filename[MAX_LEN];
        printf("Enter name file: ");
        if (!fgets(filename, MAX_LEN, stdin)) {
            fprintf(stderr, "Input error\n");
            exit(1);
        }
        
        filename[strcspn(filename, "\n")] = '\0';

        write(pipe1[1], filename, strlen(filename));
        write(pipe1[1], "\n", 1);

        printf("Enter string or empty string for exit:\n");
        
        char line[MAX_LEN];
        while (1) {
            printf("> ");
            if (!fgets(line, MAX_LEN, stdin)) {
                break;
            }

            if (strcmp(line, "\n") == 0) {
                write(pipe1[1], "\n", 1);
                break;
            }

            write(pipe1[1], line, strlen(line));
        }
        close(pipe1[1]);

        char buffer[MAX_LEN];
        ssize_t n;

        printf("\nErrors:\n");
        while ((n = read(pipe2[0], buffer, MAX_LEN - 1)) > 0) {
            buffer[n] = '\0';
            printf("%s", buffer);
        }
        close(pipe2[0]);

        waitpid(pid, NULL, 0);

    }

    return 0;
}
