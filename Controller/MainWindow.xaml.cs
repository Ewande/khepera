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

namespace Controller
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private enum SteeringType { Manual, Script };

        private ConnectionManager _connMan;
        private bool _connected;
        SteeringType steeringType;

        public MainWindow()
        {
            InitializeComponent();
            _connMan = new ConnectionManager();
            steeringType = SteeringType.Manual;
            _connected = false;
        }

        private void SelectScript(object sender, RoutedEventArgs e)
        {
            if (steeringType == SteeringType.Manual)
            {
                ManualControls.Visibility = Visibility.Hidden;
                steeringType = SteeringType.Script;
            }
        }

        private void SelectManual(object sender, RoutedEventArgs e)
        {
            if (steeringType == SteeringType.Script)
            {
                ManualControls.Visibility = Visibility.Visible;
                steeringType = SteeringType.Manual;
            }
        }

        private void ChangeConnection(object sender, RoutedEventArgs e)
        {
            if (_connected)
            {
                _connected = false;
                _connMan.Disconnect();
                ConnectionStatus.Text = "Not connected";
                ConnectButton.Content = "CONNECT";
                ConnectionStatus.Foreground = Brushes.Red;
                ConnectionStatus.FontWeight = FontWeights.Normal;
                EntityID.IsEnabled = true;
                AccessCode.IsEnabled = true;
                Host.IsEnabled = true;
            }
            else
            {
                ConnectionResult result = ConnectionResult.IncorrectParameters;
                ushort id;
                bool isIdParsable = UInt16.TryParse(EntityID.Text, out id);
                if(isIdParsable && !String.IsNullOrWhiteSpace(Host.Text))
                    result = _connMan.Connect(Host.Text, id, AccessCode.Text, 1000);
                switch(result)
                {
                    case ConnectionResult.Connected:
                        _connected = true;
                        ConnectionStatus.Text = "Connected";
                        ConnectButton.Content = "DISCONNECT";
                        ConnectionStatus.Foreground = Brushes.Green;
                        ConnectionStatus.FontWeight = FontWeights.Bold;
                        EntityID.IsEnabled = false;
                        AccessCode.IsEnabled = false;
                        Host.IsEnabled = false;
                        break;
                    case ConnectionResult.ServerUnavailable:
                        ConnectionStatus.Text = "Error: server unavailable";
                        break;
                    case ConnectionResult.IncorrectRobotId:
                        ConnectionStatus.Text = "Error: incorrect robot ID";
                        break;
                    case ConnectionResult.IncorrectAccessCode:
                        ConnectionStatus.Text = "Error: incorrect acccess code";
                        break;
                    case ConnectionResult.IncorrectParameters:
                        ConnectionStatus.Text = "Error: invalid parameters";
                        break;
                    default:
                        ConnectionStatus.Text = "Unknown error";
                        break;
                }
                    
            }
        }
    }
}
