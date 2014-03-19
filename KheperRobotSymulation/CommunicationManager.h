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

		// starts server loop, which responds to clients requests
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
		std::map<uint16_t, SOCKET> _robotsControlers;
		std::set<SOCKET>           _visualisers; // we don't need to distinguish visualisers, each of them has equal rights

		bool AcceptNewClient();
};

#endif