[1mdiff --git a/ask4.c b/ask4.c[m
[1mindex 2ece430..2e687b3 100644[m
[1m--- a/ask4.c[m
[1m+++ b/ask4.c[m
[36m@@ -3,71 +3,75 @@[m
 #include <unistd.h>[m
 #include <sys/types.h>[m
 #include <sys/wait.h>[m
[31m-#include <string.h>[m
 [m
 #define NUM_CHILDREN 5[m
 [m
[31m-int main()[m
[31m-{[m
[31m-    int pd[2];[m
[32m+[m[32mint main() {[m
[32m+[m[32m    // Αρχικοποίηση πίνακα για τα PIDs των παιδιών[m
[32m+[m[32m    pid_t child_pids[NUM_CHILDREN];[m
 [m
[31m-    if (pipe(pd) == -1)[m
[31m-    {[m
[31m-        perror("pipe");[m
[32m+[m[32m    // Δημιουργία του pipe[m
[32m+[m[32m    int pipe_fd[2];[m
[32m+[m[32m    if (pipe(pipe_fd) == -1) {[m
[32m+[m[32m        perror("Pipe creation failed");[m
         exit(EXIT_FAILURE);[m
     }[m
 [m
[31m-    for (int i = 0; i < NUM_CHILDREN; i++)[m
[31m-    {[m
[32m+[m[32m    // Δημιουργία παιδιών[m
[32m+[m[32m    for (int i = 0; i < NUM_CHILDREN; ++i) {[m
         pid_t pid = fork();[m
 [m
[31m-        if (pid == -1)[m
[31m-        {[m
[31m-            perror("fork");[m
[32m+[m[32m        if (pid == -1) {[m
[32m+[m[32m            perror("Fork failed");[m
             exit(EXIT_FAILURE);[m
[31m-        }[m
[31m-        else if (pid == 0) // Child process[m
[31m-        {[m
[31m-            // Child closes read end of the pipe[m
[31m-            close(pd[0]);[m
[31m-[m
[31m-            // Child code[m
[31m-            for (int j = 0; j < 5; j++)[m
[31m-            {[m
[32m+[m[32m        } else if (pid == 0) {  // Κώδικας για το παιδί[m
[32m+[m[32m            // Κλείσιμο του άκρου ανάγνωσης του pipe[m
[32m+[m[32m            close(pipe_fd[0]);[m
[32m+[m
[32m+[m[32m            // Εκτέλεση εργασιών παιδιού[m
[32m+[m[32m            for (int j = 0; j < 5; ++j) {[m
                 sleep(1);[m
[31m-                dprintf(pd[1], "pid: %d email: cs04507@uowm.gr\n", getpid());[m
[32m+[m
[32m+[m[32m                // Γράψιμο στο pipe[m
[32m+[m[32m                dprintf(pipe_fd[1], "pid: %d email: YOUR_EMAIL\n", getpid());[m
             }[m
 [m
[31m-            // Child closes write end of the pipe and exits[m
[31m-            close(pd[1]);[m
[32m+[m[32m            // Κλείσιμο του άκρου εγγραφής του pipe[m
[32m+[m[32m            close(pipe_fd[1]);[m
[32m+[m
[32m+[m[32m            // Τερματισμός του παιδιού[m
             exit(EXIT_SUCCESS);[m
[32m+[m[32m        } else {[m
[32m+[m[32m            // Κώδικας για τον γονέα[m
[32m+[m[32m            child_pids[i] = pid;[m
         }[m
     }[m
 [m
[31m-    // Parent closes write end of the pipe after forking[m
[31m-    close(pd[1]);[m
[32m+[m[32m    // Κλείσιμο του άκρου εγγραφής του pipe στον γονέα[m
[32m+[m[32m    close(pipe_fd[1]);[m
 [m
[31m-    // Parent reads from the pipe as long as it's open[m
[32m+[m[32m    // Διάβασμα από το pipe στον γονέα[m
     char buffer[256];[m
[31m-    ssize_t nread;[m
[31m-[m
[31m-    while ((nread = read(pd[0], buffer, sizeof(buffer))) > 0)[m
[31m-    {[m
[31m-        // Process each chunk of data from the pipe[m
[31m-        write(STDOUT_FILENO, buffer, nread);[m
[32m+[m[32m    while (read(pipe_fd[0], buffer, sizeof(buffer)) > 0) {[m
[32m+[m[32m        printf("Parent - %s", buffer);[m
     }[m
 [m
[31m-    // Parent waits for child processes to terminate[m
[31m-    pid_t terminated_pid;[m
[31m-    int status;[m
[32m+[m[32m    // Κλείσιμο του άκρου ανάγνωσης του pipe στον γονέα[m
[32m+[m[32m    close(pipe_fd[0]);[m
 [m
[31m-    while ((terminated_pid = wait(&status)) > 0)[m
[31m-    {[m
[31m-        printf("Child process with PID %d has terminated.\n", terminated_pid);[m
[32m+[m[32m    // Αναμονή για τον τερματισμό των παιδιών[m
[32m+[m[32m    for (int i = 0; i < NUM_CHILDREN; ++i) {[m
[32m+[m[32m        waitpid(child_pids[i], NULL, 0);[m
     }[m
 [m
[31m-    // End of the program[m
[31m-    printf("All child processes have terminated.\n");[m
[32m+[m[32m    // Εκτύπωση των PIDs των παιδιών όταν αυτά τερματίσουν[m
[32m+[m[32m    printf("Parent - Child processes terminated. PIDs: ");[m
[32m+[m[32m    for (int i = 0; i < NUM_CHILDREN; ++i) {[m
[32m+[m[32m        printf("%d ", child_pids[i]);[m
[32m+[m[32m    }[m
[32m+[m[32m    printf("\n");[m
 [m
     return 0;[m
 }[m
[41m+[m
[41m+[m
