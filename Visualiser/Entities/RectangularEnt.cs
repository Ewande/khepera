using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows;
using System.Windows.Shapes;

namespace Visualiser.Entities
{
    class RectangularEnt : SimEnt
    {
        private Point[]   _vertices;

        public RectangularEnt(UInt16 id, UInt32 weight, bool movable,  Point p1, Point p2, Point p3, Point p4) :
            base(id, RECTANGLE_ID, weight, movable)
        {
            _vertices = new Point[] { p1, p2, p3, p4 };
        }

        public override void AddToCanvas(Canvas canvas)
        {
            for (int i = 0; i < _vertices.Length; i++)
            {
                Line line = new Line()
                {
                    X1 = HorFunc(_vertices[i].X),
                    Y1 = VertFunc(_vertices[i].Y),
                    X2 = HorFunc(_vertices[(i + 1) % _vertices.Length].X),
                    Y2 = VertFunc(_vertices[(i + 1) % _vertices.Length].Y),
                    Stroke = System.Windows.Media.Brushes.Black,
                    SnapsToDevicePixels = true
                };
                canvas.Children.Add(line);
            }
        }
    }
}
