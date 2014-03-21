#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include <map>
#include <set>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "Symulation.h"
#include <iostream>

class Symulation;

class CommunicationManager
{
	public:
		const int LISTEN_PORT = 6020;
		const char* LISTEN_PORT_STR = "6020";

		CommunicationManager(Symulation* symulation);
		~CommunicationManager();

		bool Init();

		// starts server loop, which receives and responds to clients requests
		// WARNING: blocks current thread
		void RunServerLoop(); 

		// called by Symulation after every time when symulation state is updated, to inform visualisers about it
		void SendWorldDescriptionToVisualisers();
	private:
		SOCKET                     _listenSocket; 
		Symulation*                _symulation;
		bool                       _isStopped; // if there was request to stop communication manager
		CRITICAL_SECTION           _clientsMutex; // light mutex used to protect _visualisers to be read and written simultaneously

		// connected clients
		std::set<SOCKET>           _robotsControlers; // FIXME: change to map, if one controller can controll only one robot
		std::set<SOCKET>           _visualisers; // we don't need to distinguish visualisers, each of them has equal rights

		bool AcceptNewClient(); // accepts client, that is trying to connect, and adds it to appropriate clients set

		// receives and executes messages sent by robot controllers
		// sockets -> sockets, that have message to receive, get from select function
		void ReceiveRobotControlersMessages(fd_set* sockets);

		// the same method for visualisers messages
		void ReceiveVisualisersMessages(fd_set* sockets);
};

#endif