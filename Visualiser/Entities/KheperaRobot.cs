using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;
using Visualiser.Entities.Sensors;

namespace Visualiser.Entities
{
    class KheperaRobot : CircularEnt
    {
        public UInt16 WheelRadius { get; set;}
        public UInt16 WheelDistance { get; set; }
        public float DirectionAngle { get; set; }
        public List<Sensor> Sensors { get; set; }

        public KheperaRobot(UInt16 id, UInt32 weight, bool movable, Point center, double radius, 
            UInt16 wheelRadius, UInt16 wheelDistance, float directionAngle) : 
            base(id, weight, movable, center, radius)
        {
            WheelRadius = wheelRadius;
            WheelDistance = wheelDistance;
            DirectionAngle = directionAngle;
            ShapeID = KHEPERA_ROBOT_ID;
            Sensors = new List<Sensor>();
        }

        public void AddSensor(Sensor sensor)
        {
            Sensors.Add(sensor);
        }

        public override void AddToCanvas(Canvas canvas)
        {
            base.AddToCanvas(canvas);
            double shiftedX = HorFunc(Center.X);
            double shiftedY = VertFunc(Center.Y);

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
                Stroke = Brushes.Black
            };
            Line leftWheel = new Line()
            {
                X1 = leftWheelX + wheelVectorX,
                Y1 = leftWheelY - wheelVectorY,
                X2 = leftWheelX - wheelVectorX,
                Y2 = leftWheelY + wheelVectorY,
                Stroke = Brushes.Black
            };

            Line rightWheel = new Line()
            {
                X1 = rightWheelX + wheelVectorX,
                Y1 = rightWheelY - wheelVectorY,
                X2 = rightWheelX - wheelVectorX,
                Y2 = rightWheelY + wheelVectorY,
                Stroke = Brushes.Black
            };
            canvas.Children.Add(leftWheel);
            canvas.Children.Add(rightWheel);

            foreach(Sensor sensor in Sensors)
            {
                Line point = new Line()
                {
                    X1 = HorFunc(Center.X + Math.Cos(DirectionAngle - sensor.PlacingAngle - 1 * Math.PI / 20) * (Radius - 3)),
                    Y1 = VertFunc(Center.Y + Math.Sin(DirectionAngle - sensor.PlacingAngle - 1 * Math.PI / 20) * (Radius - 3)),
                    X2 = HorFunc(Center.X + Math.Cos(DirectionAngle - sensor.PlacingAngle + 1 * Math.PI / 20) * (Radius - 3)),
                    Y2 = VertFunc(Center.Y + Math.Sin(DirectionAngle - sensor.PlacingAngle + 1 * Math.PI / 20) * (Radius - 3)),
                    Stroke = sensor.State == 0 ? Brushes.Red : Brushes.Green,
                    StrokeThickness = 2
                };
                canvas.Children.Add(point);

                if (DisplayConfig.Instance.ShowSensorRange)
                {
                    DoubleCollection dashes = new DoubleCollection(new double[] { 2, 2 });
                    double begX = Center.X + Math.Cos(DirectionAngle - sensor.PlacingAngle) * (Radius + 1);
                    double begY = Center.Y + Math.Sin(DirectionAngle - sensor.PlacingAngle) * (Radius + 1);
                    Line left = new Line()
                    {
                        X1 = HorFunc(begX), Y1 = VertFunc(begY),
                        X2 = HorFunc(begX + Math.Cos(DirectionAngle - sensor.PlacingAngle + sensor.RangeAngle / 2)
                                * sensor.Range),
                        Y2 = VertFunc(begY + Math.Sin(DirectionAngle - sensor.PlacingAngle + sensor.RangeAngle / 2)
                                * sensor.Range),
                        Stroke = Brushes.Black, StrokeDashArray = dashes
                    };
                    Line center = new Line()
                    {
                        X1 = HorFunc(begX), Y1 = VertFunc(begY),
                        X2 = HorFunc(begX + Math.Cos(DirectionAngle - sensor.PlacingAngle)
                                * sensor.Range),
                        Y2 = VertFunc(begY + Math.Sin(DirectionAngle - sensor.PlacingAngle)
                                * sensor.Range),
                        Stroke = Brushes.Black, StrokeDashArray = dashes
                    };
                    Line right = new Line()
                    {
                        X1 = HorFunc(begX), Y1 = VertFunc(begY),
                        X2 = HorFunc(begX + Math.Cos(DirectionAngle - sensor.PlacingAngle - sensor.RangeAngle / 2)
                                * sensor.Range),
                        Y2 = VertFunc(begY + Math.Sin(DirectionAngle - sensor.PlacingAngle - sensor.RangeAngle / 2)
                                * sensor.Range),
                        Stroke = Brushes.Black, StrokeDashArray = dashes
                    };

                    PathGeometry pathGeometry = new PathGeometry();
                    PathFigure figure = new PathFigure() { StartPoint = new Point(left.X2, left.Y2) };
                    figure.Segments.Add(new ArcSegment(new Point(right.X2, right.Y2), new Size(sensor.Range, sensor.Range), sensor.RangeAngle, false, SweepDirection.Clockwise, true));
                    pathGeometry.Figures.Add(figure);
                    Path path = new Path() { Data = pathGeometry, Stroke = Brushes.Black, StrokeDashArray = dashes };

                    canvas.Children.Add(path);
                    canvas.Children.Add(left);
                    canvas.Children.Add(right);
                }
            }

            if (DisplayConfig.Instance.ShowId)
            {
                TextBlock idText = new TextBlock
                {
                    Text = ID.ToString(),
                    TextAlignment = System.Windows.TextAlignment.Center,
                    Width = 2 * Radius,
                    Height = 2 * Radius,
                    FontFamily = new System.Windows.Media.FontFamily("Courier New"),
                    FontWeight = System.Windows.FontWeights.Normal,
                    FontSize = 15,
                    SnapsToDevicePixels = true
                };
                Canvas.SetLeft(idText, HorFunc(Center.X - Radius));
                Canvas.SetTop(idText, VertFunc(Center.Y + Radius / 2));
                canvas.Children.Add(idText);
            }
            else
                canvas.Children.Add(wheelAxis);
        }
    }
}
