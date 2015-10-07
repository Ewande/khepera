using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using NNModule;

namespace GeneticEvolver
{
    class NNFactory
    {
        public static NeuralNetwork CreateElmanNN(int inputCount, int outputCount)
        {
            NeuralNetwork network = new NeuralNetwork();
            Layer input = new Layer();
            Layer output = new Layer();
            for (int i = 0; i < inputCount; i++)
                input.AddUnit(new NetworkUnit(ActFuncs.Identity));
            for (int i = 0; i < outputCount; i++)
            {
                NetworkUnit memoryUnit = new NetworkUnit(ActFuncs.Identity);
                NetworkUnit biasUnit = NetworkUnit.CreateBias();
                NetworkUnit outUnit = new NetworkUnit(ActFuncs.Sigmoid ,biasUnit) { MemoryUnit = memoryUnit };
                input.AddUnit(memoryUnit);
                output.AddUnit(outUnit);
            }
            network.AddLayer(input);
            network.AddLayer(output);

            return network;
        }
    }
}
