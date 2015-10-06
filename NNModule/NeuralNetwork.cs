using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NNModule
{
    public class NeuralNetwork
    {
        private static Random random = new Random();

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
            foreach (NetworkUnit unit in InLayer)
                unit.Output = unit.Input;
            for (int i = 1; i < _layers.Count; i++)
            {
                foreach (NetworkUnit unit in _layers[i])
                {
                    unit.Input = 0;
                    foreach (var conn in unit.Connections)
                        unit.Input += conn.Key.Output * conn.Value;
                    
                    unit.Output = _actFunc(unit.Input);
                }
            }
        }

        public void RandomizeWeights(double min, double max)
        {
            foreach (Layer layer in _layers)
                foreach (NetworkUnit unit in layer)
                    foreach (NetworkUnit key in unit.Connections.Keys.ToList())
                        unit.Connections[key] = min + random.NextDouble() * (max - min);
        }

        public List<double> GetAllWeights()
        {
            List<double> weights = new List<double>();
            foreach (Layer layer in _layers)
                foreach (NetworkUnit unit in layer)
                    foreach (double weight in unit.Connections.Values.ToList())
                        weights.Add(weight);

            return weights;
        }

        public bool SetAllWeights(List<double> weights)
        {
            Queue<double> copy = new Queue<double>(weights);
            foreach (Layer layer in _layers)
                foreach (NetworkUnit unit in layer)
                    foreach (NetworkUnit key in unit.Connections.Keys.ToList())
                    {
                        if (copy.Count == 0)
                            return false;
                        unit.Connections[key] = copy.Dequeue();
                    }
            return true;
        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder();
            result.Append(_layers.Count).AppendLine();
            foreach (Layer layer in _layers)
            {
                result.Append(layer.UnitCount).AppendLine();
                foreach (NetworkUnit unit in layer)
                {
                    result.Append(unit.UnitId).Append(" ");
                    result.Append(unit.BiasUnit == null ? -1 : unit.BiasUnit.UnitId).Append(" ");
                    result.Append(unit.MemoryUnit == null ? -1 : unit.MemoryUnit.UnitId).AppendLine();
                    result.Append(unit.Connections.Count).AppendLine();
                    foreach (NetworkUnit connUnit in unit.Connections.Keys.ToList())
                        result.Append(connUnit.UnitId).Append(" ").Append(unit.Connections[connUnit]).AppendLine();
                }
            }
            return result.ToString();
        }
    }
}
