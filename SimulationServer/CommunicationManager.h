#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include <map>
#include <set>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <iostream>

#include "DistrSimulation.h"
#include "Simulation/Buffer.h"
#include "ClientCommands/ClientCommand.h"
#include "ClientCommands/RobotSpeedChangeCommands.h"

class DistrSimulation;
class ClientCommand;

class CommunicationManager
{
	public:
        static const uint8_t TYPE_ID_CONTROLLER = 0;
        static const uint8_t TYPE_ID_VISUALISER = 1;

		const int LISTEN_PORT = 6020;
		const char* LISTEN_PORT_STR = "6020";
		static const int NUMBER_OF_CONTROLLER_COMMANDS;
			

		CommunicationManager(DistrSimulation* simulation);
		~CommunicationManager();

		bool init();

		// starts server loop, which receives and responds to clients requests
		// WARNING: blocks current thread
		void runServerLoop(); 

		// called by Simulation every time when simulation state is updated, to inform visualisers about it
		void sendWorldDescriptionToVisualisers();

        void sendRobotsStatesToControllers();
	private:
        class SocketData
        {
            public:
                SocketData() {}
                SocketData(SOCKET _socket, uint16_t _port, in_addr _ip) : socket(_socket), port(_port), ip(_ip) {}
                SOCKET socket;
                uint16_t port;
                in_addr ip;
        };

		SOCKET                     _listenSocket; 
		DistrSimulation*           _simulation;
		bool                       _isStopped; // if there was request to stop communication manager
		CRITICAL_SECTION           _clientsMutex; // light mutex used to protect _visualisers to be read and written simultaneously

		// connected clients
		std::map<uint16_t, SocketData>  _controllers;
		std::set<SOCKET>                _visualisers; // we don't need to distinguish visualisers, each of them has equal rights

		ClientCommand**                 _validControllerCommands;

		bool accept_new_client(); // accepts client, that is trying to connect, and adds it to appropriate clients set

		// receives and executes messages sent by robot controllers
		// sockets -> sockets, that have message to receive, get from select function
		void receive_controllers_messages(fd_set* sockets);

		// the same method for visualisers messages
		void receive_visualisers_messages(fd_set* sockets);

        void serializeControllersData(Buffer& buffer) const;
};

#endif