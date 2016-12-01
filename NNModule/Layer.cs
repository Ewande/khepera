using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NNModule
{
    public class Layer
    {
        public List<NetworkUnit> Units { get; private set; }

        public Layer()
        {
            Units = new List<NetworkUnit>();
        }

        public void AddUnit(NetworkUnit unit)
        {
            Units.Add(unit);
        }

        public List<NetworkUnit>.Enumerator GetEnumerator()
        {
            return Units.GetEnumerator();
        }

        public void SetInputs(List<float> inputs)
        {
            for (int i = 0; i < Math.Min(inputs.Count, Units.Count); i++)
                Units[i].Input = inputs[i];
        }

        public List<double> GetOutputs()
        {
            return Units.Select(unit => unit.Output).ToList();
        }

        public List<double> GetErrors()
        {
            return Units.Select(unit => unit.Error).ToList();
        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder();
            result.Append(Units.Count).AppendLine();
            foreach (NetworkUnit unit in Units)
                result.Append(unit.ToString());
            return result.ToString();
        }

        public static Layer Load(StreamReader reader, Dictionary<int, NetworkUnit> netUnits)
        {
            Layer layer = new Layer();
            int units = int.Parse(reader.ReadLine());
            for (int j = 0; j < units; j++)
            {
                int[] unitInfo = IOUtils.ReadInts(reader);
                int[] extraUnits = IOUtils.ReadInts(reader);
                Func<double, double> actFunc = ActFuncs.ById(unitInfo[1]);
                NetworkUnit bias = extraUnits[0] > 0 ? netUnits.Get(extraUnits[0], isBias: true) : null;
                NetworkUnit memory = extraUnits[1] > 0 ? netUnits.Get(extraUnits[1]) : null;
                NetworkUnit unit = netUnits.Get(unitInfo[0], actFunc, bias);
                unit.MemoryUnit = memory;
                int connections = int.Parse(reader.ReadLine());
                for (int k = 0; k < connections; k++)
                {
                    double[] connTokens = IOUtils.ReadDoubles(reader);
                    int connUnitId = (int) connTokens[0];
                    double connWeight = connTokens[1];
                    unit.Connections[netUnits.Get(connUnitId)] = connWeight;
                }
                layer.AddUnit(unit);
            }
            return layer;
        }
    }
}
