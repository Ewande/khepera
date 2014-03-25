#include "RobotMotorSpeedChangeCommand.h"

RobotMotorSpeedChangeCommand::RobotMotorSpeedChangeCommand() : 
	ClientCommand(ClientCommand::ROBOT_MOTOR_SPEED_CHANGE_COMMAND_ID)
{
}

RobotMotorSpeedChangeCommand::~RobotMotorSpeedChangeCommand()
{
}

uint16_t RobotMotorSpeedChangeCommand::Execute(Symulation* sym, SOCKET sock)
{
	uint16_t robotID;
	uint8_t  motorID;
	int16_t  newSpeed;

	recv(sock, reinterpret_cast<char*>(&robotID), sizeof(robotID), 0);
	robotID = ntohs(robotID);

	recv(sock, reinterpret_cast<char*>(&motorID), sizeof(motorID), 0);

	recv(sock, reinterpret_cast<char*>(&newSpeed), sizeof(newSpeed), 0);
	newSpeed = ntohs(newSpeed);

	SymEnt* entity = sym->GetEntity(robotID);
	if (entity != NULL)
	{
		KheperaRobot* robot = dynamic_cast<KheperaRobot*>(entity);
		if (robot != NULL)
		{
			// we will be changing speed, so nobody othere should write or read at the same time
			sym->Lock(); 
			switch (motorID)
			{
				case LEFT_MOTOR_ID: robot->SetLeftMotorSpeed(newSpeed); break;
				case RIGHT_MOTOR_ID: robot->SetRightMotorSpeed(newSpeed); break;
				default: sym->Unlock(); return ClientCommand::ERROR_CODE_INVALID_MOTOR_ID;
			}
			sym->Unlock();
		}
		else
			return ClientCommand::ERROR_CODE_INVALID_ENTITY_ID;
	}
	else
		return ClientCommand::ERROR_CODE_ENTITY_DOESNT_EXIST;

	return ClientCommand::ERROR_CODE_SUCCESS;
}
