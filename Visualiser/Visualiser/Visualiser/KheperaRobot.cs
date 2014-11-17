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
            base.AddToCanvas(canvas);
            double shiftedX = HorShift == 0 ? X : HorShift - X;
            double shiftedY = VertShift == 0 ? Y : VertShift - Y;

            // coordinates of vector, if we add this vector to tmpX and tmpY we have point on robot circle
            // line coming through this point and robot center goes at angle of DirectionAngle
            double directionVectorX = Math.Cos(DirectionAngle) * Radius;
            double directionVectorY = Math.Sin(DirectionAngle) * Radius;

            // scale vector, to be the same length, as half of wheel distance
            double prop = Radius / (WheelDistance * 2);
            directionVectorX *= prop;
            directionVectorY *= prop;

            // add vector perpendicular to directionVector going to the left
            double leftWheelX = shiftedX - directionVectorY;
            double leftWheelY = shiftedY - directionVectorX;

            // add vector perpendicular  to directionVector going to the right
            double rightWheelX = shiftedX + directionVectorY;
            double rightWheelY = shiftedY + directionVectorX;

            double proportion = WheelDistance / WheelRadius;

            double wheelVectorX = directionVectorX / proportion;
            double wheelVectorY = directionVectorY / proportion;

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
