using System;
using System.Collections.Generic;
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
    }
}
