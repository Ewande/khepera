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
        public static SymEnt ReadNext(BinaryReader reader)
        {
            SymEnt result = null;

            // read common values for all entities
            byte shapeID = reader.ReadByte();
            UInt16 entityID = (UInt16) IPAddress.NetworkToHostOrder(reader.ReadInt16());
            bool movable = (reader.ReadByte() == 1);
            UInt32 weight = (UInt32) IPAddress.NetworkToHostOrder(reader.ReadInt32());

            switch (shapeID)
            {
                case SymEnt.CIRCLE_ID: result = ReadCircle(reader, entityID, movable, weight); break;
                case SymEnt.RECTANGLE_ID: result = ReadRectangle(reader, entityID, movable, weight); break;
                case SymEnt.KHEPERA_ROBOT_ID: result = ReadKheperaRobot(reader, entityID, movable, weight); break;
                default: System.Windows.MessageBox.Show("Unknown ShapeID"); break;
            }

            return result;
        }

        private static CircularEnt ReadCircle(BinaryReader reader, UInt16 entityID,
            bool movable, UInt32 weight)
        {
            UInt32 x = (UInt32)IPAddress.NetworkToHostOrder(reader.ReadInt32());
            UInt32 y = (UInt32)IPAddress.NetworkToHostOrder(reader.ReadInt32());
            UInt32 radius = (UInt32)IPAddress.NetworkToHostOrder(reader.ReadInt32());

            return new CircularEnt(entityID, weight, movable, x, y, radius);
        }

        private static RectangularEnt ReadRectangle(BinaryReader reader, UInt16 entityID,
            bool movable, UInt32 weight)
        {
            UInt32 x = (UInt32)IPAddress.NetworkToHostOrder(reader.ReadInt32());
            UInt32 y = (UInt32)IPAddress.NetworkToHostOrder(reader.ReadInt32());
            UInt32 width = (UInt32)IPAddress.NetworkToHostOrder(reader.ReadInt32());
            UInt32 height = (UInt32)IPAddress.NetworkToHostOrder(reader.ReadInt32());

            return new RectangularEnt(entityID, weight, movable, x, y, width, height);
        }

        private static KheperaRobot ReadKheperaRobot(BinaryReader reader, UInt16 entityID,
            bool movable, UInt32 weight)
        {
            // KheperaRobot contains all fields, that CircularEnt contains, so to not repeat code, we use this:
            CircularEnt circularRobotPart = ReadCircle(reader, entityID, movable, weight);

            UInt16 wheelRadius = (UInt16)IPAddress.NetworkToHostOrder(reader.ReadInt16());
            UInt16 wheelDistance = (UInt16)IPAddress.NetworkToHostOrder(reader.ReadInt16());
            SByte directionAngle = (SByte)reader.ReadSByte();

            return new KheperaRobot(entityID, weight, movable, circularRobotPart.X, circularRobotPart.Y,
                circularRobotPart.Radius, wheelRadius, wheelDistance, directionAngle);
        }
    }
}
