using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace GeneticEvolver
{
    class Simulation
    {
        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern IntPtr createSimulation(string fileName, bool readBinary);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void removeSimulation(IntPtr simulation);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern IntPtr cloneSimulation(IntPtr simulation);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void updateSimulation(IntPtr simulation, uint steps);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern int getRobotCount(IntPtr simulation);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern bool fillRobotsIdArray(IntPtr simulation, int[] idArray, int arrLength);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern IntPtr getRobot(IntPtr simulation, int robotId);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern int getSensorCount(IntPtr robot);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern float getSensorState(IntPtr robot, int sensorNumber);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void setRobotSpeed(IntPtr robot, double leftMotor, double rightMotor);


        private static Random random = new Random();
        private static Simulation _defaultState;

        private IntPtr  _simulation;
        private IntPtr  _robot;
        private int    _robotId;

        public List<float> SensorStates { get; private set; }
        public double LeftMotorSpeed { get; private set; }
        public double RightMotorSpeed { get; private set; }

        private Simulation(string descriptionFileName, bool readBinary)
        {
            _simulation = createSimulation(descriptionFileName, readBinary);
        }

        private Simulation(Simulation other)
        {
            _simulation = cloneSimulation(other._simulation);
            if (other._robot != IntPtr.Zero)
            {
                _robotId = other._robotId;
                _robot = getRobot(_simulation, _robotId);
            }
            SensorStates = new List<float>(_defaultState.SensorStates);
        }

        ~Simulation()
        {
            removeSimulation(_simulation);
        }

        public static bool LoadDefaultState(string descriptionFileName, bool readBinary)
        {
            _defaultState = new Simulation(descriptionFileName, readBinary);
            
            return true; // to do: implement error passing
        }

        public static Simulation CloneDefault()
        {
            return _defaultState == null ? null : new Simulation(_defaultState);
        }

        public static List<int> GetRobotsIds()
        {
            int robotCount = getRobotCount(_defaultState._simulation);
            int[] ids = new int[robotCount];
            fillRobotsIdArray(_defaultState._simulation, ids, robotCount);
            return new List<int>(ids);
        }

        public static bool SetControlledRobot(int robotId)
        {
            if (_defaultState == null)
                return false;
            _defaultState._robot = getRobot(_defaultState._simulation, robotId);
            if(_defaultState._robot == IntPtr.Zero)
                return false;
            _defaultState._robotId = robotId;
            _defaultState.SensorStates = new List<float>();
            for(int i = 0; i < getSensorCount(_defaultState._robot); i++)
                _defaultState.SensorStates.Add(getSensorState(_defaultState._robot, i));
            return true;
        }

        public bool SetRobotSpeed(double leftMotor, double rightMotor)
        {
            if (_robot == IntPtr.Zero)
                return false;
            setRobotSpeed(_robot, leftMotor, rightMotor);
            LeftMotorSpeed = leftMotor;
            RightMotorSpeed = rightMotor;

            return true;
        }

        public void ShuffleRobot(uint steps)
        {
            SetRobotSpeed(-Controller.MAX_ABS_SPEED + random.NextDouble() * Controller.MAX_ABS_SPEED * 2,
                -Controller.MAX_ABS_SPEED + random.NextDouble() * Controller.MAX_ABS_SPEED * 2);
            Update(steps);
        }

        public void Update(uint steps = 1)
        {
            updateSimulation(_simulation, steps);
            UpdateSensorList();
        }

        private void UpdateSensorList()
        {
            if (_robot != IntPtr.Zero)
                for (int i = 0; i < SensorStates.Count; i++)
                    SensorStates[i] = getSensorState(_robot, i);
        }
    }
}
