using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NNModule
{
    public class NeuralNetwork : Predictor
    {
        private static Random random = new Random();

        private List<Layer> _layers;

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

        public NeuralNetwork()
        {
            _layers = new List<Layer>();
        }

        public void AddLayer(Layer layer, bool addConnections = true)
        {
            Layer prevLayer = OutLayer;
            if (addConnections && prevLayer != null)
            {
                foreach (NetworkUnit unit in layer)
                    foreach (NetworkUnit prevUnit in prevLayer)
                        unit.Connections[prevUnit] = 0;
            }
            _layers.Add(layer);
        }

        public void Evaluate()
        {
            foreach (Layer layer in _layers)
            {
                foreach (NetworkUnit unit in layer)
                {
                    foreach (var conn in unit.Connections)
                        unit.Input += conn.Key.Output * conn.Value;

                    unit.Evaluate();
                    unit.Input = 0;
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

        public List<Layer>.Enumerator GetEnumerator()
        {
            return _layers.GetEnumerator();
        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder();
            result.Append(_layers.Count).AppendLine();
            foreach (Layer layer in _layers)
                result.Append(layer);
            return result.ToString();
        }

        public override List<double> Predict(List<double> input)
        {
            InLayer.SetInputs(input.Select(x => (float)x).ToList());
            Evaluate();
            return OutLayer.GetOutputs();
        }

        public static NeuralNetwork Load(StreamReader reader)
        {
            NeuralNetwork network = new NeuralNetwork();
            Dictionary<int, NetworkUnit> netUnits = new Dictionary<int, NetworkUnit>();
            int layers = int.Parse(reader.ReadLine());
            for (int i = 0; i < layers; i++)
                network.AddLayer(Layer.Load(reader, netUnits), false);
            return network;
        }
    }

    static class NNExtensions
    {
        public static NetworkUnit Get(this Dictionary<int, NetworkUnit> dict, int key,
            Func<double, double> actFunc = null, NetworkUnit biasUnit = null, bool isBias = false)
        {
            if (!dict.ContainsKey(key))
                dict[key] = isBias ? NetworkUnit.CreateBias(key) : new NetworkUnit(actFunc, key, biasUnit);
            return dict[key];
        }
    }
}
