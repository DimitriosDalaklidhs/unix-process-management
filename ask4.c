#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 5
#define MESSAGES_PER_CHILD 5

int main(void) {
    int pipe_fd[2];

    // Create pipe
    if (pipe(pipe_fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork children
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("fork");
            // Close pipe before exiting due to error
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) { // Child process
            // Close unused read end in child
            if (close(pipe_fd[0]) == -1) {
                perror("close read end in child");
                _exit(EXIT_FAILURE);
            }

            for (int j = 0; j < MESSAGES_PER_CHILD; j++) {
                sleep(1); // Wait 1 second

                // Write message to pipe
                if (dprintf(pipe_fd[1],
                            "pid: %d message: pid: %d email: cs04507@uowm.gr\n",
                            getppid(), getpid()) < 0) {
                    perror("dprintf");
                    // Even on error, try to close and exit
                    close(pipe_fd[1]);
                    _exit(EXIT_FAILURE);
                }
            }

            // Close write end in child when done
            if (close(pipe_fd[1]) == -1) {
                perror("close write end in child");
                _exit(EXIT_FAILURE);
            }

            _exit(EXIT_SUCCESS);
        }
        // Parent continues the loop and forks the next child
    }

    // Parent process:
    // Close unused write end
    if (close(pipe_fd[1]) == -1) {
        perror("close write end in parent");
        close(pipe_fd[0]);
        exit(EXIT_FAILURE);
    }

    // Read from pipe and print
    char buffer[256];
    ssize_t bytes_read;

    while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
        if (write(STDOUT_FILENO, buffer, bytes_read) == -1) {
            perror("write to stdout");
            close(pipe_fd[0]);
            exit(EXIT_FAILURE);
        }
    }

    if (bytes_read == -1) {
        perror("read");
        close(pipe_fd[0]);
        exit(EXIT_FAILURE);
    }

    // Close read end when finished
    if (close(pipe_fd[0]) == -1) {
        perror("close read end in parent");
        exit(EXIT_FAILURE);
    }

    // Wait for all children to avoid zombies
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (wait(NULL) == -1) {
            perror("wait");
            // Still continue trying to wait for others
        }
    }

    return 0;
}

