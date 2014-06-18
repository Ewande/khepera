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
        private ConnectionManager _connMan;

        public MainWindow()
        {
            InitializeComponent();
        }

        private delegate void UpdateWorldCallback(SymulationWorld world);

        private void WorldReceiverThread()
        {
            for (int i = 0; i < 100000; i++)
            {
                SymulationWorld symulationWorld = _connMan.ReciveWorldDesc();
                worldCanvas.Dispatcher.Invoke(new UpdateWorldCallback(PaintSymulationWorld), symulationWorld);
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            _connMan = new ConnectionManager();
            if (_connMan.Connect(host.Text))
            {
                // receive world descriptions on separate thread and don't block GUI thread, so that app remains responsive
                Thread worldReceiverThread = new Thread(new ThreadStart(WorldReceiverThread));
                worldReceiverThread.Start();
            }
            else
                MessageBox.Show("Couldnt connect");


        }

        private void PaintSymulationWorld(SymulationWorld symulationWorld)
        {
            /* TODO: If window is smaller then symulation world, we should probably scale every coordinates and dimensions, 
             * so that we can paint whole world in smaller window */

            worldCanvas.Children.Clear();

            foreach (SymEnt entity in symulationWorld.Entities.Values)
            {
                entity.AddToCanvas(worldCanvas);
            }
        }
    }
}
