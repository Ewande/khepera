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
        public UInt32 X { get; set; }
        public UInt32 Y { get; set; }
        public UInt32 Width { get; set; }
        public UInt32 Height {get; set; }

        public RectangularEnt(UInt16 id, UInt32 weight, bool movable, UInt32 x,
            UInt32 y, UInt32 width, UInt32 height) : base(id, SymEnt.CIRCLE_ID, weight, movable)
        {
            X = x;
            Y = y;
            Width = width;
            Height = height;
        }

        public override void AddToCanvas(Canvas canvas)
        {
            Rectangle result = new Rectangle();

            result.Width = Width;
            result.Height = Height;
            result.Stroke = System.Windows.Media.Brushes.Black; /* TODO: Color information should be sent by server */

            Canvas.SetLeft(result, X); 
            Canvas.SetTop(result, Y);

            canvas.Children.Add(result);
        }
    }
}
