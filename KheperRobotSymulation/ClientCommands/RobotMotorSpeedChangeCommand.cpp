#include "RobotMotorSpeedChangeCommand.h"

RobotMotorSpeedChangeCommand::RobotMotorSpeedChangeCommand() : 
	ClientCommand(ClientCommand::ROBOT_MOTOR_SPEED_CHANGE_COMMAND_ID)
{
}

RobotMotorSpeedChangeCommand::~RobotMotorSpeedChangeCommand()
{
}

uint16_t RobotMotorSpeedChangeCommand::execute(Simulation* sim, SOCKET sock)
{
	uint16_t robotID;
	uint8_t  motorID;
	double  newSpeed;

	recv(sock, reinterpret_cast<char*>(&robotID), sizeof(robotID), 0);
	robotID = ntohs(robotID);

	recv(sock, reinterpret_cast<char*>(&motorID), sizeof(motorID), 0);

	recv(sock, reinterpret_cast<char*>(&newSpeed), sizeof(newSpeed), 0);

	SimEnt* entity = sim->getEntity(robotID);
	if (entity != NULL)
	{
		KheperaRobot* robot = dynamic_cast<KheperaRobot*>(entity);
		if (robot != NULL)
		{
			// we will be changing speed, so nobody othere should write or read at the same time
			sim->lock(); 
			switch (motorID)
			{
				case LEFT_MOTOR_ID: robot->setLeftMotorSpeed(newSpeed); break;
				case RIGHT_MOTOR_ID: robot->setRightMotorSpeed(newSpeed); break;
				default: sim->unlock(); return ClientCommand::ERROR_CODE_INVALID_MOTOR_ID;
			}
			sim->unlock();
		}
		else
			return ClientCommand::ERROR_CODE_INVALID_ENTITY_ID;
	}
	else
		return ClientCommand::ERROR_CODE_ENTITY_DOESNT_EXIST;

	return ClientCommand::ERROR_CODE_SUCCESS;
}
