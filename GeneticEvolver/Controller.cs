using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneticEvolver
{
    class Controller :IComparable<Controller>
    {
        public static double MAX_ABS_SPEED = 5;

        public double Fitness { get; set; }
        public NeuralNetwork NeuralNetwork { get; set; }

        public Controller(NeuralNetwork network)
        {
            NeuralNetwork = network;
        }

        public bool MoveRobot(Simulation simulation)
        {
            NeuralNetwork.InLayer.SetInputs(simulation.SensorStates);
            NeuralNetwork.Evaluate();
            List<double> motorSpeeds = NeuralNetwork.OutLayer.GetOutputs();
            if (motorSpeeds.Count != 2)
                return false;
            simulation.SetRobotSpeed(motorSpeeds[0], motorSpeeds[1]);
            return true;
        }

        public int CompareTo(Controller other)
        {
            return Fitness.CompareTo(other.Fitness);
        }

        public override string ToString()
        {
            return NeuralNetwork.ToString();
        }
    }
}
