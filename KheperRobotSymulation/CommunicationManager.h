#ifndef COMMUNICATION_MANAGER_H
#define COMMUNICATION_MANAGER_H

#include <map>
#include <set>
#include <WinSock2.h>
#include <Ws2tcpip.h>

#include "Symulation.h"

class Symulation;

class CommunicationManager
{
	public:
		const int LISTEN_PORT = 6020;
		const char* LISTEN_PORT_STR = "6020";

		CommunicationManager(Symulation* symulation);
		~CommunicationManager();

		bool Init();

		// called by Symulation after every time when symulation state is updated, to inform visualisers about it
		void SendWorldDescriptionToVisualisers();
	private:
		SOCKET                     _listenSocket; 
		Symulation*                _symulation;

		// connected clients
		std::map<uint8_t, SOCKET>  _robotsControlers;
		std::set<SOCKET>  _visualisers; // we don't need to distinguish visualisers, each of them has equal rights

		bool ListenForNewClient();
};

#endif