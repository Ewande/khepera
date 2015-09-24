using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneticEvolver
{
    class NeuralNetwork
    {
        private List<Layer> _layers;
        private Func<NetworkUnit, Layer, double> _actFunc;

        public Layer InLayer
        {
            get
            {
                return _layers != null ? _layers.First() : null;
            }
        }
        public Layer OutLayer
        {
            get
            {
                return _layers != null ? _layers.Last() : null;
            }
        }

        public NeuralNetwork(Func<NetworkUnit, Layer, double> actFunc)
        {
            _layers = new List<Layer>();
            _actFunc = actFunc;
        }

        public void AddLayer(Layer layer)
        {
            _layers.Add(layer);
        }

        public void Evaluate()
        {
            for (int i = 1; i < _layers.Count; i++)
            {
                foreach (NetworkUnit unit in _layers[i])
                    unit.Output = _actFunc(unit, _layers[i - 1]);
            }
        }
    }
}
