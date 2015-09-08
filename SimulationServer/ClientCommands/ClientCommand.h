#ifndef CLIENT_COMMAND_H
#define CLIENT_COMMAND_H

#include <stdint.h>
#include <WinSock2.h>

#include "../DistrSimulation.h"
#include "../Simulation/Entities/SimEnt.h"

class DistrSimulation;

class ClientCommand
{
	public:
		// error codes list
		static const uint16_t ERROR_CODE_SUCCESS = 0;
        static const uint16_t ERROR_CODE_INVALID_ENTITY = 1;
        static const uint16_t ERROR_CODE_INVALID_MOTOR_ID = 2;

		// valid controller commands with IDs
		static const uint8_t  SINGLE_MOTOR_SPEED_CHANGE_COMMAND_ID = 0;
        static const uint8_t  MOTORS_SPEED_CHANGE_COMMAND_ID = 1;

		ClientCommand(uint8_t id) : _id(id) {}

		virtual uint16_t execute(SimEnt& entity, DistrSimulation& sim, SOCKET sock) = 0;

	protected:
		uint8_t   _id;
};

#endif
