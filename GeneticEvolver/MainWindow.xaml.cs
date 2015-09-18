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

using System.Runtime.InteropServices;
using System.IO;

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

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet=CharSet.Ansi)]
        public static extern IntPtr createSimulation(string fileName, bool readBinary);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void removeSimulation(IntPtr simulation);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern IntPtr cloneSimulation(IntPtr simulation);

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            IntPtr sim = createSimulation("sample.txt", false);
            IntPtr simClone = cloneSimulation(sim);
            removeSimulation(sim);
            removeSimulation(simClone);
        }
    }
}
