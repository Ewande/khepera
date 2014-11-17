using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Visualiser
{
    class SymulationWorld
    {
        public const int INFINITY = 1000000;

        public UInt32                       WorldWidth { get; set; }
        public UInt32                       WorldHeight { get; set; }
        public UInt32                       Time { get; set; }

        public Dictionary<UInt16, SymEnt>   Entities { get; private set; }

        public SymulationWorld()
        {
            Entities = new Dictionary<ushort, SymEnt>();
        }
    }
}
