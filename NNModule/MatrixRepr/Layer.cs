using MathNet.Numerics.LinearAlgebra;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NNModule.MatrixRepr
{
    public class Layer
    {
        public Matrix<double> Weights;
        public Matrix<double> Biases;
        public Func<double, double> Activation;

        public Layer(Matrix<double> weights, Matrix<double> biases, Func<double, double> activation)
        {
            Weights = weights;
            Biases = biases;
            Activation = activation;
        }
    }
}
