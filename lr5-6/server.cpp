#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fstream>

using namespace std;

#define DEFAULT_PORT 1611
#define ERROR_S "SERVER ERROR: "
#define BUFFER_SIZE 1024
#define CLIENT_CLOSE_CONNECTION_SYMBOL '#'

//Проверка на завершающий символ
bool is_client_connection_close(const char *msg)
{
	for(int i=0; i < strlen(msg); i++)
	{
		if(msg[i] == CLIENT_CLOSE_CONNECTION_SYMBOL)
			return true;
	}
	return false;
}

int main(int argc, char const* argv[])
{
	ofstream fout; fout.open("log.txt");
	int client, server; //Дескрипторы клиента и сервера
	struct sockaddr_in server_address;
	client = socket(AF_INET, SOCK_STREAM, 0);

	if (client < 0) 
	{ 
		cout << ERROR_S << "Error with socket" << endl; 
		fout << ERROR_S << "Error with socket" << endl; 
		exit(0); 
	}
	else
	{
		cout << "Socket was created, port: " << DEFAULT_PORT << endl;
		fout << "Socket was created, port: " << DEFAULT_PORT << endl;
	}

	server_address.sin_port = htons(DEFAULT_PORT); //Задание порта
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htons(INADDR_ANY);

	//Присваиваем серверу адрес, создаём сокет
	if(bind(client, reinterpret_cast<struct sockaddr*>(&server_address),sizeof(server_address))<0)
	{ 
		cout << ERROR_S << "Error with socket" << endl; 
		fout << ERROR_S << "Error with socket" << endl;
		return -1; 
	}

	socklen_t size = sizeof(server_address);
	cout << "Listening_clients" << endl;
	fout << "Listening_clients" << endl;
	//Прослушиваем заданный порт
	listen(client, 1);

	//Связываем сокеты сервера и клиента для их взаимодействия
	server = accept(client, reinterpret_cast<struct sockaddr*>(&server_address), &size);
	if(server<0)
	{
		cout << ERROR_S << "Cannot accepting" << endl;
		fout << ERROR_S << "Cannot accepting" << endl;
	}

	char buffer[BUFFER_SIZE];
	bool isExit = false;

	//Пока соединение установлено - читаем из этого сокета
	while(server>0)
	{
		strcpy(buffer, "Server connected\n");
		send(server, buffer, BUFFER_SIZE, 0);
		cout << "Connection №1" << endl
		<< 	"Enter " << CLIENT_CLOSE_CONNECTION_SYMBOL << " to end" << endl << endl;
		fout << "Connection №1" << endl
		<< 	"Enter " << CLIENT_CLOSE_CONNECTION_SYMBOL << " to end" << endl << endl;

		cout << "Client №1: "; fout << "Client №1: ";
		recv(server, buffer, BUFFER_SIZE, 0);
		cout << buffer << endl; fout << buffer << endl;
		if(is_client_connection_close(buffer))
			isExit = true;

		//Является ли сообщение завершающим
		while(!isExit)
		{
			cout << "Server №1: "; fout << "Server №1: ";
			cin.getline (buffer, BUFFER_SIZE);
			send(server, buffer, BUFFER_SIZE, 0);
			if(is_client_connection_close(buffer))
				break;

			cout << "Client №1: "; fout << "Client №1: ";
			recv(server, buffer, BUFFER_SIZE, 0);
			cout << buffer << endl; fout << buffer << endl;
			if(is_client_connection_close(buffer))
				break;
		}
		cout << endl << "Goodbye..." << endl;
		fout << endl << "Goodbye..." << endl;
		isExit = false;
		exit(1);
	}
	fout.close();
}
