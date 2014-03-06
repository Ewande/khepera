using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Visualiser
{
    abstract class SymEnt
    {
        public static byte RECTANGLE_ID = 0;
        public static byte CIRCLE_ID = 1;

        public UInt16 _id { get; private set; }
        public byte _shapeID { get; private set; }
        public UInt32 _weight { get; private set; }
        public bool _movable { get; private set; }

        public SymEnt(UInt16 id, byte shapeID, UInt32 weight, bool movable)
        {
            _id = id;
            _shapeID = shapeID;
            _weight = weight;
            _movable = movable;
        }
    }
}
