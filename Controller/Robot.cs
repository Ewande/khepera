using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Controller
{
    class Sensor
    {
        public float State { get; set; }
    };

    class Robot
    {
        public static double DEFAULT_SPEED = 3;

        public bool SpeedChanged { get; set; }
        public double LeftMotorSpeed { get; set; }
        public double RightMotorSpeed { get; set; }
        public List<Sensor> Sensors { get; set; }
    }
}
