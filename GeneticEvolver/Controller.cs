using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneticEvolver
{
    class Controller
    {
        public static double MAX_ABS_SPEED = 5;

        public double Fitness { get; set; }
        public NeuralNetwork NeuralNetwork { get; set; }

        public Controller(NeuralNetwork network)
        {
            NeuralNetwork = network;
        }

        public void MoveRobot(Simulation simulation)
        {
            simulation.SetRobotSpeed(0, 0); // to do
        }

    }
}
