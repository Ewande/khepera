using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Windows.Media;
using System.Windows.Shapes;

namespace MapEditor
{
    class SelectionRegion
    {
        private Line _line1;
        private Line _line2;
        private Line _line3;
        private Line _line4;

        public SelectionRegion(Canvas canvas)
        {
            DoubleCollection dashes = new DoubleCollection();
            dashes.Add(10);
            dashes.Add(10);

            _line1 = new Line()
            {
                Stroke = System.Windows.Media.Brushes.Black,
                StrokeDashArray = dashes,
                StrokeDashCap = PenLineCap.Round
            };

            _line2 = new Line()
            {
                Stroke = System.Windows.Media.Brushes.Black,
                StrokeDashArray = dashes,
                StrokeDashCap = PenLineCap.Round
            };

            _line3 = new Line()
            {
                Stroke = System.Windows.Media.Brushes.Black,
                StrokeDashArray = dashes,
                StrokeDashCap = PenLineCap.Round
            };

            _line4 = new Line()
            {
                Stroke = System.Windows.Media.Brushes.Black,
                StrokeDashArray = dashes,
                StrokeDashCap = PenLineCap.Round
            };

            Reset();

            canvas.Children.Add(_line1);
            canvas.Children.Add(_line2);
            canvas.Children.Add(_line3);
            canvas.Children.Add(_line4);
        }

        public void Reset()
        {
            _line1.X1 = 0;
            _line1.Y1 = 0;
            _line1.X2 = 0;
            _line1.Y2 = 0;

            _line2.X1 = 0;
            _line2.Y1 = 0;
            _line2.X2 = 0;
            _line2.Y2 = 0;

            _line3.X1 = 0;
            _line3.Y1 = 0;
            _line3.X2 = 0;
            _line3.Y2 = 0;

            _line4.X1 = 0;
            _line4.Y1 = 0;
            _line4.X2 = 0;
            _line4.Y2 = 0;
        }

        public void Update(int leftX, int rightX, int upperY, int bottomY)
        {
                _line1.X1 = leftX;
                _line1.Y1 = upperY;
                _line1.X2 = rightX;
                _line1.Y2 = upperY;

                _line2.X1 = rightX;
                _line2.Y1 = upperY;
                _line2.X2 = rightX;
                _line2.Y2 = bottomY;
            
                _line3.X1 = rightX;
                _line3.Y1 = bottomY;
                _line3.X2 = leftX;
                _line3.Y2 = bottomY;

                _line4.X1 = leftX;
                _line4.Y1 = bottomY;
                _line4.X2 = leftX;
                _line4.Y2 = upperY;
        }
    }
}
