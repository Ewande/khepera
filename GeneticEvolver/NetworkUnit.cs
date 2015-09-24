using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneticEvolver
{
    class NetworkUnit
    {
        private bool _isBias;
        private double _input;
        private double _output;

        public double Weight { get; set; }
        public double Input
        {
            get { return _input; }
            set
            {
                if (!_isBias)
                    _input = value;
            }
        }
        public double Output
        {
            get { return _output; }
            set
            {
                _output = value;
                if (MemoryUnit != null)
                    MemoryUnit.Output = value;
            }
        }

        public NetworkUnit BiasUnit { get; set; }
        public NetworkUnit MemoryUnit { get; set; }

        public static NetworkUnit CreateBias()
        {
            return new NetworkUnit() { _input = 1, _isBias = true};
        }
    }
}
