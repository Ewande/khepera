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
using System.ComponentModel;

namespace GeneticEvolver
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private static readonly Dictionary<string, Func<Simulation, double>> _BEHAVIORS =
            new Dictionary<string, Func<Simulation, double>>
        {
            {"collision avoidance", Functions.AvoidCollisions}
        };

        private Func<Simulation, double> _chosenEvaluator;
        private BackgroundWorker _bWorker;

        public MainWindow()
        {
            InitializeComponent();
            _bWorker = new BackgroundWorker();
            _bWorker.WorkerReportsProgress = true;
            _bWorker.DoWork += RunGeneticAlgorithm;
            _bWorker.ProgressChanged += ChangeProgress;
            _bWorker.RunWorkerCompleted += SignalCompletion;
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
            ConfigurationGrid.IsEnabled = true;
            BehaviorType.ItemsSource = _BEHAVIORS.Keys;
            if (_BEHAVIORS.Keys.Count > 0)
                BehaviorType.SelectedIndex = 0;
        }

        private void Evolve(object sender, RoutedEventArgs e)
        {
            EvolveButton.IsEnabled = false;
            _chosenEvaluator = _BEHAVIORS[BehaviorType.Text];
            _bWorker.RunWorkerAsync();
        }

        private void RunGeneticAlgorithm(object sender, DoWorkEventArgs e)
        {
            
            int generations = 3;
            int popSize = 20;
            Population pop = new Population(popSize);
            for (int i = 0; i < generations; i++)
            {
                pop.Evaluate(_chosenEvaluator, 20, 7);
                _bWorker.ReportProgress((i + 1) * 100 / (generations + 1));
                pop = pop.Select(5);
                pop.Crossover(0.5);
                pop.Mutate(0.5);
            }
            pop.Evaluate(_chosenEvaluator, 20, 7);
            _bWorker.ReportProgress(100);
        }

        private void ChangeProgress(object sender, ProgressChangedEventArgs e)
        {
            ProgressInfo.Value = e.ProgressPercentage;
        }

        private void SignalCompletion(object sender, RunWorkerCompletedEventArgs e)
        {
            EvolveButton.IsEnabled = true;
        }
    }
}
