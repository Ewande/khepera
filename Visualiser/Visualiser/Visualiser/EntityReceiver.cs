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
    class EntityReceiver
    {
        // Shapes IDs definitions, must be the same as in SymEnt.h !!
        public const byte RECTANGLE = 0;
        public const byte CIRCLE = 1;

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
                case CIRCLE: result = ReadCircle(reader, entityID, movable, weight); break;
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
    }
}
