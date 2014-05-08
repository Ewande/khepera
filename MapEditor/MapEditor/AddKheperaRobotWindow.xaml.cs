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
using System.Windows.Shapes;

namespace MapEditor
{
    /// <summary>
    /// Interaction logic for AddKheperaRobotWindow.xaml
    /// </summary>
    public partial class AddKheperaRobotWindow : Window
    {
        public bool WasCanceled { get; private set; }

        public AddKheperaRobotWindow(UInt16 suggestedID)
        {
            InitializeComponent();

            IdLabel.Text = "" + suggestedID;
            WasCanceled = true;
        }

        public uint GetWeight()
        {
            return uint.Parse(weightEdit.Text);
        }

        public bool GetMovable()
        {
            return movableCheckbox.IsChecked.GetValueOrDefault();
        }

        public UInt16 GetWheelDistance()
        {
            return UInt16.Parse(wheelDistanceEdit.Text);
        }

        public UInt16 GetWheelRadius()
        {
            return UInt16.Parse(wheelRadiusEdit.Text);
        }

        public double GetStartingAngle()
        {
            return double.Parse(angleEdit.Text);
        }

        private void AddButtonClick(object sender, RoutedEventArgs e)
        {
            WasCanceled = false;
            Close();
        }
    }
}
