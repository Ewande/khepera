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
    }
}
