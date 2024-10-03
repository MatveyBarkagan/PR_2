#include "mylib.h"
#include <stdio.h>
#include <string.h>

// Преобразование уровня журналирования в строку
const char* level_to_string(int level) {
    switch (level) {
        case DEBUG: return "DEBUG";
        case INFO: return "INFO";
        case WARN: return "WARN";
        case ERROR: return "ERROR";
        case CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

// Форматирование сообщения в зависимости от указанного формата
void format_message(const char* msg, int level, int format, char* buffer, size_t buffer_size) {
    switch (format) {
        case FORMAT_PLAIN:
            snprintf(buffer, buffer_size, "[%s] %s", level_to_string(level), msg);
            break;
        case FORMAT_JSON:
            snprintf(buffer, buffer_size, "{\"level\": \"%s\", \"message\": \"%s\"}", level_to_string(level), msg);
            break;
        default:
            snprintf(buffer, buffer_size, "%s", msg);
            break;
    }
}

// Основная функция журналирования
int mysyslog(const char* msg, int level, int format, const char* path) {
    if (!msg || !path) {
        return -1; // Неверные аргументы
    }

    // Открытие файла для добавления записей
    FILE* logfile = fopen(path, "a");
    if (!logfile) {
        perror("Ошибка открытия файла журналирования");
        return -1;
    }

    // Буфер для форматированного сообщения
    char formatted_msg[1024];
    format_message(msg, level, format, formatted_msg, sizeof(formatted_msg));

    // Запись форматированного сообщения в файл
    if (fprintf(logfile, "%s\n", formatted_msg) < 0) {
        perror("Ошибка записи в файл журналирования");
        fclose(logfile);
        return -1;
    }

    fclose(logfile);
    return 0;
}
