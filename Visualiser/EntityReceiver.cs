using System;
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
                case SimEnt.CIRCLE_ID:
                    result = ReadCircle(reader, entityID, movable, weight);
                    break;
                case SimEnt.RECTANGLE_ID:
                    result = ReadRectangle(reader, entityID, movable, weight);
                    break;
                case SimEnt.KHEPERA_ROBOT_ID:
                    result = ReadKheperaRobot(reader, entityID, movable, weight);
                    break;
                case SimEnt.LINE_ID: 
                    result = ReadLinearEnt(reader, entityID); 
                    break;
                default: 
                    System.Windows.MessageBox.Show("Unknown shape ID"); 
                    break;
            }

            return result;
        }

        private static Point ReadPoint(BinaryReader reader)
        {
            double x = reader.ReadDouble();
            double y = reader.ReadDouble();
            return new Point(x, y);
        }

        private static CircularEnt ReadCircle(BinaryReader reader, UInt16 entityID,
            bool movable, UInt32 weight)
        {
            Point center = ReadPoint(reader);
            double radius = reader.ReadDouble();

            return new CircularEnt(entityID, weight, movable, center, radius);
        }

        private static RectangularEnt ReadRectangle(BinaryReader reader, UInt16 entityID,
            bool movable, UInt32 weight)
        {
            Point p1 = ReadPoint(reader);
            Point p2 = ReadPoint(reader);
            Point p3 = ReadPoint(reader);
            Point p4 = ReadPoint(reader);

            return new RectangularEnt(entityID, weight, movable, p1, p2, p3 , p4);
        }

        private static KheperaRobot ReadKheperaRobot(BinaryReader reader, UInt16 entityID, bool movable, UInt32 weight)
        {
            // KheperaRobot contains all fields, that CircularEnt contains, so to not repeat code, we use this:
            CircularEnt circularRobotPart = ReadCircle(reader, entityID, movable, weight);

            UInt16 wheelRadius = (UInt16) IPAddress.NetworkToHostOrder(reader.ReadInt16());
            UInt16 wheelDistance = (UInt16) IPAddress.NetworkToHostOrder(reader.ReadInt16());
            float directionAngle = reader.ReadSingle();

            KheperaRobot robot =  new KheperaRobot(entityID, weight, movable, circularRobotPart.Center,
                circularRobotPart.Radius, wheelRadius, wheelDistance, directionAngle);
            UInt16 sensorsCount = (UInt16) IPAddress.NetworkToHostOrder(reader.ReadInt16());
            for (int i = 0; i < sensorsCount; i++)
            {
                Sensor sensor = new Sensor();
                byte type = reader.ReadByte(); // not used for now, only one type of sensor exists
                sensor.Range = reader.ReadDouble();
                sensor.RangeAngle = reader.ReadSingle();
                sensor.PlacingAngle = reader.ReadSingle();
                sensor.State = reader.ReadSingle();
                robot.AddSensor(sensor);
            }

            return robot;
        }

        private static LinearEnt ReadLinearEnt(BinaryReader reader, UInt16 entityID)
        {
            Point beg = ReadPoint(reader);
            Point end = ReadPoint(reader);

            return new LinearEnt(entityID, beg, end);
        }
    }
}
