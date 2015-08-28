#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include <map>
#include <set>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>

#include "Simulation.h"

#include "ClientCommands\ClientCommand.h"
#include "ClientCommands\RobotMotorSpeedChangeCommand.h"

class Simulation;
class ClientCommand;

class CommunicationManager
{
	public:
        static const uint8_t CONTROLLER = 0;
        static const uint8_t VISUALIZER = 1;

		const int LISTEN_PORT = 6020;
		const char* LISTEN_PORT_STR = "6020";
		static const int NUMBER_OF_CONTROLLERS_COMMANDS;
			

		CommunicationManager(Simulation* simulation);
		~CommunicationManager();

		bool init();

		// starts server loop, which receives and responds to clients requests
		// WARNING: blocks current thread
		void runServerLoop(); 

		// called by Simulation every time when simulation state is updated, to inform visualisers about it
		void sendWorldDescriptionToVisualisers();

        void sendRobotsStatesToControllers();
	private:
		SOCKET                     _listenSocket; 
		Simulation*                _simulation;
		bool                       _isStopped; // if there was request to stop communication manager
		CRITICAL_SECTION           _clientsMutex; // light mutex used to protect _visualisers to be read and written simultaneously

		// connected clients
		std::map<int, SOCKET>      _robotsControllers;
		std::set<SOCKET>           _visualisers; // we don't need to distinguish visualisers, each of them has equal rights

		// arrays of ClientCommand*
		ClientCommand**            _robotsControlersCommandsList;

		bool accept_new_client(); // accepts client, that is trying to connect, and adds it to appropriate clients set

		// receives and executes messages sent by robot controllers
		// sockets -> sockets, that have message to receive, get from select function
		void receive_robot_controlers_messages(fd_set* sockets);

		// the same method for visualisers messages
		void receive_visualisers_messages(fd_set* sockets);
};

#endif