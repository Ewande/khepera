using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Visualiser
{
    class SimulationWorld
    {
        public const int INFINITY = 1000000;

        public UInt32                       WorldWidth { get; set; }
        public UInt32                       WorldHeight { get; set; }
        public Double                       Time { get; set; }

        public Dictionary<UInt16, SimEnt>   Entities { get; private set; }

        public SimulationWorld()
        {
            Entities = new Dictionary<UInt16, SimEnt>();
        }
    }
}
