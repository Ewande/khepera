using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Controller
{
    public class IOUtils
    {
        public static double[] ReadDoubles(StreamReader reader)
        {
            return reader.ReadLine().Split().Select(x => x.ToDouble()).ToArray();
        }

        public static int[] ReadInts(StreamReader reader)
        {
            return reader.ReadLine().Split().Select(x => int.Parse(x)).ToArray();
        }
    }

    static class IOExtensions
    {
        public static double ToDouble(this string value)
        {
            double result;

            // Try parsing in the current culture
            if (!double.TryParse(value, System.Globalization.NumberStyles.Any, CultureInfo.CurrentCulture, out result) &&
                // Then try in US english
                !double.TryParse(value, System.Globalization.NumberStyles.Any, CultureInfo.GetCultureInfo("en-US"), out result) &&
                // Then in neutral language
                !double.TryParse(value, System.Globalization.NumberStyles.Any, CultureInfo.InvariantCulture, out result))
            {
                result = 0;
            }
            return result;
        }
    }
}
