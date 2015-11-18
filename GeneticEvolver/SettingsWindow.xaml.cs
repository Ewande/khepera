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

namespace GeneticEvolver
{
    /// <summary>
    /// Interaction logic for SettingsWindow.xaml
    /// </summary>
    public partial class SettingsWindow : Window
    {
        public SettingsWindow()
        {
            InitializeComponent();
            String[] methods = {"tournament", "roulette wheel"};
            SelMethods.ItemsSource = methods;
            SelMethods.SelectedIndex = 0;
        }

        private void ApplyAndCloseWindow(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
