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
        private SymulationWorld _world;
        private UInt16 _nextID;
        private AddCommand _activeCommand;
        private bool _mouseButtonDown;
        private SelectionRegion _selectionRegion;

        public MainWindow()
        {
            InitializeComponent();
            _world = new SymulationWorld(500, 500);
            _nextID = 0;
            _activeCommand = new AddRectangularEntCommand();
            _mouseButtonDown = false;
            _selectionRegion = new SelectionRegion(canvas);
        }

        private void OnMouseDown(object sender, MouseButtonEventArgs e)
        {
            _startPoint = Mouse.GetPosition(canvas);
            _mouseButtonDown = true;
        }

        private void OnMouseUp(object sender, MouseButtonEventArgs e)
        {
            if (_mouseButtonDown)
            {
                _mouseButtonDown = false;
                Point endPoint = Mouse.GetPosition(canvas);

                int leftX = Math.Min((int)_startPoint.X, (int)endPoint.X);
                int upY = Math.Min((int)_startPoint.Y, (int)endPoint.Y);

                int rightX = Math.Max((int)_startPoint.X, (int)endPoint.X);
                int downY = Math.Max((int)_startPoint.Y, (int)endPoint.Y);

                SymEnt newEnt = _activeCommand.Exectue(leftX, rightX, upY, downY, _nextID);

                if (newEnt != null)
                {
                    _world.AddEnt(newEnt);
                    _world.Redraw(canvas);
                    ++_nextID;
                }

                _selectionRegion.Reset();
            }
        }

        private void OnSaveClick(object sender, RoutedEventArgs e)
        {
            _world.SaveToFile("world.wld");
        }

        private void OnMouseMove(object sender, MouseEventArgs e)
        {
            if (_mouseButtonDown)
            {
                Point endPoint = Mouse.GetPosition(canvas);

                int leftX = Math.Min((int)_startPoint.X, (int)endPoint.X);
                int upperY = Math.Min((int)_startPoint.Y, (int)endPoint.Y);

                int rightX = Math.Max((int)_startPoint.X, (int)endPoint.X);
                int bottomY = Math.Max((int)_startPoint.Y, (int)endPoint.Y);

                _selectionRegion.Update(leftX, rightX, upperY, bottomY);
            }
        }

        private void EllipseButtonClick(object sender, RoutedEventArgs e)
        {
            _activeCommand = new AddCircularEntCommand();
        }

        private void RectangleButtonClick(object sender, RoutedEventArgs e)
        {
            _activeCommand = new AddRectangularEntCommand();
        }

        private void RobotButtonClick(object sender, RoutedEventArgs e)
        {
            _activeCommand = new AddKheperaRobotCommand();
        }


    }
}
