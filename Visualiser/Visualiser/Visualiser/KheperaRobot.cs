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
        public float DirectionAngle { get; set; }
        public List<Sensor> Sensors { get; set; }

        public KheperaRobot(UInt16 id, UInt32 weight, bool movable, double x, 
            double y, double radius, UInt16 wheelRadius, UInt16 wheelDistance, float directionAngle) : 
            base(id, weight, movable, x, y, radius)
        {
            WheelRadius = wheelRadius;
            WheelDistance = wheelDistance;
            DirectionAngle = directionAngle;
            ShapeID = SimEnt.KHEPERA_ROBOT_ID;
            Sensors = new List<Sensor>();
        }

        public void AddSensor(Sensor sensor)
        {
            Sensors.Add(sensor);
        }

        public override void AddToCanvas(Canvas canvas)
        {
            base.AddToCanvas(canvas);
            double shiftedX = HorFunc(X);
            double shiftedY = VertFunc(Y);

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

            foreach(Sensor sensor in Sensors)
            {
                Line point = new Line()
                {
                    X1 = HorFunc(X + Math.Cos(DirectionAngle - sensor.PlacingAngle - 1 * Math.PI / 20) * (Radius - 3)),
                    Y1 = VertFunc(Y + Math.Sin(DirectionAngle - sensor.PlacingAngle - 1 * Math.PI / 20) * (Radius - 3)),
                    X2 = HorFunc(X + Math.Cos(DirectionAngle - sensor.PlacingAngle + 1 * Math.PI / 20) * (Radius - 3)),
                    Y2 = VertFunc(Y + Math.Sin(DirectionAngle - sensor.PlacingAngle + 1 * Math.PI / 20) * (Radius - 3)),
                    Stroke = sensor.State == 0 ? System.Windows.Media.Brushes.Red : System.Windows.Media.Brushes.Green,
                    StrokeThickness = 2
                };
                System.Windows.Media.DoubleCollection dashes = new System.Windows.Media.DoubleCollection();
                dashes.Add(2);
                dashes.Add(2);
                double begX = X + Math.Cos(DirectionAngle - sensor.PlacingAngle) * (Radius + 1);
                double begY = Y + Math.Sin(DirectionAngle - sensor.PlacingAngle) * (Radius + 1);
                Line left = new Line()
                {
                    X1 = HorFunc(begX),
                    Y1 = VertFunc(begY),
                    X2 = HorFunc(begX + Math.Cos(DirectionAngle - sensor.PlacingAngle + sensor.RangeAngle / 2)
                            * sensor.Range),
                    Y2 = VertFunc(begY + Math.Sin(DirectionAngle - sensor.PlacingAngle + sensor.RangeAngle / 2)
                            * sensor.Range),
                    Stroke = System.Windows.Media.Brushes.Black,
                    StrokeDashArray = dashes
                };
                Line center = new Line()
                {
                    X1 = HorFunc(begX),
                    Y1 = VertFunc(begY),
                    X2 = HorFunc(begX + Math.Cos(DirectionAngle - sensor.PlacingAngle)
                            * sensor.Range),
                    Y2 = VertFunc(begY + Math.Sin(DirectionAngle - sensor.PlacingAngle)
                            * sensor.Range),
                    Stroke = System.Windows.Media.Brushes.Black,
                    StrokeDashArray = dashes
                };
                Line right = new Line()
                {
                    X1 = HorFunc(begX),
                    Y1 = VertFunc(begY),
                    X2 = HorFunc(begX + Math.Cos(DirectionAngle - sensor.PlacingAngle - sensor.RangeAngle / 2)
                            * sensor.Range),
                    Y2 = VertFunc(begY + Math.Sin(DirectionAngle - sensor.PlacingAngle - sensor.RangeAngle / 2)
                            * sensor.Range),
                    Stroke = System.Windows.Media.Brushes.Black,
                    StrokeDashArray = dashes
                };
                Line connector = new Line()
                {
                    X1 = left.X2,
                    Y1 = left.Y2,
                    X2 = right.X2,
                    Y2 = right.Y2,
                    Stroke = System.Windows.Media.Brushes.Black,
                    StrokeDashArray = dashes

                };
                canvas.Children.Add(point);
                canvas.Children.Add(left);
                canvas.Children.Add(right);
                canvas.Children.Add(center);
                canvas.Children.Add(connector);
            }
        }
    }
}
