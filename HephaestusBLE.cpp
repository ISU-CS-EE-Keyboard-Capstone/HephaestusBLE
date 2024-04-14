// HephaestusBLE.cpp : Defines the entry point for the application.
//

#include "HephaestusBLE.h"

#include "pch.h"
#include <iostream>
//#include "WinBleGccl.h"
#include "libBLe.h"

void print_test(char* character)
{
	std::cout << "printing character " << *character << std::endl;
	std::string things = character;
	// this method uses windows to send keypress event for the ascii character
	ShowDesktop(things);
}

int main()
{
	std::cout << "Start Programme" << std::endl;
	startBLE(&print_test);
}