using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Shapes;

namespace Visualiser
{
    class RectangularEnt : SymEnt
    {
        public UInt32 X1 { get; set; }
        public UInt32 Y1 { get; set; }

        public UInt32 X2 { get; set; }
        public UInt32 Y2 { get; set; }

        public UInt32 X3 { get; set; }
        public UInt32 Y3 { get; set; }

        public UInt32 X4 { get; set; }
        public UInt32 Y4 { get; set; }

        public RectangularEnt(UInt16 id, UInt32 weight, bool movable, UInt32 x1,
            UInt32 y1, UInt32 x2, UInt32 y2, UInt32 x3, UInt32 y3, UInt32 x4, 
            UInt32 y4) : base(id, SymEnt.CIRCLE_ID, weight, movable)
        {
            X1 = x1;
            Y1 = y1;

            X2 = x2;
            Y2 = y2;

            X3 = x3;
            Y3 = y3;

            X4 = x4;
            Y4 = y4;
        }

        public override void AddToCanvas(Canvas canvas)
        {
            Line line1 = new Line() { 
                X1 = this.X1,
                Y1 = this.Y1,
                X2 = this.X2,
                Y2 = this.Y2,
                Stroke = System.Windows.Media.Brushes.Black
            };

            Line line2 = new Line()
            {
                X1 = this.X2,
                Y1 = this.Y2,
                X2 = this.X3,
                Y2 = this.Y3,
                Stroke = System.Windows.Media.Brushes.Black
            };

            Line line3 = new Line() { 
                X1 = this.X3,
                Y1 = this.Y3,
                X2 = this.X4,
                Y2 = this.Y4,
                Stroke = System.Windows.Media.Brushes.Black
            };

            Line line4 = new Line() { 
                X1 = this.X4,
                Y1 = this.Y4,
                X2 = this.X1,
                Y2 = this.Y1,
                Stroke = System.Windows.Media.Brushes.Black
            };

            canvas.Children.Add(line1);
            canvas.Children.Add(line2);
            canvas.Children.Add(line3);
            canvas.Children.Add(line4);
        }
    }
}
