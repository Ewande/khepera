#include "CommunicationManager.h"
#include "CircularEnt.h"
#include "RectangularEnt.h"
#include "KheperaRobot.h"
#include <iostream>

int main(int argc, char** argv)
{
	const int symulationDelay = 100; // in [ msec ]

/*	CircularEnt* c = new CircularEnt(0, 1024, true, 350, 250, 40);
	RectangularEnt* r = new RectangularEnt(1, 12, false, 100, 40, 30, 50);

	CircularEnt* c2 = new CircularEnt(3, 45, true, 150, 250, 30);
	KheperaRobot* robot = new KheperaRobot(2, 10, 250, 150, 15, 2, 10, 0);
	RectangularEnt* rotatedRect = new RectangularEnt(4, 45, false, 110, 150, 130, 130, 150, 150, 130, 170);

	Symulation* symulation = new Symulation(200, 400);

	robot->SetLeftMotorSpeed(0);
	robot->SetRightMotorSpeed(0);

	symulation->AddEntity(c);
	symulation->AddEntity(r);
	symulation->AddEntity(c2);
	symulation->AddEntity(robot);
	symulation->AddEntity(rotatedRect);

	std::ofstream  file("test.wld", std::ios::out | std::ios::binary | std::ios::trunc);
	symulation->Serialize(file);
	file.close(); */

	std::ifstream worldFile("test.wld", std::ios::in | std::ios::binary);
	Symulation* symulation = new Symulation(worldFile);
	worldFile.close();


	Buffer b2;
	symulation->Serialize(b2);
	for (int i = 0; i < b2.GetLength(); i++)
	{
		printf("%x ", b2.GetBuffer()[i]);
		if ((i + 1) % 32 == 0)
			std::cout << std::endl;
	}

	CommunicationManager* commMan = new CommunicationManager(symulation);

	symulation->SetCommunicationManager(commMan);

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

		symulation->Start();
		commMan->RunServerLoop();
	}
	else
		std::cout << "ERROR!" << std::endl;

	delete symulation;
	delete commMan;

	WSACleanup();

	getchar();
	return 0;
}