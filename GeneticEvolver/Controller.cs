using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneticEvolver
{
    class Controller
    {
        public double Fitness { get; set; }

        public Controller(/* some config */)
        {

        }

        public void MoveRobot(Simulation simulation)
        {
            simulation.SetRobotSpeed(0, 0); // to do
        }

    }
}
