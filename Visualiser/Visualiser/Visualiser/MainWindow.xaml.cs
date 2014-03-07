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

namespace Visualiser
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

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            ConnectionManager c = new ConnectionManager();
            if (c.Connect(host.Text))
            {
                SymulationWorld symulationWorld = c.ReciveWorldDesc();
                PaintSymulationWorld(symulationWorld);
            }
            else
                MessageBox.Show("Couldnt connect");


        }

        private void PaintSymulationWorld(SymulationWorld symulationWorld)
        {
            /* TODO: If window is smaller then symulation world, we should probably scale every coordinates and dimensions, 
             * so that we can paint whole world in smaller window */

            foreach (SymEnt entity in symulationWorld.Entities.Values)
            {
                entity.AddToCanvas(worldCanvas);
            }
        }
    }
}
