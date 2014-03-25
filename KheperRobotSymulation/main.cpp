#include "CommunicationManager.h"
#include "CircularEnt.h"
#include "RectangularEnt.h"
#include "KheperaRobot.h"
#include <iostream>

int main(int argc, char** argv)
{
	CircularEnt* c = new CircularEnt(0, 1024, true, 250, 250, 40);
	RectangularEnt* r = new RectangularEnt(1, 12, false, 100, 40, 30, 50);
	CircularEnt* c2 = new CircularEnt(3, 10, true, 150, 200, 60);
	KheperaRobot* robot = new KheperaRobot(2, 10, 250, 150, 30, 5, 10, 0);
	RectangularEnt* rotatedRect = new RectangularEnt(4, 45, false, 90, 130, 110, 110, 130, 130, 110, 150);
	Buffer b2;
	const int symulationDelay = 100; // in [ msec ]

	Symulation* symulation = new Symulation(200, 400);
	CommunicationManager* commMan = new CommunicationManager(symulation);

	symulation->AddEntity(c);
	symulation->AddEntity(r);
	symulation->AddEntity(c2);
	symulation->AddEntity(robot);
	symulation->AddEntity(rotatedRect);

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
		/* make some simple simulation */
		robot->SetLeftMotorSpeed(4);
		robot->SetRightMotorSpeed(2);

		int i = 0;
		while (i < 9)
		{
			std::cout << ">>>>>>> Symualtion step: " << i << std::endl;
			Sleep(symulationDelay);
			symulation->Update(1);
			commMan->SendWorldDescriptionToVisualisers(); 
			i++;
		}

		robot->SetRightMotorSpeed(4);
		while (i < 20)
		{
			std::cout << ">>>>>>> Symualtion step: " << i << std::endl;
			Sleep(symulationDelay);
			symulation->Update(1);
			commMan->SendWorldDescriptionToVisualisers();
			i++;
		}

		robot->SetLeftMotorSpeed(4);
		while (i < 30)
		{
			std::cout << ">>>>>>> Symualtion step: " << i << std::endl;
			Sleep(symulationDelay);
			symulation->Update(1);
			commMan->SendWorldDescriptionToVisualisers();
			i++;
		}
	}
	else
		std::cout << "ERROR!" << std::endl;

	delete symulation;
	delete commMan;

	WSACleanup();

	getchar();
	return 0;
}