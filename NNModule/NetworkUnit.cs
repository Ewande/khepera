using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NNModule
{
    public class NetworkUnit
    {
        private static int ID_COUNTER = 0;

        private bool _isBias;
        private double _input;
        private double _output;

        public Func<double, double> ActFunc { get; set; }
        public int UnitId { get; private set; }
        public Dictionary<NetworkUnit, double> Connections;
        public double Input
        {
            get { return _input; }
            set
            {
                if (!_isBias)
                    _input = value;
            }
        }
        public double Output
        {
            get { return _output; }
            set
            {
                _output = value;
                if (MemoryUnit != null)
                    MemoryUnit.Input = value;
            }
        }
        public double Error;
        public double LastChange;
        public NetworkUnit MemoryUnit { get; set; }
        public NetworkUnit BiasUnit { get; private set; }

        public NetworkUnit(Func<double, double> actFunc, NetworkUnit biasUnit = null)
        {
            UnitId = ID_COUNTER;
            ID_COUNTER++;
            Connections = new Dictionary<NetworkUnit, double>();
            ActFunc = actFunc;
            BiasUnit = biasUnit;
            if(BiasUnit != null)
                Connections[BiasUnit] = 0;
        }

        public NetworkUnit(Func<double, double> actFunc, int unitId, NetworkUnit biasUnit = null)
        {
            UnitId = unitId;
            ID_COUNTER = Math.Max(unitId, ID_COUNTER) + 1;
            Connections = new Dictionary<NetworkUnit, double>();
            BiasUnit = biasUnit;
            ActFunc = actFunc;
        }

        public void Evaluate()
        {
            Output = ActFunc(_input);
        }

        public override string ToString()
        {
            StringBuilder result = new StringBuilder();
            result.Append(UnitId).Append(" ");
            result.Append(ActFuncs.GetIdOfFunc(ActFunc)).AppendLine();
            result.Append(BiasUnit == null ? -1 : BiasUnit.UnitId).Append(" ");
            result.Append(MemoryUnit == null ? -1 : MemoryUnit.UnitId).AppendLine();
            result.Append(Connections.Count).AppendLine();
            foreach (NetworkUnit connUnit in Connections.Keys.ToList())
                result.Append(connUnit.UnitId).Append(" ").Append(Connections[connUnit]).AppendLine();
            return result.ToString();
        }

        public static NetworkUnit CreateBias(int unitId = -1)
        {
            if(unitId < 0)
                return new NetworkUnit(ActFuncs.Identity) { _input = 1, _output = 1, _isBias = true };
            else
                return new NetworkUnit(ActFuncs.Identity, unitId) { _input = 1, _output = 1, _isBias = true};
        }
    }
}
