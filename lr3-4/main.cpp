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
	stdin = argv[1]; stdout = argv[2]; stderr = argv[3]; path = argv[4];

	if(stdin == "default")
	{
		//Сохранение содержимого потока ввода в temp
		getline(cin, temp);
		if(stdout == "default")
			cout << "Output is OK. String content: " << "\t" << temp << argv[1] << endl;
		else
		{
			fout.open("out.txt");
			if(!fout.is_open()) cout << "Error with output file" << endl;
			fout << "Output is OK. String content: " << "\t" << temp << argv[1] << endl;
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
		fin.close();
	}
}
