using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace MapEditor
{
    class CircularEnt : SymEnt
    {
        public double X { get; set; }
        public double Y { get; set; }
        public double Radius { get; set; }

        public CircularEnt(UInt16 id, UInt32 weight, bool movable, double x, double y, double radius) :
            base(id, SymEnt.CIRCLE_ID, weight, movable)
        {
            Radius = radius;
            X = x;
            Y = y;
        }

        public override void AddToCanvas(Canvas canvas)
        {
            Ellipse result = new Ellipse();

            result.Width = Radius * 2;
            result.Height = Radius * 2;
            result.Stroke = System.Windows.Media.Brushes.Black; /* TODO: Color information should be sent by server */

            Canvas.SetLeft(result, X - Radius); /* TODO: Check if those are really center coordinates, not upper-right corner coordinates */
            Canvas.SetTop(result, Y - Radius);

            canvas.Children.Add(result);
        }

        public override void SaveToFile(BinaryWriter writer)
        {
            writer.Write(ShapeID);
            writer.Write(ID);
            writer.Write((byte)(Movable ? 1 : 0));
            writer.Write(Weight);

            writer.Write(X);
            writer.Write(Y);
            writer.Write(Radius);
        }
    }

}
