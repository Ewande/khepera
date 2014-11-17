using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace Visualiser
{
    class LinearEnt : SymEnt
    {
        private Point _beg, _end;

        public LinearEnt(UInt16 id, double begX, double begY, double endX, double endY)
            : this(id, new Point(begX, begY), new Point(endX, endY))
        {
        }

        public LinearEnt(UInt16 id, Point beg, Point end) :
            base(id, SymEnt.CIRCLE_ID, SymulationWorld.INFINITY, false)
        {
            _beg = beg;
            _end = end;
        }

        public override void AddToCanvas(Canvas canvas)
        {
            Line line = new Line()
            {
                X1 = HorShift == 0 ? _beg.X : HorShift - _beg.X,
                Y1 = VertShift == 0 ? _beg.Y : VertShift - _beg.Y,
                X2 = HorShift == 0 ? _end.X : HorShift - _end.X,
                Y2 = VertShift == 0 ? _end.Y : VertShift - _end.Y,
                Stroke = System.Windows.Media.Brushes.Black
            };
            canvas.Children.Add(line);
        }
    }
}
