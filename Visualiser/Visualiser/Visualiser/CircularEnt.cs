using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Shapes;

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

        public override void AddToCanvas(Canvas canvas)
        {
            Ellipse result = new Ellipse();

            result.Width = Radius;
            result.Height = Radius;
            result.Stroke = System.Windows.Media.Brushes.Black; /* TODO: Color information should be sent by server */

            Canvas.SetLeft(result, X); /* TODO: Check if those are really center coordinates, not upper-right corner coordinates */
            Canvas.SetTop(result, Y);

            canvas.Children.Add(result);
        }
    }
}
