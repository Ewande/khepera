#include "CommunicationManager.h"
#include "DistrSimulation.h"
#include <iostream>
#include <fstream>

template <class T>
void AssertEquals(const T& expected, const T& actual, const std::string& errorMsg)
{
	if (expected != actual)
		std::cout << errorMsg.c_str() << " : expected: " << expected << " actual: " << actual << std::endl;
}
void TestRectangleSerialization();

int main(int argc, char** argv)
{
	TestRectangleSerialization();

	CircularEnt* c = new CircularEnt(0, 1024, true, 350, 250, 40);
	RectangularEnt* r = new RectangularEnt(1, 12, false, 100, 40, 50, 50);

	CircularEnt* c2 = new CircularEnt(3, 45, true, 150, 250, 30);
	KheperaRobot* robot = new KheperaRobot(2, 10, 250, 150, 15, 2, 10, 0);
	RectangularEnt* rotatedRect = new RectangularEnt(4, 45, false, 110, 150, 40, 30, 45);

	Buffer b2;
	const int simulationDelay = 100; // in [ msec ]

	DistrSimulation* simulation = new DistrSimulation(500, 400, true);
	CommunicationManager* commMan = new CommunicationManager(simulation);

	simulation->setCommunicationManager(commMan);

	robot->setLeftMotorSpeed(0);
	robot->setRightMotorSpeed(0);

	simulation->addEntity(c);
	simulation->addEntity(r);
	simulation->addEntity(c2);
	simulation->addEntity(robot);
	simulation->addEntity(rotatedRect);

	simulation->serialize(b2);
	/*for (int i = 0; i < b2.getLength(); i++)
	{
		printf("%x ", b2.getBuffer()[i]);
		if ((i + 1) % 32 == 0)
			std::cout << std::endl;
	}*/

	// init WINSock
	WSADATA wsaData;
	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
    {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	if (commMan->init())
	{
		std::cout << "DONE!" << std::endl;

		simulation->start();
		commMan->runServerLoop();
	}
	else
		std::cout << "ERROR!" << std::endl;

	delete simulation;
	delete commMan;

	WSACleanup();

	getchar();
	return 0;
}

void TestRectangleSerialization()
{
	uint16_t id = 0xDEAD;
	uint32_t weight = 0xCAFE;
	bool movable = true;
	RectangularEnt originalRect(id, weight, movable, 100, 100, 200, 200, 90);

	std::ofstream outputStream;
	outputStream.open("testRectangleSerialized.bin", std::ios::out | std::ios::trunc | std::ios::binary);
	originalRect.serialize(outputStream);
	outputStream.close();

	std::ifstream inputStream;
	inputStream.open("testRectangleSerialized.bin", std::ios::in | std::ios::binary);
	uint8_t shapeID; // this is normally read by symulation class, see Symulation.cpp:56
	inputStream.read(reinterpret_cast<char*>(&shapeID), sizeof(shapeID));
	RectangularEnt deserializedRect(inputStream);

	AssertEquals(id, deserializedRect.getID(), "Blad: identyfikator ksztaltu nie zgadza sie");
	AssertEquals(weight, deserializedRect.getWeight(), "BLAD: waga ksztaltu nie zgadza sie");
	// AssertEquals(movable, deserializedRect.IsMovable(), "BLAD: mozliwosc przesuwania ksztaltu nie zgadza sie");
}