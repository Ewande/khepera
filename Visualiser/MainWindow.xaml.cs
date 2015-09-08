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

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (!_connected)
            {
                if (_connMan.Connect(host.Text))
                {
                    _connected = true;
                    button.Content = "DISCONNECT";
                    host.IsEnabled = false;
                    // receive world descriptions on separate thread and don't block GUI thread, so that app remains responsive
                    _worldReceiverThread = new Thread(new ThreadStart(WorldReceiverThread));
                    _worldReceiverThread.Start();
                }
                else
                    MessageBox.Show("Could not connect to the server.");
            }
            else
            {
                _connected = false;
                button.Content = "CONNECT";
                host.IsEnabled = true;
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
