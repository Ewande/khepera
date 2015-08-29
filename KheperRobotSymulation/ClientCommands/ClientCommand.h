#ifndef CLIENT_COMMAND_H
#define CLIENT_COMMAND_H

#include <stdint.h>
#include <WinSock2.h>

#include "../DistrSimulation.h"

class DistrSimulation;

class ClientCommand
{
	public:
		// Error codes list
		static const uint16_t ERROR_CODE_SUCCESS = 0;
		static const uint16_t ERROR_CODE_ENTITY_DOESNT_EXIST = 1;
		// entity exists, but is unapropriate for command
		static const uint16_t ERROR_CODE_INVALID_ENTITY_ID = 2; 
		static const uint16_t ERROR_CODE_INVALID_MOTOR_ID = 3;

		// contoller commands with IDs
		static const uint8_t  ROBOT_MOTOR_SPEED_CHANGE_COMMAND_ID = 0;

		ClientCommand(uint8_t id) : _id(id) {}
		virtual ~ClientCommand() {}

		// Execute this command on given simulation object, get necessary args from socket, should return error code
		virtual uint16_t execute(DistrSimulation* sim, SOCKET sock) = 0;
	protected:
		uint8_t   _id;
};

#endif
