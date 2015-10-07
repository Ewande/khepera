using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NNModule;
using System.IO;

namespace Controller
{
    class NNManager
    {
        public static NeuralNetwork CreateNN(StreamReader reader)
        {
            NeuralNetwork network = new NeuralNetwork();
            Dictionary<int, NetworkUnit> netUnits = new Dictionary<int,NetworkUnit>();
            try
            {
                int layers = int.Parse(reader.ReadLine());
                for (int i = 0; i < layers; i++)
                {
                    int units = int.Parse(reader.ReadLine());
                    Layer layer = new Layer();
                    for (int j = 0; j < units; j++)
                    {
                        int[] unitInfo = reader.ReadLine().Split().Select(x => int.Parse(x)).ToArray();
                        int[] extraUnits = reader.ReadLine().Split().Select(x => int.Parse(x)).ToArray();
                        Func<double, double> actFunc = ActFuncs.GetFuncById(unitInfo[1]);
                        NetworkUnit bias = extraUnits[0] > 0 ? netUnits.Get(extraUnits[0]) : null;
                        NetworkUnit memory = extraUnits[1] > 0 ? netUnits.Get(extraUnits[1]) : null;
                        NetworkUnit unit = netUnits.Get(unitInfo[0], actFunc, bias);
                        unit.MemoryUnit = memory;
                        int connections = int.Parse(reader.ReadLine());
                        for (int k = 0; k < connections; k++)
                        {
                            string[] connTokens = reader.ReadLine().Split();
                            int connUnitId = int.Parse(connTokens[0]);
                            double connWeight = double.Parse(connTokens[1]);
                            unit.Connections[netUnits.Get(connUnitId)] = connWeight;
                        }
                        layer.AddUnit(unit);
                    }
                    network.AddLayer(layer);
                }
                return network;
            }
            catch (FormatException)
            {
                return null;
            }
        }
    }

    static class NNExtensions
    {
        public static NetworkUnit Get(this Dictionary<int, NetworkUnit> dict, int key, 
            Func<double, double> actFunc = null, NetworkUnit biasUnit = null)
        {
            if(!dict.ContainsKey(key))
                dict[key] = new NetworkUnit(actFunc, key, biasUnit);
            return dict[key];
        }
    }
}
