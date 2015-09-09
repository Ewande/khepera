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

namespace Visualiser
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private Thread _worldReceiverThread;
        private ConnectionManager _connMan;
        private bool _connected;

        public MainWindow()
        {
            InitializeComponent();
            _connMan = new ConnectionManager();
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
                        Host.IsEnabled = false;

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
                Host.IsEnabled = true;
            }


        }

        private void PaintSimulationWorld(SimulationWorld simulationWorld)
        {
            /* TODO: If window is smaller then symulation world, we should probably scale every coordinates and dimensions, 
             * so that we can paint whole world in smaller window */

            worldCanvas.Children.Clear();

            foreach (SimEnt entity in simulationWorld.Entities.Values)
            {
                entity.AddToCanvas(worldCanvas);
            }
        }
    }
}
