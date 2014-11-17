using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace Visualiser
{
    class KheperaRobot : CircularEnt
    {
        public UInt16 WheelRadius { get; set;}
        public UInt16 WheelDistance { get; set; }
        public double DirectionAngle { get; set; }

        public KheperaRobot(UInt16 id, UInt32 weight, bool movable, double x, 
            double y, double radius, UInt16 wheelRadius, UInt16 wheelDistance, double directionAngle) : 
            base(id, weight, movable, x, y, radius)
        {
            WheelRadius = wheelRadius;
            WheelDistance = wheelDistance;
            DirectionAngle = directionAngle;
            ShapeID = SymEnt.KHEPERA_ROBOT_ID;
        }

        public override void AddToCanvas(Canvas canvas)
        {
            Ellipse result = new Ellipse();

            result.Width = Radius * 2;
            result.Height = Radius * 2;
            result.Stroke = System.Windows.Media.Brushes.Black; /* TODO: Color information should be sent by server */

            double bottLeftX = HorShift == 0 ? X - Radius : HorShift - X + Radius;
            double bottLeftY = VertShift == 0 ? Y - Radius : VertShift - Y - Radius;

            Canvas.SetLeft(result, bottLeftX);
            Canvas.SetTop(result, bottLeftY);

            canvas.Children.Add(result);

            // draw wheels directed in robot heading direction 

            // line comming through center of robot at angle of DirectionAngle
            double a = Math.Tan(DirectionAngle);
            double b = Y - a * X;
        }
    }
}
