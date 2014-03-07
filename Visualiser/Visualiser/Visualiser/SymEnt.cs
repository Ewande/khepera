using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;

namespace Visualiser
{
    abstract class SymEnt
    {
        public static byte RECTANGLE_ID = 0;
        public static byte CIRCLE_ID = 1;

        public UInt16 ID { get; private set; }
        public byte ShapeID { get; private set; }
        public UInt32 Weight { get; private set; }
        public bool Movable { get; private set; }

        public SymEnt(UInt16 id, byte shapeID, UInt32 weight, bool movable)
        {
            ID = id;
            ShapeID = shapeID;
            Weight = weight;
            Movable = movable;
        }

        public abstract void AddToCanvas(Canvas canvas); // Adds this object to canvas (System.Windows.Controls.Canvas)
    }
}
