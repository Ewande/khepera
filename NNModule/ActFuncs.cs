using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NNModule
{
    public class ActFuncs
    {
        private static readonly Dictionary<int, Func<double, double>> _FUNC_IDS =
            new Dictionary<int, Func<double, double>>
        {
            {0, Identity},
            {1, Sigmoid}
        };

        public static double Sigmoid(double x)
        {
            return 1.0 / (1.0 + Math.Exp(-x));
        }

        public static double Identity(double x)
        {
            return x;
        }

        public static Func<double, double> CreateStep(double threshold, double negative, double positive)
        {
            return x => x < threshold ? negative : positive;
        }

        public static Func<double, double> GetFuncById(int id)
        {
            Func<double, double> func = null;
            _FUNC_IDS.TryGetValue(id, out func);
            return func;
        }

        public static int GetIdOfFunc(Func<double, double> func)
        {
            return _FUNC_IDS.ContainsValue(func) ?_FUNC_IDS.FirstOrDefault(f => f.Value == func).Key : -1;
        }
    }
}
