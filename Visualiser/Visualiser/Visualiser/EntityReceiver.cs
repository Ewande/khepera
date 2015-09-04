﻿using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace Visualiser
{

    // some kind of SymEnt factory
    class EntityReceiver
    {
        public static SimEnt ReadNext(BinaryReader reader)
        {
            SimEnt result = null;

            // read common values for all entities
            byte shapeID = reader.ReadByte();
            UInt16 entityID = (UInt16) IPAddress.NetworkToHostOrder(reader.ReadInt16());
            bool movable = (reader.ReadByte() == 1);
            UInt32 weight = (UInt32) IPAddress.NetworkToHostOrder(reader.ReadInt32());

            switch (shapeID)
            {
                case SimEnt.CIRCLE_ID: result = ReadCircle(reader, entityID, movable, weight); break;
                case SimEnt.RECTANGLE_ID: result = ReadRectangle(reader, entityID, movable, weight); break;
                case SimEnt.KHEPERA_ROBOT_ID: result = ReadKheperaRobot(reader, entityID, movable, weight); break;
                case SimEnt.LINE_ID: result = ReadLinearEnt(reader, entityID); break;
                default: System.Windows.MessageBox.Show("Unknown ShapeID"); break;
            }

            return result;
        }

        private static CircularEnt ReadCircle(BinaryReader reader, UInt16 entityID,
            bool movable, UInt32 weight)
        {
            double x = reader.ReadDouble();
            double y = reader.ReadDouble();
            double radius = reader.ReadDouble();

            return new CircularEnt(entityID, weight, movable, x, y, radius);
        }

        private static RectangularEnt ReadRectangle(BinaryReader reader, UInt16 entityID,
            bool movable, UInt32 weight)
        {
            double x1 = reader.ReadDouble();
            double y1 = reader.ReadDouble();

            double x2 = reader.ReadDouble();
            double y2 = reader.ReadDouble();

            double x3 = reader.ReadDouble();
            double y3 = reader.ReadDouble();

            double x4 = reader.ReadDouble();
            double y4 = reader.ReadDouble();

            return new RectangularEnt(entityID, weight, movable, x1, y1, x2, y2,
                x3, y3, x4, y4);
        }

        private static KheperaRobot ReadKheperaRobot(BinaryReader reader, UInt16 entityID, bool movable, UInt32 weight)
        {
            // KheperaRobot contains all fields, that CircularEnt contains, so to not repeat code, we use this:
            CircularEnt circularRobotPart = ReadCircle(reader, entityID, movable, weight);

            UInt16 wheelRadius = (UInt16)IPAddress.NetworkToHostOrder(reader.ReadInt16());
            UInt16 wheelDistance = (UInt16)IPAddress.NetworkToHostOrder(reader.ReadInt16());
            float directionAngle = reader.ReadSingle();

            KheperaRobot robot =  new KheperaRobot(entityID, weight, movable, circularRobotPart.X, circularRobotPart.Y,
                circularRobotPart.Radius, wheelRadius, wheelDistance, directionAngle);
            UInt16 sensorsCount = (UInt16)IPAddress.NetworkToHostOrder(reader.ReadInt16());
            for (int i = 0; i < sensorsCount; i++)
            {
                Sensor sensor = new Sensor();
                sensor.PlacingAngle = reader.ReadSingle();
                sensor.RangeAngle = reader.ReadSingle();
                sensor.Range = reader.ReadDouble();
                sensor.State = reader.ReadSingle();
                robot.AddSensor(sensor);
            }

            return robot;
        }

        private static LinearEnt ReadLinearEnt(BinaryReader reader, UInt16 entityID)
        {
            double begX = reader.ReadDouble();
            double begY = reader.ReadDouble();

            double endX = reader.ReadDouble();
            double endY = reader.ReadDouble();

            return new LinearEnt(entityID, begX, begY, endX, endY);
        }
    }
}
