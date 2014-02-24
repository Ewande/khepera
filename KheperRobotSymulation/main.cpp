#include "CommunicationManager.h"
#include <iostream>

int main(int argc, char** argv)
{
	Symulation* symulation = new Symulation();
	CommunicationManager* commMan = new CommunicationManager(symulation);

	// init WINSock
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	if (commMan->Init())
	{
		std::cout << "DONE!" << std::endl;
	}
	else
		std::cout << "ERROR!" << std::endl;

	delete symulation;
	delete commMan;

	WSACleanup();

	return 0;
}