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

        /* TODO: Change drawing procedure, so that user will be able to distinguish betwin robots and static circles */
        public override void AddToCanvas(Canvas canvas)
        {
            base.AddToCanvas(canvas);

            double x = Math.Cos(DirectionAngle) * Radius;
            double y = Math.Sin(DirectionAngle) * Radius;

            /* scale vector, to be the same length, as half of wheel distance */
            double prop = Radius / (WheelDistance * 2);
            x /= prop;
            y /= prop;
            
            double leftWheelX = X - y;
            double leftWheelY = Y - x;

            double rightWheelX = X + y;
            double rightWheelY = Y + x ;

            double proportion = WheelDistance / WheelRadius;

            double wheelVectorX = x / proportion;
            double wheelVectorY = y / proportion;

            Line wheelAxis = new Line()
            {
                X1 = leftWheelX,
                Y1 = leftWheelY,
                X2 = rightWheelX,
                Y2 = rightWheelY,
                Stroke = System.Windows.Media.Brushes.Black
            };
            Line leftWheel = new Line()
            {
                X1 = leftWheelX + wheelVectorX,
                Y1 = leftWheelY - wheelVectorY,
                X2 = leftWheelX - wheelVectorX,
                Y2 = leftWheelY + wheelVectorY,
                Stroke = System.Windows.Media.Brushes.Black
            };

            Line rightWheel = new Line()
            {
                X1 = rightWheelX + wheelVectorX,
                Y1 = rightWheelY - wheelVectorY,
                X2 = rightWheelX - wheelVectorX,
                Y2 = rightWheelY + wheelVectorY,
                Stroke = System.Windows.Media.Brushes.Black
            };
            canvas.Children.Add(wheelAxis);
            canvas.Children.Add(leftWheel);
            canvas.Children.Add(rightWheel);
        }
    }
}
