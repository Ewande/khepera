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

    public partial class AddRectangularEntWindow : Window
    {
        public bool WasCanceled {get; private set;}

        public AddRectangularEntWindow(int suggestedID)
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

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            WasCanceled = false;
            Close();
        }
    }
}
