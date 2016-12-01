#include "CommunicationManager.h"

const int CommunicationManager::NUMBER_OF_CONTROLLER_COMMANDS = 
	ClientCommand::MOTORS_SPEED_CHANGE_COMMAND_ID + 1;

CommunicationManager::CommunicationManager(DistrSimulation* sim) : _isStopped(false), _simulation(sim)
{
	InitializeCriticalSection(&_clientsMutex);

	// initialize arrays with clients commands
    _validControllerCommands = new ClientCommand*[NUMBER_OF_CONTROLLER_COMMANDS];
    _validControllerCommands[ClientCommand::SINGLE_MOTOR_SPEED_CHANGE_COMMAND_ID] =
		new SingleMotorSpeedChangeCommand();
    _validControllerCommands[ClientCommand::MOTORS_SPEED_CHANGE_COMMAND_ID] =
        new MotorsSpeedChangeCommand();
}

CommunicationManager::~CommunicationManager()
{
	/* close visualisers sockets */
	for (std::set<SOCKET>::iterator it = _visualisers.begin(); it != _visualisers.end(); it++)
	{
		shutdown(*it, SD_SEND);
		closesocket(*it);
	}

	// close robot controlers sockets
    for (std::map<uint16_t, SocketData>::iterator it = _controllers.begin(); it != _controllers.end(); it++)
	{
		shutdown(it->second.socket, SD_SEND);
		closesocket(it->second.socket);
	}

	// delete commands
	for (int i = 0; i < NUMBER_OF_CONTROLLER_COMMANDS; i++)
        delete _validControllerCommands[i];

    delete[] _validControllerCommands;

	DeleteCriticalSection(&_clientsMutex);
}

bool CommunicationManager::init()
{
	struct addrinfo *result = NULL, *ptr = NULL, hints;

	ZeroMemory(&hints, sizeof (hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the local address and port to be used by the server
	int iResult = getaddrinfo(NULL, LISTEN_PORT_STR, &hints, &result);
	if (iResult != 0)
    {
        std::cout << "getaddrinfo failed. Error code: " << iResult << std::endl;
		return false;
	}

	// create socket
	_listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (_listenSocket == INVALID_SOCKET)
    {
		std::cout << "Error at socket(). Error code: " << WSAGetLastError() << std::endl;
		freeaddrinfo(result);
		return false;
	}

	// bind socket
	iResult = bind(_listenSocket, result->ai_addr, (int) result->ai_addrlen);
	freeaddrinfo(result);
	if (iResult == SOCKET_ERROR)
    {
		std::cout << "bind failed. Error code: " << WSAGetLastError() << std::endl;
		closesocket(_listenSocket);
		return false;
	}

	// listen
	if (listen(_listenSocket, SOMAXCONN) == SOCKET_ERROR)
    {
		std::cout << "Listen failed. Error code: " << WSAGetLastError() << std::endl;
		closesocket(_listenSocket);
		return false;
	}

	return true;
}

void CommunicationManager::runServerLoop()
{
	while (!_isStopped)
	{
		fd_set receivingSockets;
		FD_ZERO(&receivingSockets);

		// add controllers to observed sockets set
        for (std::map<uint16_t, SocketData>::iterator it = _controllers.begin(); it != _controllers.end(); it++)
			FD_SET(it->second.socket, &receivingSockets);

		// add visualisers
		for (std::set<SOCKET>::iterator it = _visualisers.begin(); it != _visualisers.end(); it++)
			FD_SET(*it, &receivingSockets);

		// add server's listen socket
		FD_SET(_listenSocket, &receivingSockets);

		int numberOfSockets = select(0, &receivingSockets, NULL, NULL, NULL);

		if (FD_ISSET(_listenSocket, &receivingSockets))
			accept_new_client();

		receive_controllers_messages(&receivingSockets);
		receive_visualisers_messages(&receivingSockets);
	}
}

void CommunicationManager::sendWorldDescriptionToVisualisers()
{
	Buffer buffer;
    _simulation->serialize(buffer);
    serializeControllersData(buffer);

	EnterCriticalSection(&_clientsMutex); // if server-thread adds new client, iterator would be broken
	for (std::set<SOCKET>::iterator it = _visualisers.begin(); it != _visualisers.end(); it++)
        send(*it, reinterpret_cast<const char*>(buffer.getBuffer()), buffer.getLength(), 0);
	LeaveCriticalSection(&_clientsMutex);
}

void CommunicationManager::sendRobotsStatesToControllers()
{
    EnterCriticalSection(&_clientsMutex); // if server-thread adds new client, iterator would be broken

    for (std::map<uint16_t, SocketData>::iterator it = _controllers.begin(); it != _controllers.end(); it++)
    {
        Buffer buffer;
        dynamic_cast<KheperaRobot*>(_simulation->getEntity(it->first))->serializeForController(buffer);
        send(it->second.socket, reinterpret_cast<const char*>(buffer.getBuffer()), buffer.getLength(), 0);
    }

    LeaveCriticalSection(&_clientsMutex);
}

bool CommunicationManager::accept_new_client()
{
	SOCKET clientSocket = INVALID_SOCKET;
    struct sockaddr sockData;
    int sockDataSize = sizeof(sockData);

	// Accept a client socket
	clientSocket = accept(_listenSocket, &sockData, &sockDataSize);
	if (clientSocket == INVALID_SOCKET)
    {
		printf("accept failed: %d\n", WSAGetLastError());
		closesocket(_listenSocket);
		WSACleanup();
		return false;
	}
    
    // receive new client type information, and it to appropriate container
	uint8_t newClientType;
	recv(clientSocket, reinterpret_cast<char*>(&newClientType), 1, 0);

    unsigned long mode = 1;
    //ioctlsocket(ClientSocket, FIONBIO, &mode);
	if (newClientType == TYPE_ID_VISUALISER)
	{
		EnterCriticalSection(&_clientsMutex);
		    _visualisers.insert(clientSocket);
		LeaveCriticalSection(&_clientsMutex);
	}
    else if (newClientType == TYPE_ID_CONTROLLER)
    {
        uint16_t controlledRobotId;
        recv(clientSocket, reinterpret_cast<char*>(&controlledRobotId), sizeof(controlledRobotId), 0);
        controlledRobotId = ntohs(controlledRobotId);
        SimEnt* robot = _simulation->getEntity(controlledRobotId);
        if (robot != NULL && robot->getShapeID() == SimEnt::KHEPERA_ROBOT 
            && _controllers.find(controlledRobotId) == _controllers.end())
        {
            struct sockaddr_in* parsedData = reinterpret_cast<sockaddr_in*>(&sockData);
            std::cout << "CONTROLLER FOR ROBOT WITH ID = " << controlledRobotId << " SUCCESSFULLY CONNECTED\n";
            EnterCriticalSection(&_clientsMutex);
                _controllers[controlledRobotId] = SocketData(clientSocket, parsedData->sin_port, parsedData->sin_addr);
            LeaveCriticalSection(&_clientsMutex);
        }
        else
        {
            shutdown(clientSocket, SD_RECEIVE);
            closesocket(clientSocket);
            std::cout << "NO ROBOT WITH ID = " << controlledRobotId << " TO CONTROL.\n";
        }
    }

	return true;
}

void CommunicationManager::receive_controllers_messages(fd_set* sockets)
{
	// find who sent us a message
	std::map<uint16_t, SocketData>::iterator it = _controllers.begin();
	while (it != _controllers.end())
	{
		if (FD_ISSET(it->second.socket, sockets))
		{
			uint8_t commandID;
			int dataLength = recv(it->second.socket, reinterpret_cast<char*>(&commandID), 1, 0);
            if (dataLength < 1)
            {
                std::cout << "REMOVING CONTROLLER" << std::endl;
                EnterCriticalSection(&_clientsMutex);
                    closesocket(it->second.socket);
                    _controllers.erase(it++);
                LeaveCriticalSection(&_clientsMutex);
            }
			else
			{
				std::cout << "Received command id: " << (int) commandID << std::endl;
				if (commandID < NUMBER_OF_CONTROLLER_COMMANDS)
					// TODO: Send back error code in case of errors
                    _validControllerCommands[commandID]->
                        execute(*_simulation->getEntity(it->first), *_simulation, it->second.socket);
				it++;
			}
		}
		else
			it++;
	}
}

void CommunicationManager::receive_visualisers_messages(fd_set* sockets)
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
            {
                std::cout << "REMOVING VISUALISER" << std::endl;
                EnterCriticalSection(&_clientsMutex);
                    closesocket(*it);
                    _visualisers.erase(it++);
                LeaveCriticalSection(&_clientsMutex);
            }
		}
		else
			it++;
	}
}

void CommunicationManager::serializeControllersData(Buffer& buffer) const
{
    buffer.pack(static_cast<uint8_t>(_controllers.size()));
    for (std::map<uint16_t, SocketData>::const_iterator it = _controllers.begin(); it != _controllers.end(); it++)
    {
        buffer.pack(htons(it->first));
        buffer.pack(htons(it->second.port));
        buffer.pack(it->second.ip.S_un.S_un_b.s_b1);
        buffer.pack(it->second.ip.S_un.S_un_b.s_b2);
        buffer.pack(it->second.ip.S_un.S_un_b.s_b3);
        buffer.pack(it->second.ip.S_un.S_un_b.s_b4);
    }
}