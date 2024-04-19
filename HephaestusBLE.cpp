// HephaestusBLE.cpp : Defines the entry point for the application.
//

#include "HephaestusBLE.h"

#include "pch.h"
#include <iostream>
//#include "WinBleGccl.h"
#include "libBLe.h"
class test
{
public:
	void print_test();
};
void test::print_test()
{
	std::cout << "test" << std::endl;
}


void print_test(char* character, void* context)
{
	std::cout << "printing character " << *character << std::endl;
	std::string things = character;
	test* t = static_cast<test*>(context);
	t->print_test();
	// this method uses windows to send keypress event for the ascii character
	ShowDesktop(things);
}


int main()
{
	std::cout << "Start Programme" << std::endl;
	test* t = new test();
	startBLE(t, &print_test);
}