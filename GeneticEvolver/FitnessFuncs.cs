using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneticEvolver
{
    static class FitnessFuncs
    {
        public static double AvoidCollisions(Simulation simulation, Controller contr)
        {
            double left = simulation.LeftMotorSpeed;
            double right = simulation.RightMotorSpeed;
            double max = Controller.MAX_ABS_SPEED;

            double speedFactor = (Math.Abs(left) + Math.Abs(right)) / (2 * max);
            double movementFactor = 1 - Math.Sqrt(Math.Abs(left - right) / (2 * max));
            double proximityFactor = 1 - Math.Sqrt(simulation.SensorStates.Max());

            // ------ for research:
            contr.SpeedFactor += speedFactor;
            contr.MovementFactor += movementFactor;
            contr.ProximityFactor += proximityFactor;
            // --------------------

            return speedFactor * movementFactor * proximityFactor;
        }

        
    }
}
