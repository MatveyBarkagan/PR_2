# Имя исполняемого файла
TARGET = myprogram

# Компилятор
CC = gcc

# Флаги компилятора
CFLAGS = -Wall -Wextra -I.

# Исходные файлы
SRCS = main.c mylib.c

# Объектные файлы (они будут создаваться из исходных файлов)
OBJS = main.o mylib.o

# Правило по умолчанию: компилировать все и создать исполняемый файл
$(TARGET): $(OBJS)
  $(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Правило для создания объектных файлов
main.o: main.c mylib.h
  $(CC) $(CFLAGS) -c main.c

mylib.o: mylib.c mylib.h
  $(CC) $(CFLAGS) -c mylib.c

# Правило для очистки: удалить объектные файлы и исполняемый файл
clean:
  rm -f $(OBJS) $(TARGET)
