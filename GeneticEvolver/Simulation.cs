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
        private static extern IntPtr getRobot(IntPtr simulation, int robotId);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern int getSensorCount(IntPtr robot);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern float getSensorState(IntPtr robot, int sensorNumber);

        [DllImport("SimulationServer.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Ansi)]
        private static extern void setRobotSpeed(IntPtr robot, double leftMotor, double rightMotor);


        private IntPtr  _simulation;
        private IntPtr  _robot;
        private int    _robotId;

        public Simulation(string descriptionFileName, bool readBinary)
        {
            _simulation = createSimulation(descriptionFileName, readBinary);
        }

        public Simulation(Simulation other)
        {
            _simulation = cloneSimulation(other._simulation);
            if (other._robot != IntPtr.Zero)
                _robot = getRobot(_simulation, _robotId);
        }

        ~Simulation()
        {
            removeSimulation(_simulation);
        }

        bool setControlledRobot(int robotId)
        {
            _robot = getRobot(_simulation, robotId);
            _robotId = robotId;
            return _robot != IntPtr.Zero;
        }
    }
}
