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

        public int Count
        {
            get { return _units.Count; }
        }

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
            return _units.Select(unit => unit.Output).ToList();
        }

        public List<double> GetErrors()
        {
            return _units.Select(unit => unit.Error).ToList();
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
