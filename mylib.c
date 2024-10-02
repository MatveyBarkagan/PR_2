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
    if (format == FORMAT_PLAIN) {
        snprintf(buffer, buffer_size, "[%s] %s", level_to_string(level), msg);
    } else if (format == FORMAT_JSON) {
        snprintf(buffer, buffer_size, "{\"level\": \"%s\", \"message\": \"%s\"}", level_to_string(level), msg);
    } else {
        snprintf(buffer, buffer_size, "%s", msg);
    }
}

// Основная функция журналирования
int mysyslog(const char* msg, int level, int format, const char* path) {
    // Открытие файла для добавления записей
    FILE* logfile = fopen(path, "a");
    if (!logfile) {
        return -1; // Ошибка открытия файла
    }

    // Буфер для форматированного сообщения
    char formatted_msg[1024];
    // Форматирование сообщения
    format_message(msg, level, format, formatted_msg, sizeof(formatted_msg));
    
    // Запись форматированного сообщения в файл
    if (fprintf(logfile, "%s\n", formatted_msg) < 0) {
        fclose(logfile);
        return -1; // Ошибка записи в файл
    }

    // Закрытие файла
    fclose(logfile);
    return 0; // Успешное выполнение
}
