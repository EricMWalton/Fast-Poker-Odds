#include <iostream>
#include <string>
#include <conio.h>

//using namespace std;



// pauses for user to push a key before exiting (otherwise user can't read the console output)
void pauseExit(int c = 0)
	{
	_getch();
	exit(c);
	}

// displays an error message, pauses to allow the user to read it, and exits
void errorExit(std::string errorMsg)
	{
	std::cout << "Error: " << errorMsg << std::endl;
	pauseExit(1);
	}

