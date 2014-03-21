#include "CommunicationManager.h"

CommunicationManager::CommunicationManager(Symulation* sym) : _isStopped(false)
{
	_symulation = sym;

	InitializeCriticalSection(&_clientsMutex);
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

	// close robot controlers sockets
	for (std::set<SOCKET>::iterator it = _robotsControlers.begin();
		it != _robotsControlers.end(); it++)
	{
		shutdown(*it, SD_SEND);
		closesocket(*it);
	}

	DeleteCriticalSection(&_clientsMutex);
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

	// listen
	if (listen(_listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("Listen failed with error: %ld\n", WSAGetLastError());
		closesocket(_listenSocket);
		WSACleanup();
		return false;
	}

	AcceptNewClient();

	return true;
}

void CommunicationManager::RunServerLoop()
{
	while (!_isStopped)
	{
		fd_set receivingSockets;
		FD_ZERO(&receivingSockets);

		// add controllers to observed sockets set
		for (std::set<SOCKET>::iterator it = _robotsControlers.begin();
			it != _robotsControlers.end(); it++)
			FD_SET(*it, &receivingSockets);

		// add visualisers
		for (std::set<SOCKET>::iterator it = _visualisers.begin();
			it != _visualisers.end(); it++)
			FD_SET(*it, &receivingSockets);

		// add server's listen socket
		FD_SET(_listenSocket, &receivingSockets);

		int numberOfSockets = select(0, &receivingSockets, NULL, NULL, NULL);

		if (FD_ISSET(_listenSocket, &receivingSockets))
			AcceptNewClient();

		ReceiveRobotControlersMessages(&receivingSockets);
		ReceiveVisualisersMessages(&receivingSockets);
	}
}

void CommunicationManager::SendWorldDescriptionToVisualisers()
{
	Buffer b;
	_symulation->Serialize(b);

	EnterCriticalSection(&_clientsMutex); // if server-thread add new client, iterator would be broken

	for (std::set<SOCKET>::iterator it = _visualisers.begin();
		it != _visualisers.end(); it++)
	{
		send(*it, reinterpret_cast<const char*>(b.GetBuffer()), b.GetLength(), 0);
	}

	LeaveCriticalSection(&_clientsMutex);
}

bool CommunicationManager::AcceptNewClient()
{
	SOCKET ClientSocket = INVALID_SOCKET;
	// Accept a client socket
	ClientSocket = accept(_listenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(_listenSocket);
		WSACleanup();
		return false;
	}
	
	// receive new client type information, and it to appropriate container
	uint8_t newClientType;

	recv(ClientSocket, reinterpret_cast<char*>(&newClientType), 1, 0);
	if (newClientType == 1)
	{
		EnterCriticalSection(&_clientsMutex);

		_visualisers.insert(ClientSocket);

		LeaveCriticalSection(&_clientsMutex);
	}
	else
		_robotsControlers.insert(ClientSocket);

	return true;
}

void CommunicationManager::ReceiveRobotControlersMessages(fd_set* sockets)
{
	// find who sent us a message
	std::set<SOCKET>::iterator it = _robotsControlers.begin();
	while (it != _robotsControlers.end())
	{
		if (FD_ISSET(*it, sockets))
		{
			uint8_t message;
			int dataLength = recv(*it, reinterpret_cast<char*>(&message), 1, 0);
			if (dataLength < 1)
				_robotsControlers.erase(it++); // see http://stackoverflow.com/a/263958, the same applies to std::set
			else
			{
				// FIXME: For testing purpose only!
				std::cout << "Message from controler: " << message << std::endl;
				it++;
			}
		}
		else
			it++;
	}
}

void CommunicationManager::ReceiveVisualisersMessages(fd_set* sockets)
{
	// find who sent us a message
	std::set<SOCKET>::iterator it = _visualisers.begin();
	while (it != _visualisers.end())
	{
		if (FD_ISSET(*it, sockets))
		{
			uint8_t message;
			int dataLength = recv(*it, reinterpret_cast<char*>(&message), 1, 0);
			if (dataLength < 1)
				_visualisers.erase(it++); // see http://stackoverflow.com/a/263958, the same applies to std::set
			else
			{
				// FIXME: For testing purpose only!
				std::cout << "Message from visualiser: " << message << std::endl;
				it++;
			}
		}
		else
			it++;
	}
}