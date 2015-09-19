#include "CommunicationManager.h"
#include "DistrSimulation.h"
#include <iostream>
#include <fstream>

char* getCmdOption(char** begin, char** end, const std::string& option)
{
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
        return *itr;
    return 0;
}

bool cmdOptionExists(char** begin, char** end, const std::string& option)
{
    return std::find(begin, end, option) != end;
}

int main(int argc, char** argv)
{
    char* inputFile = NULL;
    if (cmdOptionExists(argv, argv + argc, "-h"))
    {
        std::cout << "This simulation server is a part of Khepera Simulation System. More information about "
            << "the project, protocol description and usage can be found at github.com/*.\n\n";
        std::cout << "Flags to use as command line arguments:\n";
        std::cout << "   -in FILE\tspecifies input world description file\n";
        std::cout << "   [-bin]\tindicates that input file should be read as a binary file" << std::endl;
        return 0;
    }
    if (cmdOptionExists(argv, argv + argc, "-in") && (inputFile = getCmdOption(argv, argv + argc, "-in")));
    else
    {
        std::cout << "World description file not passed. Use '-in' flag to pass input file." << std::endl;
        return 1;
    }
    std::ifstream file(inputFile);
    if (!file.is_open())
    {
        std::cout << "World description file could not be found." << std::endl;
        return 2;
    }
    bool readBinary = cmdOptionExists(argv, argv + argc, "-bin");
    DistrSimulation simulation(file, readBinary);
    CommunicationManager commMan(&simulation);
    simulation.setCommunicationManager(&commMan);

	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
    {
		std::cout << "WSAStartup failed. Error code: " << iResult << std::endl;
		return 3;
	}
	if (commMan.init())
	{
		simulation.start();
		commMan.runServerLoop();
	}
    else
    {
        WSACleanup();
        return 4;
    }
	WSACleanup();

	return 0;
}