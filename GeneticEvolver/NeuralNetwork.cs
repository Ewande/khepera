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
        private Func<double, double> _actFunc;

        public Layer InLayer
        {
            get
            {
                return _layers.Count > 0 ? _layers.First() : null;
            }
        }
        public Layer OutLayer
        {
            get
            {
                return _layers.Count > 0 ? _layers.Last() : null;
            }
        }

        public NeuralNetwork(Func<double, double> actFunc)
        {
            _layers = new List<Layer>();
            _actFunc = actFunc;
        }

        public void AddLayer(Layer layer)
        {
            Layer prevLayer = OutLayer;
            if (prevLayer != null)
            {
                foreach (NetworkUnit unit in layer)
                    foreach (NetworkUnit prevUnit in prevLayer)
                        unit.Connections[prevUnit] = 0;
            }
            _layers.Add(layer);
        }

        public void Evaluate()
        {
            for (int i = 1; i < _layers.Count; i++)
            {
                foreach (NetworkUnit unit in _layers[i])
                {
                    double input = 0;
                    foreach (var conn in unit.Connections)
                        input += conn.Key.Input * conn.Value;
                    
                    unit.Output = _actFunc(input);
                }
            }
        }

        public void RandomizeWeights(double min, double max)
        {
            Random random = new Random();
            foreach (Layer layer in _layers)
                foreach (NetworkUnit unit in layer)
                    foreach (NetworkUnit key in unit.Connections.Keys)
                        unit.Connections[key] = min + random.NextDouble() * (max - min);
        }

        public List<double> GetAllWeights()
        {
            List<double> weights = new List<double>();
            foreach (Layer layer in _layers)
                foreach (NetworkUnit unit in layer)
                    foreach (double weight in unit.Connections.Values)
                        weights.Add(weight);

            return weights;
        }

        public bool SetAllWeights(List<double> weights)
        {
            Queue<double> copy = new Queue<double>(weights);
            foreach (Layer layer in _layers)
                foreach (NetworkUnit unit in layer)
                    foreach (NetworkUnit key in unit.Connections.Keys)
                    {
                        if (copy.Count == 0)
                            return false;
                        unit.Connections[key] = copy.Dequeue();
                    }
            return true;
        }

        public void TestPrint()
        {
            int i = 0;
            foreach (Layer layer in _layers)
            {
                Console.WriteLine("Layer " + i++);
                foreach (NetworkUnit unit in layer)
                {
                    Console.Write("\tUnit id = " + unit.UnitId + ": [");
                    foreach (NetworkUnit key in unit.Connections.Keys)
                        Console.Write("(" + key.UnitId + (key._isBias ? "B" : "") + ", " 
                            + unit.Connections[key] + ") ; ");
                    Console.WriteLine("]");
                }
            }        
        }
    }
}
