#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

using namespace std;

#define ERROR_S "CLIENT_ERROR"
#define SERVER_IP "127.0.0.1" //Значение IP порта
#define DEFAULT_PORT 1611
#define SERVER_CLOSE_CONNECTION_SYMBOL '#'
#define BUFFER_SIZE 1024

//Проверка на завершающий символ
bool is_cilent_connection_close(const char *msg)
{
	for(int i=0; i<strlen(msg); i++)
	{
		if(msg[i] == SERVER_CLOSE_CONNECTION_SYMBOL)
			return true;
	}
	return false;
}

int main(int argc, const char *argv[])
{
	int client;
	struct sockaddr_in server_address;

	client = socket(AF_INET, SOCK_STREAM, 0);

	if(client < 0)
	{
		cout << ERROR_S << "sock_err" << endl;
		exit(0);
	}

	server_address.sin_port = htons(DEFAULT_PORT);
	server_address.sin_family = AF_INET;
	//Задаём параметр синхронизации
	inet_pton(AF_INET, SERVER_IP, &server_address.sin_addr);

	cout << endl << "Client's socket created" << endl;

	//Попытка подключения к серверу
	if(connect(client, reinterpret_cast<const struct sockaddr*>(&server_address), sizeof(server_address)))
		cout << "Connect to server..." << inet_ntoa(server_address.sin_addr) << ", port №: " << DEFAULT_PORT <<endl;

	char buffer[BUFFER_SIZE];
	cout << "Wait to serv confirm..." << endl;
	//Функция для получения от сервера сообщения
	recv(client, buffer, BUFFER_SIZE, 0);
	cout << "Connection estabilished" << "port: " << DEFAULT_PORT << "ip: " << SERVER_IP
	<< endl << "Enter " << SERVER_CLOSE_CONNECTION_SYMBOL << " for close connection ";

	//Создаём условия завершения соединения
	while(true)
	{
		cout << "Client №1: ";
		cin.getline(buffer, BUFFER_SIZE);
		send(client, buffer, BUFFER_SIZE, 0);
		if(is_cilent_connection_close(buffer))
			break;

		cout << "Server №1: ";
		recv(client, buffer, BUFFER_SIZE, 0);
		cout << buffer;
		if(is_cilent_connection_close(buffer))
			break;
		cout << endl;
	}
	cout << endl << "Goodbye" << endl;
}
