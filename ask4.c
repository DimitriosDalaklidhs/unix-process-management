#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM_CHILDREN 5

int main() {
    // Αρχικοποίηση pipe
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        exit(EXIT_FAILURE);
    }

    // Δημιουργία παιδιών
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t child_pid = fork();

        if (child_pid == -1) {
            perror("Fork failed");
            exit(EXIT_FAILURE);
        }

        if (child_pid == 0) { // Κώδικας για το παιδί
            // Κλείσιμο του άκρου ανάγνωσης του pipe
            close(pipe_fd[0]);

            // Εκτέλεση επαναληπτικών ενεργειών
            for (int j = 0; j < 5; j++) {
                sleep(1); // Περίμενε 1 δευτερόλεπτο

                // Εγγραφή στο pipe με το επιθυμητό μήνυμα
                dprintf(pipe_fd[1], "pid: %d message: pid: %d email: cs04507@uowm.gr\n", getppid(), getpid());
            }
            
            // Κλείσιμο του άκρου εγγραφής του pipe
            close(pipe_fd[1]);

            // Τερματισμός της διεργασίας παιδιού
            exit(EXIT_SUCCESS);
        }
    }

    // Κλείσιμο του άκρου εγγραφής του pipe στη γονική διεργασία
    close(pipe_fd[1]);

    // Διάβασμα από το pipe και εκτύπωση στη γονική διεργασία
    char buffer[256];
    ssize_t bytes_read;
    while ((bytes_read = read(pipe_fd[0], buffer, sizeof(buffer))) > 0) {
        // Εκτύπωση κάθε γραμμής που διαβάστηκε
        write(STDOUT_FILENO, buffer, bytes_read);
    }

    return 0;
}
