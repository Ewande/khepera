using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace MapEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Point _startPoint;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void OnMouseDown(object sender, MouseButtonEventArgs e)
        {
            _startPoint = Mouse.GetPosition(canvas);
        }

        private void OnMouseUp(object sender, MouseButtonEventArgs e)
        {
            Point endPoint = Mouse.GetPosition(canvas);

            int leftX = Math.Min((int)_startPoint.X, (int)endPoint.X);
            int upY = Math.Min((int)_startPoint.Y, (int)endPoint.Y);

            int rightX = Math.Max((int)_startPoint.X, (int)endPoint.X);
            int downY = Math.Max((int)_startPoint.Y, (int)endPoint.Y);

            RectangularEnt newEnt = new RectangularEnt(1, 666, false, leftX, upY, rightX, upY, rightX, downY, leftX, downY);

            newEnt.AddToCanvas(canvas);
        }


    }
}
