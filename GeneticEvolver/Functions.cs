using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneticEvolver
{
    static class Functions
    {
        public static double AvoidCollisions(Simulation simulation)
        {
            double left = simulation.LeftMotorSpeed;
            double right = simulation.RightMotorSpeed;
            double max = Controller.MAX_ABS_SPEED;

            double speedFactor = (Math.Abs(left) + Math.Abs(right)) / max;
            double movementFactor = 1 - Math.Sqrt(Math.Abs(left - right) / (2 * max));
            double proximityFactor = 1 - simulation.SensorStates.Max();
            return speedFactor * movementFactor * proximityFactor;
        }

        public static double Sigmoid(double x)
        {
            return 1.0 / (1.0 + Math.Exp(-x));
        }
    }
}
