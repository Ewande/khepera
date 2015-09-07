#include "RobotSpeedChangeCommands.h"

uint16_t SingleMotorSpeedChangeCommand::execute(SimEnt& entity, DistrSimulation& sim, SOCKET sock)
{
    uint16_t errorCode = ClientCommand::ERROR_CODE_SUCCESS;

    KheperaRobot* robot = dynamic_cast<KheperaRobot*>(&entity);
    if (robot == NULL)
        errorCode = ClientCommand::ERROR_CODE_INVALID_ENTITY;
    else
    {
        uint8_t motorID;
        double newSpeed;
        recv(sock, reinterpret_cast<char*>(&motorID), sizeof(motorID), 0);
        recv(sock, reinterpret_cast<char*>(&newSpeed), sizeof(newSpeed), 0);

        sim.lock();
        if (motorID == LEFT_MOTOR_ID)
            robot->setLeftMotorSpeed(newSpeed);
        else if (motorID == RIGHT_MOTOR_ID)
            robot->setRightMotorSpeed(newSpeed);
        else
            errorCode = ClientCommand::ERROR_CODE_INVALID_MOTOR_ID;
        sim.unlock();
    }

	return errorCode;
}

uint16_t MotorsSpeedChangeCommand::execute(SimEnt& entity, DistrSimulation& sim, SOCKET sock)
{
    uint16_t errorCode = ClientCommand::ERROR_CODE_SUCCESS;

    KheperaRobot* robot = dynamic_cast<KheperaRobot*>(&entity);
    if (robot == NULL)
        errorCode = ClientCommand::ERROR_CODE_INVALID_ENTITY;
    else
    {
        double newLeftSpeed;
        double newRightSpeed;
        recv(sock, reinterpret_cast<char*>(&newLeftSpeed), sizeof(newLeftSpeed), 0);
        recv(sock, reinterpret_cast<char*>(&newRightSpeed), sizeof(newRightSpeed), 0);

        sim.lock();
        robot->setLeftMotorSpeed(newLeftSpeed);
        robot->setRightMotorSpeed(newRightSpeed);
        sim.unlock();
    }

    return errorCode;
}
