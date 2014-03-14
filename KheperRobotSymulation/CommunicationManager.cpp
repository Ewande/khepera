#include "CommunicationManager.h"

CommunicationManager::CommunicationManager(Symulation* sym)
{
	_symulation = sym;
}

CommunicationManager::~CommunicationManager()
{
	/* close visualisers sockets */

	for (std::set<SOCKET>::iterator it = _visualisers.begin();
		it != _visualisers.end(); it++)
	{
		shutdown(*it, SD_SEND);
		closesocket(*it);
	}
}

bool CommunicationManager::Init()
{
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	int iResult = getaddrinfo(NULL, LISTEN_PORT_STR, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		WSACleanup();
		return false;
	}

	// create socket
	_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (_listenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return false;
	}

	// bind socket
	iResult = bind(_listenSocket, result->ai_addr, (int) result->ai_addrlen);
	freeaddrinfo(result);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		closesocket(_listenSocket);
		WSACleanup();
		return false;
	}

	ListenForNewClient();

/*	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed: %d\n", WSAGetLastError());
		closesocket(ClientSocket);
		WSACleanup();
		return false;
	}
	closesocket(ClientSocket); */

	return true;
}

void CommunicationManager::SendWorldDescriptionToVisualisers()
{
	Buffer b;
	_symulation->Serialize(b);

	for (std::set<SOCKET>::iterator it = _visualisers.begin();
		it != _visualisers.end(); it++)
	{
		send(*it, reinterpret_cast<const char*>(b.GetBuffer()), b.GetLength(), 0);
	}
}

bool CommunicationManager::ListenForNewClient()
{
	// listen
	if (listen(_listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(_listenSocket);
		WSACleanup();
		return false;
	}

	SOCKET ClientSocket = INVALID_SOCKET;
	// Accept a client socket
	ClientSocket = accept(_listenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(_listenSocket);
		WSACleanup();
		return false;
	}

	uint8_t newClientType;

	recv(ClientSocket, reinterpret_cast<char*>(&newClientType), 1, 0);
	if (newClientType == 1)
	{
		_visualisers.insert(ClientSocket);
		return true;
	}

	return false;
}