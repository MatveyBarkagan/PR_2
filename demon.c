#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CONFIG_FILE "/etc/mysyslog/mysyslog.cfg"
static volatile int running = 1;

void handle_signal(int sig) {
    if (sig == SIGTERM || sig == SIGINT) {
        running = 0;
    }
}

void daemonize() {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        perror("Failed to become session leader");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGHUP, &sa, NULL) < 0) {
        perror("Failed to ignore SIGHUP");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);
    if (chdir("/") < 0) {
        perror("Failed to change directory");
        exit(EXIT_FAILURE);
    }

    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x);
    }
}

void read_config(char** path, char** format) {
    FILE *file = fopen(CONFIG_FILE, "r");
    if (file == NULL) {
        perror("Failed to open config file");
        exit(EXIT_FAILURE);
    }

    size_t path_len = 256, format_len = 16;
    *path = malloc(path_len);
    *format = malloc(format_len);
    
    if (fscanf(file, "path=%255s\nformat=%15s", *path, *format) != 2) {
        perror("Failed to read config file");
        fclose(file);
        exit(EXIT_FAILURE);
    }
    
    fclose(file);
}

void write_log(const char* path, const char* message) {
    FILE *file = fopen(path, "a");
    if (file != NULL) {
        fprintf(file, "%s\n", message);
        fclose(file);
    } else {
        perror("Failed to open log file");
    }
}

int main() {
    daemonize();

    struct sigaction sa;
    sa.sa_handler = handle_signal;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGTERM, &sa, NULL) < 0 || sigaction(SIGINT, &sa, NULL) < 0) {
        perror("Failed to set signal handlers");
        exit(EXIT_FAILURE);
    }

    char *path = NULL, *format = NULL;
    read_config(&path, &format);

    while (running) {
        write_log(path, "Daemon running...");
        sleep(10);
    }

    write_log(path, "Daemon stopping...");

    free(path);
    free(format);

    return 0;
}
