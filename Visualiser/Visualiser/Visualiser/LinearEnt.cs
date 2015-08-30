using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace Visualiser
{
    class LinearEnt : SimEnt
    {
        private Point _beg, _end;

        public LinearEnt(UInt16 id, double begX, double begY, double endX, double endY)
            : this(id, new Point(begX, begY), new Point(endX, endY))
        {
        }

        public LinearEnt(UInt16 id, Point beg, Point end) :
            base(id, SimEnt.CIRCLE_ID, SimulationWorld.INFINITY, false)
        {
            _beg = beg;
            _end = end;
        }

        public override void AddToCanvas(Canvas canvas)
        {
            Line line = new Line()
            {
                X1 = HorFunc(_beg.X),
                Y1 = VertFunc(_beg.Y),
                X2 = HorFunc(_end.X),
                Y2 = VertFunc(_end.Y),
                Stroke = System.Windows.Media.Brushes.Black
            };
            canvas.Children.Add(line);
        }
    }
}
