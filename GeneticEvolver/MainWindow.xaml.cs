using Microsoft.Win32;
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

namespace GeneticEvolver
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
        }

        private void LoadSimulation(object sender, RoutedEventArgs e)
        {
            if (!String.IsNullOrWhiteSpace(SimFile.Text))
            {
                Simulation.LoadDefaultState(SimFile.Text, false);
                List<int> ids = Simulation.GetRobotsIds();
                if (ids.Count > 0)
                {
                    FilePickGrid.IsEnabled = false;
                    RobotPickGrid.IsEnabled = true;
                    RobotId.ItemsSource = ids;
                    RobotId.SelectedIndex = 0;
                    SimParseResult.Text = "File loaded";
                    SimParseResult.Foreground = Brushes.Green;
                    SimParseResult.FontWeight = FontWeights.Bold;
                }
                else
                    SimParseResult.Text = "No robot to control";
            }
        }

        private void SelectFile(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "World Description Files|*.wd";

            bool? fileChosen = openFileDialog.ShowDialog();

            if (fileChosen == true)
                SimFile.Text = openFileDialog.FileName;
        }

        private void PickRobot(object sender, RoutedEventArgs e)
        {
            Simulation.SetControlledRobot(int.Parse(RobotId.Text));
            RobotPickGrid.IsEnabled = false;
        }

        private void Evolve()
        {
            // this is only a scheme of algorithm, will be upgraded
            Population pop = new Population(100);
            pop.Evaluate(null, -1);
            for(int i = 0; i < 200; i++)
            {
                pop = pop.Select(5);
                pop.Crossover(0.5);
                pop.Mutate(0.5);
                pop.Evaluate(null, -1);
            }
        }
    }
}
