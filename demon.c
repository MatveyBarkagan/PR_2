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
        running = 0; // Устанавливаем флаг для завершения работы демона
    }
}

void daemonize() {
    pid_t pid;

    pid = fork(); // Создаем дочерний процесс

    if (pid < 0) {
        exit(EXIT_FAILURE); // Ошибка при создании дочернего процесса
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS); // Родительский процесс завершает работу
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE); // Дочерний процесс становится сессионным лидером
    }

    signal(SIGCHLD, SIG_IGN);
    signal(SIGHUP, SIG_IGN);

    pid = fork(); // Создаем второй дочерний процесс

    if (pid < 0) {
        exit(EXIT_FAILURE); // Ошибка при создании дочернего процесса
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS); // Родительский процесс завершает работу
    }

    umask(0); // Устанавливаем права доступа к файлам
    chdir("/"); // Меняем текущую директорию на корневую

    for (int x = sysconf(_SC_OPEN_MAX); x >= 0; x--) {
        close(x); // Закрываем все открытые файловые дескрипторы
    }
}

void read_config(char* path, char* format) {
    FILE *file = fopen(CONFIG_FILE, "r");
    if (file == NULL) {
        perror("Failed to open config file");
        exit(EXIT_FAILURE);
    }

    fscanf(file, "path=%255s\nformat=%15s", path, format);
    fclose(file);
}

void write_log(const char* path, const char* message) {
    FILE *file = fopen(path, "a");
    if (file != NULL) {
        fprintf(file, "%s\n", message);
        fclose(file);
    }
}

int main() {
    daemonize(); // Демонизируем процесс

    signal(SIGTERM, handle_signal); // Устанавливаем обработчики сигналов
    signal(SIGINT, handle_signal);

    char path[256], format[16];
    read_config(path, format); // Читаем конфигурацию

    while (running) {
        write_log(path, "Daemon running..."); // Записываем сообщение в журнал
        sleep(10); // Пауза на 10 секунд
    }

    write_log(path, "Daemon stopping..."); // Записываем сообщение о завершении работы

    return 0;
}
