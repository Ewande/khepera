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
            {0, Identity}, {1, Sigmoid}, {2, Relu}, {3, Math.Tanh}
        };

        private static readonly Dictionary<string, Func<double, double>> _FUNC_NAMES =
            new Dictionary<string, Func<double, double>>
        {
            {"identity", Identity}, {"sigmoid", Sigmoid}, {"relu", Relu}, {"tanh", Math.Tanh}
        };

        public static Func<double, double> ById(int id)
        {
            Func<double, double> func = null;
            _FUNC_IDS.TryGetValue(id, out func);
            return func;
        }

        public static Func<double, double> ByName(string name)
        {
            Func<double, double> func = null;
            _FUNC_NAMES.TryGetValue(name, out func);
            return func;
        }

        public static int GetId(Func<double, double> func)
        {
            return _FUNC_IDS.ContainsValue(func) ?_FUNC_IDS.FirstOrDefault(f => f.Value == func).Key : -1;
        }

        public static string GetName(Func<double, double> func)
        {
            return _FUNC_NAMES.ContainsValue(func) ? _FUNC_NAMES.FirstOrDefault(f => f.Value == func).Key : null;
        }

        public static Func<double, double> CreateStep(double threshold, double negative, double positive)
        {
            return x => x < threshold ? negative : positive;
        }

        /*
            Function definitions
        */

        public static double Relu(double x)
        {
            return Math.Max(0, x);
        }

        public static double Sigmoid(double x)
        {
            return 1.0 / (1.0 + Math.Exp(-x));
        }

        public static double Identity(double x)
        {
            return x;
        }
    }
}
