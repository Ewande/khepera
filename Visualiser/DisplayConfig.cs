using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Visualiser
{
    public class DisplayConfig
    {
        private static DisplayConfig instance;

        public static DisplayConfig Instance
        {
            get
            {
                return instance ?? (instance = new DisplayConfig());
            }
        }

        public bool ShowSensorRange { get; set; }
        public bool ShowId { get; set; }

        private DisplayConfig()
        {
            ShowSensorRange = false;
            ShowId = false;
        }
        
    }
}
