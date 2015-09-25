using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GeneticEvolver
{
    class NNFactory
    {
        public static NeuralNetwork CreateElmanNN(int inputCount, int outputCount,
            Func<NetworkUnit, Layer, double> actFunc)
        {
            NeuralNetwork network = new NeuralNetwork(actFunc);
            Layer input = new Layer();
            Layer output = new Layer();
            for (int i = 0; i < inputCount; i++)
                input.AddUnit(new NetworkUnit());
            for (int i = 0; i < outputCount; i++)
            {
                NetworkUnit memoryUnit = new NetworkUnit();
                NetworkUnit biasUnit = NetworkUnit.CreateBias();
                NetworkUnit outUnit = new NetworkUnit() { MemoryUnit = memoryUnit, BiasUnit = biasUnit };
                input.AddUnit(memoryUnit);
                output.AddUnit(outUnit);
            }

            return network;
        }
    }
}
