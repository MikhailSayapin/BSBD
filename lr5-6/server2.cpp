#include "error_functions.h"
#include "unix_socket.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>

#include <sys/socket.h>
#include <sys/un.h>

#define BACKLOG 5
#define SV_SOCK_PATH "/tmp/us_xfr"
#define BUF_SIZE 100

int main(int argc, char *argv[]) 
{
  struct sockaddr_un addr;
  std::ofstream fout;
  fout.open("log.txt");
  if(!fout.is_open())
    std::cout << "Error with file" << std::endl;

  // Создадим новый клиентский сокет с доменом: AF_UNIX, типом: SOCK_STREAM
  int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  printf("Server socket fd = %d\n", sock_fd);
  fout << "Server socket fd = " << sock_fd << std::endl;

  // Убедимся, что файловый дескриптор сокета действителен
  if (sock_fd == -1)
    return 0;

  // Убедимся, что адрес, который мы планируем использовать, не слишком длинный.
  if (strlen(SV_SOCK_PATH) > sizeof(addr.sun_path) - 1)
    return 0;

  // Удалим все файлы, которые уже существуют по указанному адресу.
  if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
    return 0;

  // Обнулим адрес и установим семейство и путь.
  memset(&addr, 0, sizeof(struct sockaddr_un));
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

  // Привяжем сокет к адресу
  if (bind(sock_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
    return 0;

  // Вызов прослушивания помечает сокет как пассивный. Сокет будет впоследствии
  // использоваться для приема подключений от активных сокетов.
  if (listen(sock_fd, BACKLOG) == -1)
    return 0;

  ssize_t numb_read;
  char buf[BUF_SIZE];
  while(true) 
  {  
    /*Примем соединение. Соединение возвращается на НОВЫЙ сокет, 'cfd'; 
    прослушивающий сокет ('sock_fd') остается открытым
    и может использоваться для принятия дальнейших подключений.*/
    printf("Waiting to accept a connection...\n"); 
    fout << "Waiting to accept a connection...\n";
    int cfd = accept(sock_fd, NULL, NULL);
    printf("Accepted socket id = %d\n", cfd);
    fout << "Accepted socket id = " << cfd << std::endl;

    // Передача данных из подключенного сокета в стандартный вывод до EOF и в файл

    // Прочтём не более BUF_SIZE байт из сокета в buf.
    while ((numb_read = read(cfd, buf, BUF_SIZE)) > 0) 
    {
      // Запишем эти байты из buf в STDOUT
      if (write(STDOUT_FILENO, buf, numb_read) != numb_read)
        return 0;
    }

    if (numb_read == -1)
      return 0;

    if (close(cfd) == -1)
      return 0;
  }
  fout.close();
}
