using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Visualiser
{
    class CircularEnt : SymEnt
    {
        public UInt32 X { get; set; }
        public UInt32 Y { get; set; }
        public UInt32 Radius { get; set; }

        public CircularEnt(UInt16 id, UInt32 weight, bool movable, UInt32 x, UInt32 y, UInt32 radius) : 
            base(id, SymEnt.CIRCLE_ID, weight, movable)
        {
            X = x;
            Y = y;
            Radius = radius;
        }
    }
}
