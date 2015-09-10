using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace Visualiser
{
    class CircularEnt : SimEnt
    {
        public Point Center { get; set; }
        public double Radius { get; set; }

        public CircularEnt(UInt16 id, UInt32 weight, bool movable, Point center, double radius) : 
            base(id, SimEnt.CIRCLE_ID, weight, movable)
        {
            Radius = radius;
            Center = center;
        }

        public override void AddToCanvas(Canvas canvas)
        {
            Ellipse result = new Ellipse();

            result.Width = Radius*2;
            result.Height = Radius*2;
            result.Stroke = System.Windows.Media.Brushes.Black; /* TODO: Color information should be sent by server */

            double bottLeftX = HorFunc(Center.X - Radius);
            double bottLeftY = VertFunc(Center.Y + Radius);

            Canvas.SetLeft(result, bottLeftX);
            Canvas.SetTop(result, bottLeftY);
            canvas.Children.Add(result);
        }
    }
}
