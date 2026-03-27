#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

/*
 * This program demonstrates inter-process communication using a pipe.
 *
 * - The parent process creates a pipe and forks NUM_CHILDREN children.
 * - Each child writes MESSAGES_PER_CHILD messages into the pipe.
 * - The parent reads from the pipe and prints all messages to stdout.
 *
 * Key concepts:
 * - fork() creates child processes
 * - pipe() provides unidirectional IPC
 * - Proper closing of unused pipe ends is essential
 */

#define NUM_CHILDREN 5
#define MESSAGES_PER_CHILD 5

int main(void) {
    int pipe_fd[2];

    // Create pipe (pipe_fd[0] = read end, pipe_fd[1] = write end)
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
            // IMPORTANT: If not closed, the pipe may never signal EOF to the parent
            if (close(pipe_fd[0]) == -1) {
                perror("close read end in child");
                _exit(EXIT_FAILURE);
            }

            for (int j = 0; j < MESSAGES_PER_CHILD; j++) {
                sleep(1); // Delay to simulate work and stagger output

                // Multiple children write to the same pipe.
                // Messages may interleave, but small writes (like this) are typically atomic.
                if (dprintf(pipe_fd[1],
                            "pid: %d message: pid: %d email: cs04507@uowm.gr\n",
                            getppid(), getpid()) < 0) {
                    perror("dprintf");
                    close(pipe_fd[1]);
                    _exit(EXIT_FAILURE);
                }
            }

            // Close write end in child when done
            // This helps signal EOF to the parent when all children finish
            if (close(pipe_fd[1]) == -1) {
                perror("close write end in child");
                _exit(EXIT_FAILURE);
            }

            _exit(EXIT_SUCCESS);
        }

        // Only the parent reaches here and continues creating more children.
        // Each child exits inside the if (child_pid == 0) block.
    }

    // Parent process:

    // Close unused write end
    // This allows the parent to detect EOF when all children finish writing
    if (close(pipe_fd[1]) == -1) {
        perror("close write end in parent");
        close(pipe_fd[0]);
        exit(EXIT_FAILURE);
    }

    // Read from pipe and print to stdout
    // Read from pipe until EOF (all write ends are closed)
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

    // Wait for all children to terminate to prevent zombie processes
    // (children that have exited but still occupy a process table entry)
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (wait(NULL) == -1) {
            perror("wait");
            // Continue attempting to wait for remaining children
        }
    }

    return 0;
}
