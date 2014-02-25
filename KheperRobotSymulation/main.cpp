#include "CommunicationManager.h"
#include "CircularEnt.h"
#include "RectangularEnt.h"
#include <iostream>

int main(int argc, char** argv)
{
	CircularEnt* c = new CircularEnt(0, 1024, true, 666, 666, 4);
	Buffer* b = c->Serialize();

	for (int i = 0; i < b->GetLength(); i++)
	{
		printf("%x ", b->GetBuffer()[i]);
		if ((i + 1) % 32 == 0)
			std::cout << std::endl;
	}
	std::cout << std::endl;

	delete b;
	delete c;

	RectangularEnt* r = new RectangularEnt(1, 12, false, 400, 500, 30, 50);
	b = r->Serialize();

	for (int i = 0; i < b->GetLength(); i++)
	{
		printf("%x ", b->GetBuffer()[i]);
		if ((i + 1) % 32 == 0)
			std::cout << std::endl;
	}

	delete r;
	delete b;

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