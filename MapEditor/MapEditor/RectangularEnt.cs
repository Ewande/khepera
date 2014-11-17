using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace MapEditor
{
    class RectangularEnt : SymEnt
    {
        private Point[] _vertices;

        public RectangularEnt(UInt16 id, UInt32 weight, bool movable, double x1,
            double y1, double x2, double y2, double x3, double y3, double x4,
            double y4)
            : this(id, weight, movable, new Point(x1, y1), new Point(x2, y2),
                new Point(x3, y3), new Point(x4, y4))
        {
        }

        public RectangularEnt(UInt16 id, UInt32 weight, bool movable,
            Point p1, Point p2, Point p3, Point p4) :
            base(id, SymEnt.RECTANGLE_ID, weight, movable)
        {
            _vertices = new Point[4];

            _vertices[0] = p1;
            _vertices[1] = p2;
            _vertices[2] = p3;
            _vertices[3] = p4;
        }

        public override void AddToCanvas(Canvas canvas)
        {
            Line line1 = new Line()
            {
                X1 = this._vertices[0].X,
                Y1 = this._vertices[0].Y,
                X2 = this._vertices[1].X,
                Y2 = this._vertices[1].Y,
                Stroke = System.Windows.Media.Brushes.Black
            };

            Line line2 = new Line()
            {
                X1 = this._vertices[1].X,
                Y1 = this._vertices[1].Y,
                X2 = this._vertices[2].X,
                Y2 = this._vertices[2].Y,
                Stroke = System.Windows.Media.Brushes.Black
            };

            Line line3 = new Line()
            {
                X1 = this._vertices[2].X,
                Y1 = this._vertices[2].Y,
                X2 = this._vertices[3].X,
                Y2 = this._vertices[3].Y,
                Stroke = System.Windows.Media.Brushes.Black
            };

            Line line4 = new Line()
            {
                X1 = this._vertices[3].X,
                Y1 = this._vertices[3].Y,
                X2 = this._vertices[0].X,
                Y2 = this._vertices[0].Y,
                Stroke = System.Windows.Media.Brushes.Black
            };

            canvas.Children.Add(line1);
            canvas.Children.Add(line2);
            canvas.Children.Add(line3);
            canvas.Children.Add(line4);
        }

        public override void SaveToFile(BinaryWriter writer)
        {
            writer.Write(ShapeID);
            writer.Write(ID);
            writer.Write((byte) (Movable ? 1 : 0));
            writer.Write(Weight);

            for (int i = 0; i < 4; i++)
            {
                writer.Write(_vertices[i].X);
                writer.Write(_vertices[i].Y);
            }
        }
    }
}
