#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int tube[2];
    if (pipe(tube) == -1) {
        perror("pipe");
        exit(1);}
    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(2);
    }
    if (pid == 0) {
        close(tube[1]);
        int n;
        while (read(tube[0], &n, sizeof(int)) > 0) 
            printf("[Fils] %d\n", n);
        close(tube[0]);
        exit(1);
    } else {
        close(tube[0]);
        int N = 4;
        for (int i = 1; i <= N; i++) {
            write(tube[1], &i, sizeof(int));
            sleep(1);}
        close(tube[1]);
        int status;
        waitpid(pid, &status, 0);
        printf("[Père] : Mon fils %d est terminé avec le code %d.\n", pid, WEXITSTATUS(status));
    }
    return 0;
}
