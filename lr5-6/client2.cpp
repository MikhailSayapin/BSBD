#include "error_functions.h"
#include "unix_socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100

int main(int argc, char *argv[]) 
{
    struct sockaddr_un addr;
    ssize_t numb_read;
    char buf[BUF_SIZE];

    // Создадим новый клиентский сокет с доменом: AF_UNIX, типом: SOCK_STREAM
    int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    printf("Client socket id = %d\n", sock_fd);

    // Убедимся, что файловый дескриптор сокета действителен
    if (sock_fd == -1)
      return 0;

    // Присвоим адрес сервера и установим соединение
    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

    // Подключаем активный сокет "sock_fd" к прослушивающему сокету
    if (connect(sock_fd, (struct sockaddr *) &addr,
                sizeof(struct sockaddr_un)) == -1)
        return 0;

    //
    // Копируем stdin в socket
    //

    // Читаем не более BUF_SIZE байт из STDIN в buf.
    while ((numb_read = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) 
    {
      // Затем запишем эти байты из buf в сокет.
      if (write(sock_fd, buf, numb_read) != numb_read)
        return 0;
    }

    if (numb_read == -1)
      return 0;

    // Закрываем сокет.
    exit(EXIT_SUCCESS);
}
