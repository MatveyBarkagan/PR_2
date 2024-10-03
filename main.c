#include "mylib.h"
#include <stdio.h>

int main() {
    const char* message = "Hello, world!";
    const char* logFilePath = "logfile.txt";

    int result = mysyslog(message, INFO, FORMAT_PLAIN, logFilePath);

    if (result == 0) {
        printf("Message '%s' logged successfully to '%s'!\n", message, logFilePath);
    } else {
        fprintf(stderr, "Error logging message '%s' to '%s'\n", message, logFilePath);
    }

    return result;
}
