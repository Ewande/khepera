using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Visualiser
{
    class RectangularEnt : SymEnt
    {
        public UInt32 X { get; set; }
        public UInt32 Y { get; set; }
        public UInt32 Width { get; set; }
        public UInt32 Height {get; set; }

        public RectangularEnt(UInt16 id, UInt32 weight, bool movable, UInt32 x,
            UInt32 y, UInt32 width, UInt32 height) : base(id, SymEnt.CIRCLE_ID, weight, movable)
        {
            X = x;
            Y = y;
            Width = width;
            Height = height;
        }
    }
}
