#ifndef ROBOT_SPEED_CHANGE_COMMANDS_H
#define ROBOT_SPEED_CHANGE_COMMANDS_H

#include "ClientCommand.h"
#include "../Simulation/Entities/KheperaRobot.h"

/*
          Serialization format (integers in network-byte-order, doubles and floats in host-byte-order)

             +-------------------+----------------------------------------+-------------------+
             |                   |                                        |                   |
             |   COMMAND_ID      |                 ROBOT_ID               |     ENGINE_ID     |
			 |     1 byte        |                  2 bytes               |      1 byte       |
			 +--------------------------------------------------------------------------------+
			 |                                                                                |
			 |                                                                                |
			 |                             NEW_SPEED_VALUE                                    | 
			 |                                64 bites                                        |
			 |                                                                                |
			 |                                                                                |
			 +--------------------------------------------------------------------------------+
*/

class SingleMotorSpeedChangeCommand : public ClientCommand
{
	public:
        SingleMotorSpeedChangeCommand() : ClientCommand(ClientCommand::SINGLE_MOTOR_SPEED_CHANGE_COMMAND_ID) {}

        uint16_t execute(SimEnt& entity, DistrSimulation& sim, SOCKET sock);

	private:
        static const int LEFT_MOTOR_ID = 0;
        static const int RIGHT_MOTOR_ID = 1;
};

class MotorsSpeedChangeCommand : public ClientCommand
{
    public:
        MotorsSpeedChangeCommand() : ClientCommand(ClientCommand::MOTORS_SPEED_CHANGE_COMMAND_ID) {}

        uint16_t execute(SimEnt& entity, DistrSimulation& sim, SOCKET sock);
};

#endif

