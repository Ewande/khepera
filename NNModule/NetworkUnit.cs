﻿using System;
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

        public NetworkUnit MemoryUnit { get; set; }
        public NetworkUnit BiasUnit { get; private set; }

        public NetworkUnit()
        {
            UnitId = ID_COUNTER;
            ID_COUNTER++;
            Connections = new Dictionary<NetworkUnit, double>();
        }

        public NetworkUnit(NetworkUnit biasUnit) : this()
        {
            BiasUnit = biasUnit;
            Connections[BiasUnit] = 0;
        }

        public NetworkUnit(int unitId, NetworkUnit biasUnit = null)
        {
            UnitId = unitId;
            ID_COUNTER = Math.Max(unitId, ID_COUNTER) + 1;
            Connections = new Dictionary<NetworkUnit, double>();
            BiasUnit = biasUnit;
        }

        public static NetworkUnit CreateBias()
        {
            return new NetworkUnit() { _input = 1, _output = 1, _isBias = true};
        }
    }
}
