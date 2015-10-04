using Microsoft.Win32;
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
using System.IO;
using NNModule;

namespace Controller
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private enum SteeringType { Manual, Script };

        private SteeringType _steeringType;
        private KeyEventHandler _keyHandler;
        private NeuralNetwork _neuralNetwork;

        private Thread _communicationThread;
        private ConnectionManager _connMan;
        private bool _connected;

        private Robot _robot;
        private object _lockRobot = new object();

        public MainWindow()
        {
            InitializeComponent();
            _connMan = new ConnectionManager();
            _steeringType = SteeringType.Manual;
            _connected = false;
            _robot = new Robot();
            _keyHandler = new KeyEventHandler(OnButtonKeyDown);
            
        }

        private void CommunicationRoutine()
        {
            int i = 0;
            while (_connected)
            {
                _robot.Sensors = _connMan.ReadSensorsState();
                if (_steeringType == SteeringType.Script && i % 7 == 0)
                {
                    _neuralNetwork.InLayer.SetInputs(_robot.Sensors.Select(x => x.State).ToList());
                    _neuralNetwork.Evaluate();
                    List<double> speeds = _neuralNetwork.OutLayer.GetOutputs();
                    _robot.LeftMotorSpeed = speeds[0] * Robot.DEFAULT_MAX_SPEED;
                    _robot.RightMotorSpeed = speeds[1] * Robot.DEFAULT_MAX_SPEED;
                    _robot.SpeedChanged = true;
                }
                if (_robot.SpeedChanged)
                {
                    _connMan.SendMotorsSpeedCommand(_robot.LeftMotorSpeed, _robot.RightMotorSpeed);
                    _robot.SpeedChanged = false;
                }
                i++;
            }
            _connMan.Disconnect();
        }

        private void OnButtonKeyDown(object sender, KeyEventArgs e)
        {
            lock (_lockRobot)
            {
                switch (e.Key)
                {
                    case Key.Up:
                        _robot.LeftMotorSpeed = Robot.DEFAULT_MAX_SPEED;
                        _robot.RightMotorSpeed = Robot.DEFAULT_MAX_SPEED;
                        _robot.SpeedChanged = true;
                        break;
                    case Key.Down:
                        _robot.LeftMotorSpeed = 0;
                        _robot.RightMotorSpeed = 0;
                        _robot.SpeedChanged = true;
                        break;
                    case Key.Left:
                        _robot.LeftMotorSpeed = 0;
                        _robot.RightMotorSpeed = Robot.DEFAULT_MAX_SPEED;
                        _robot.SpeedChanged = true;
                        break;
                    case Key.Right:
                        _robot.LeftMotorSpeed = Robot.DEFAULT_MAX_SPEED;
                        _robot.RightMotorSpeed = 0;
                        _robot.SpeedChanged = true;
                        break;
                    default:
                        break;
                }
            }
        }

        private void ChangeConnection(object sender, RoutedEventArgs e)
        {
            if (_connected)
            {
                _connected = false;
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

                        if (_steeringType == SteeringType.Manual)
                            this.KeyDown += _keyHandler;
                        _communicationThread = new Thread(new ThreadStart(CommunicationRoutine));
                        _communicationThread.Start();
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

        private void SelectScript(object sender, RoutedEventArgs e)
        {
            if (_steeringType == SteeringType.Manual)
            {
                ManualButton.IsChecked = true;

                OpenFileDialog openFileDialog = new OpenFileDialog();

                openFileDialog.Filter = "Robot Controller Scripts|*.rcs";

                bool? fileChosen = openFileDialog.ShowDialog();

                if (fileChosen == true)
                {
                    Stream fileStream = openFileDialog.OpenFile();

                    using (StreamReader reader = new StreamReader(fileStream))
                    {
                        if ((_neuralNetwork = NNManager.CreateNN(reader)) != null)
                        {
                            ScriptButton.IsChecked = true;
                            ManualControls.Visibility = Visibility.Hidden;
                            _steeringType = SteeringType.Script;
                            if (_connected)
                                this.KeyDown -= _keyHandler;
                        }
                    }
                    fileStream.Close();
                }
            }
        }

        private void SelectManual(object sender, RoutedEventArgs e)
        {
            if (_steeringType == SteeringType.Script)
            {
                _neuralNetwork = null;
                ManualControls.Visibility = Visibility.Visible;
                _steeringType = SteeringType.Manual;
                if (_connected)
                    this.KeyDown += _keyHandler;
            }
        }
    }
}
