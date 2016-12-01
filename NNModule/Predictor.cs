using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NNModule
{
    public abstract class Predictor
    {
        public abstract List<double> Predict(List<double> input);
    }
}
