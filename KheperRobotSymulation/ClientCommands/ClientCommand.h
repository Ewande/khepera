#ifndef CLIENT_COMMAND_H
#define CLIENT_COMMAND_H

#include <stdint.h>
#include <WinSock2.h>

#include "../Symulation.h"

class ClientCommand
{
	public:
		// Error codes list
		static const uint16_t ERROR_CODE_SUCCESS = 0;

		ClientCommand(uint8_t id) : _id(id) {}
		virtual ~ClientCommand() {}

		// Execute this command on given symulation object, get necessary args from socket, should return error code
		virtual uint16_t Execute(Symulation* sym, SOCKET sock) = 0;
	protected:
		uint8_t   _id;
};

#endif
