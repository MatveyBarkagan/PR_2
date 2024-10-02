#include "mylib.h" 
#include <stdio.h>
int main() {
    const char* message = "Hello, world!"; // Создаем сообщение для записи в журнал

    // Вызываем функцию mysyslog из нашей библиотеки для записи сообщения в журнал
    int result = mysyslog(message, INFO, FORMAT_PLAIN, "logfile.txt");
    // Проверяем результат вызова функции
    if (result == 0) {
        printf("Message '%s' logged successfully!\n", message); // Если успешно, выводим сообщение об успешной записи
    } else {
        printf("Error logging message '%s'\n", message); // Если произошла ошибка, выводим сообщение об ошибке
    }

    return 0;
}
