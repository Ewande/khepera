#include "CommunicationManager.h"
#include "CircularEnt.h"
#include "RectangularEnt.h"
#include <iostream>

int main(int argc, char** argv)
{
	CircularEnt* c = new CircularEnt(0, 1024, true, 250, 250, 40);
	RectangularEnt* r = new RectangularEnt(1, 12, false, 100, 40, 30, 50);
	CircularEnt* c2 = new CircularEnt(3, 45, true, 150, 200, 60);
	Buffer b2;

	Symulation* symulation = new Symulation(200, 400);
	CommunicationManager* commMan = new CommunicationManager(symulation);

	symulation->AddEntity(c);
	symulation->AddEntity(r);
	symulation->AddEntity(c2);

	symulation->Serialize(b2);
	for (int i = 0; i < b2.GetLength(); i++)
	{
		printf("%x ", b2.GetBuffer()[i]);
		if ((i + 1) % 32 == 0)
			std::cout << std::endl;
	}

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
		commMan->SendWorldDescriptionToVisualisers();
	}
	else
		std::cout << "ERROR!" << std::endl;

	delete symulation;
	delete commMan;

	WSACleanup();

	return 0;
}