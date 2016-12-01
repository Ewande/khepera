using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
using Visualiser.Entities;

namespace Visualiser
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public DisplayConfig DisplayConfig { get { return DisplayConfig.Instance; } }
        private Thread _worldReceiverThread;
        private ConnectionManager _connMan;
        private bool _connected;

        public MainWindow()
        {
            InitializeComponent();
            _connMan = new ConnectionManager();
            this.DataContext = this;
        }

        private delegate void UpdateWorldCallback(SimulationWorld world);

        private void WorldReceiverThread()
        {
            while(_connected)
            {
                SimulationWorld simulationWorld = _connMan.ReciveWorldDesc();
                worldCanvas.Dispatcher.Invoke(new UpdateWorldCallback(PaintSimulationWorld), simulationWorld);
            }
            _connMan.Disconnect();
        }

        private void ChangeConnection(object sender, RoutedEventArgs e)
        {
            if (!_connected)
            {
                ConnectionResult result = ConnectionResult.IncorrectParameters;
                if(!String.IsNullOrWhiteSpace(Host.Text))
                    result = _connMan.Connect(Host.Text, 1000);
                switch(result)
                {
                    case ConnectionResult.Connected:
                        _connected = true;
                        ConnectButton.Content = "DISCONNECT";
                        ConnectionStatus.Text = "Connected";
                        ConnectionStatus.Foreground = Brushes.Green;
                        ConnectionStatus.FontWeight = FontWeights.Bold;
                        Host.IsEnabled = false;
                        ShowSensRange.IsEnabled = true;
                        ShowId.IsEnabled = true;

                        _worldReceiverThread = new Thread(new ThreadStart(WorldReceiverThread));
                        _worldReceiverThread.Start();
                        break;
                    case ConnectionResult.ServerUnavailable:
                        ConnectionStatus.Text = "Error: server unavailable";
                        break;
                    case ConnectionResult.IncorrectParameters:
                        ConnectionStatus.Text = "Error: invalid parameters";
                        break;
                    default:
                        ConnectionStatus.Text = "Unknown error";
                        break;
                }
            }
            else
            {
                _connected = false;
                ConnectButton.Content = "CONNECT";
                ConnectionStatus.Text = "Not connected";
                ConnectionStatus.Foreground = Brushes.Red;
                ConnectionStatus.FontWeight = FontWeights.Normal;
                Host.IsEnabled = true;
                ShowSensRange.IsEnabled = false;
                ShowId.IsEnabled = false;
            }


        }

        private void PaintSimulationWorld(SimulationWorld simulationWorld)
        {
            /* TODO: If window is smaller then symulation world, we should probably scale every coordinates and dimensions, 
             * so that we can paint whole world in smaller window */

            worldCanvas.Children.Clear();
            WorldDim.Text = String.Format("{0} x {1}", simulationWorld.WorldWidth, simulationWorld.WorldHeight);
            SimTime.Text = String.Format("{0:0.00} s", simulationWorld.Time);
            int robots = simulationWorld.Entities.Where(x => x.Value.ShapeID == SimEnt.KHEPERA_ROBOT_ID).Count();
            NumOfRob.Text = robots.ToString();
            if (robots != 0)
            {
                RobotsGrid.Visibility = System.Windows.Visibility.Visible;
                RobotsGrid.ItemsSource = simulationWorld.Entities
                    .Select(x => x.Value)
                    .Where(x => x.ShapeID == SimEnt.KHEPERA_ROBOT_ID)
                    .Select(x => new
                    {
                        ID = x.ID,
                        IP = x.ControllerAddr ?? "Not connected"
                    })
                    .OrderBy(x => x.ID);
            }
            else
                RobotsGrid.Visibility = System.Windows.Visibility.Hidden;

            foreach (SimEnt entity in simulationWorld.Entities.Values)
                entity.AddToCanvas(worldCanvas);
        }

        private void TextBox_GotFocus(object sender, RoutedEventArgs e)
        {
            (sender as TextBox).CaptureMouse();
        }


        private void Highlight(object sender, MouseEventArgs e)
        {
            (sender as TextBox).SelectAll();
        }

        private void Highlight(object sender, DependencyPropertyChangedEventArgs e)
        {
            (sender as TextBox).SelectAll();
        }
    }
}
