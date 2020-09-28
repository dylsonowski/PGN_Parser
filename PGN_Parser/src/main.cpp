#include "PGN_Parser.h"
#include <iostream>

int main() {

	Pale::PGN_Parser _parser;
	int choice, exampleCount;
	std::string path, name;

begin:
	system("cls");
	std::cout << "Main Menu\n";
	std::cout << "Please make your selection\n";
	std::cout << "1 - Add file\n";
	std::cout << "2 - Remove file\n";
	std::cout << "3 - Number files to parse\n";
	std::cout << "4 - Parse\n";
	std::cout << "5 - Quite\n";
	std::cout << "Selection: ";
	std::cin >> choice;
	while (choice != 5) {
		switch (choice)
		{
		case 1:
			std::cout << "What is full path to file?\n";
			std::cin >> path;
			printf("Elements in vector: %d\n", _parser.AddFile(path));
			system("pause");
			goto begin;
			break;
		case 2:
			std::cout << "What is file name?\n";
			std::cin >> name;
			_parser.RemoveFile(name);
			goto begin;
			break;
		case 3:
			std::cout << "In queue to parse is: " << _parser.GetFilesCount() << " files\n";
			goto begin;
			break;
		case 4:
			std::cout << "How many examples per file you want?\n";
			std::cin >> exampleCount;
			_parser.Parse(exampleCount);
			goto begin;
			break;
		default:
			system("cls");
			std::cout << "Main Menu\n";
			std::cout << "Please make your selection\n";
			std::cout << "1 - Add file\n";
			std::cout << "2 - Remove file\n";
			std::cout << "3 - Number files to parse\n";
			std::cout << "4 - Parse\n";
			std::cout << "5 - Quite\n";
			std::cout << "Selection: ";
			std::cin >> choice;
			break;
		}
	} 

	system("pause");
	return 0;
}