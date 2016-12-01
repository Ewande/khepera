using MathNet.Numerics.LinearAlgebra;
using MathNet.Numerics.LinearAlgebra.Double;
using System;
using System.Collections.Generic;
using System.IO;
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

        public static Layer Load(StreamReader reader)
        {
            Func<double, double> activation = ActFuncs.ByName(reader.ReadLine());

            int[] shape = IOUtils.ReadInts(reader);
            double[][] weights = new double[shape[0]][];
            for (int i = 0; i < shape[0]; i++)
                weights[i] = IOUtils.ReadDoubles(reader);

            reader.ReadLine(); // num of biases - same as num of weights columns
            double[] biases = IOUtils.ReadDoubles(reader);

            return new Layer(DenseMatrix.OfRowArrays(weights), new DenseMatrix(1, biases.Length, biases), activation);
        }
    }
}
