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
	//TestRectangleSerialization();

    DistrSimulation* simulation = new DistrSimulation(420, 300, true);
    CommunicationManager* commMan = new CommunicationManager(simulation);

    //-- STATIC ENTITIES
	CircularEnt* c = new CircularEnt(0, 1024, true, 350, 250, 40);
	RectangularEnt* r = new RectangularEnt(1, 12, false, 100, 40, 50, 50);
	CircularEnt* c2 = new CircularEnt(3, 45, true, 150, 250, 30);
    RectangularEnt* rotatedRect = new RectangularEnt(4, 45, false, 110, 150, 40, 30, M_PI / 4);
    //-- ROBOTS AND SENSORS
    uint16_t robotId = 2;
	KheperaRobot* robot = new KheperaRobot(robotId, 10, 250, 150, 15, 2, 10, M_PI / 4);
    //Sensor* front = new Sensor(30, M_PI / 4, 0);
    Sensor* frontRight = new Sensor(30, M_PI / 4, M_PI / 12);
    Sensor* frontLeft = new Sensor(30, M_PI / 4, 23 * M_PI / 12);
    Sensor* middleLeft = new Sensor(30, M_PI / 4, 20 * M_PI / 12);
    Sensor* middleRight = new Sensor(30, M_PI / 4, 4 * M_PI / 12);
    Sensor* left = new Sensor(30, M_PI / 4, 18 * M_PI / 12);
    Sensor* right = new Sensor(30, M_PI / 4, 6 * M_PI / 12);
    Sensor* bottomLeft = new Sensor(30, M_PI / 4, 11 * M_PI / 12);
    Sensor* bottomRight = new Sensor(30, M_PI / 4, 13 * M_PI / 12);
    //----

	simulation->setCommunicationManager(commMan);

	simulation->addEntity(c);
	//simulation->addEntity(r);
	simulation->addEntity(c2);
	simulation->addEntity(robot);
	//simulation->addEntity(rotatedRect);

    //simulation->addSensor(front, robotId);
    simulation->addSensor(frontRight, robotId);
    simulation->addSensor(frontLeft, robotId);
    simulation->addSensor(right, robotId);
    simulation->addSensor(left, robotId);
    simulation->addSensor(middleRight, robotId);
    simulation->addSensor(middleLeft, robotId);
    simulation->addSensor(bottomRight, robotId);
    simulation->addSensor(bottomLeft, robotId);

	// WINSock
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