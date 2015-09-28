using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneticEvolver
{
    class Layer
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
    }
}
