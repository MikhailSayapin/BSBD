/*Данный код после компиляции
образует программу, которая 
будет запускаться*/

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>

using namespace std;

int main(int argc, char *argv[])
{
	if(argc < 4)
	{cerr << "Error occured with arguments" << endl; return 0;}
	
	//Объекты класса для записи/чтения из файла
	ofstream fout, ferr; 
	ifstream fin;

	string in, out, err, path, temp;
	//Присвоимаргументы строковым переменным
	stdin = (argv[1]).c_str(); stdout = (argv[2]).c_str(); stderr = (argv[3]).c_str(); path = (argv[4]).c_str();

	if(stdin == "default")
	{
		//Сохранение содержимого потока ввода в temp
		getline(cin, temp);
		if(stdout == "default")
			cout << "Output is OK. String content: " << temp << endl;
		else
		{
			fout.open("out.txt");
			if(!fout.is_open()) cout << "Error with output file" << endl;
			fout << "Output is OK. String content: " << temp << endl;
			fout.close();
		}

		if(stderr == "default")
			cerr << "Error found" << endl;
		else
		{
			ferr.open("err.txt");
			if(!ferr.is_open()) cout << "Error with output file" << endl;
			ferr << "Error found" << endl;
			ferr.close();
		}
	}
	else
	{
		fin.open("in.txt");
		if(!fin.is_open()) cout << "Error with imput file" << endl;
		getline(fin, temp);
		if(stdout == "default")
			cout << "Output is OK. String content: " << temp << endl;
		else
		{
			fout.open("out.txt");
			if(!fout.is_open()) cout << "Error with output file" << endl;
			fout << "Output is OK. String content: " << temp << endl;
			fout.close();
		}
		
		if(stderr == "default")
			cerr << "Error found" << endl;
		else
		{
			ferr.open("err.txt");
			if(!ferr.is_open()) cout << "Error with output file" << endl;
			ferr << "Error found" << endl;
			ferr.close();
		}
		fin.close();
	}
}
