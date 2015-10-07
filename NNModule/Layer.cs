using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NNModule
{
    public class Layer
    {
        private List<NetworkUnit> _units;

        public Layer()
        {
            _units = new List<NetworkUnit>();
        }

        public void AddUnit(NetworkUnit unit)
        {
            _units.Add(unit);
        }

        public List<NetworkUnit>.Enumerator GetEnumerator()
        {
            return _units.GetEnumerator();
        }

        public void SetInputs(List<float> inputs)
        {
            for (int i = 0; i < Math.Min(inputs.Count, _units.Count); i++)
                _units[i].Input = inputs[i];
        }

        public List<double> GetOutputs()
        {
            List<double> outputs = new List<double>(_units.Count);
            foreach (NetworkUnit unit in _units)
                outputs.Add(unit.Output);
            return outputs;
        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder();
            result.Append(_units.Count).AppendLine();
            foreach (NetworkUnit unit in _units)
                result.Append(unit.ToString());
            return result.ToString();
        }
    }
}
