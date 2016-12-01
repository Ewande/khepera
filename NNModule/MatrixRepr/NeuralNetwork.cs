using MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics.LinearAlgebra.Double;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NNModule.MatrixRepr
{
    public class NeuralNetwork : Predictor
    {
        private List<Layer> _layers;

        public NeuralNetwork()
        {
            _layers = new List<Layer>();
        }

        public void AddLayer(Layer layer)
        {
            _layers.Add(layer);
        }

        public override List<double> Predict(List<double> input)
        {
            Matrix<double> inputMat = new DenseMatrix(1, input.Count, input.ToArray());
            foreach(Layer layer in _layers)
            {
                inputMat = inputMat.Multiply(layer.Weights) + layer.Biases;
                inputMat = inputMat.Map(layer.Activation);
            }
            return inputMat.Row(0).ToList();
        }
    }
}
