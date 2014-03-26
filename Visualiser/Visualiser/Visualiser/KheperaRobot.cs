using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Visualiser
{
    class KheperaRobot : CircularEnt
    {
        public UInt16 WheelRaduis { get; set;}
        public UInt16 WheelDistance { get; set; }
        public float DirectionAngle { get; set; }

        public KheperaRobot(UInt16 id, UInt32 weight, bool movable, double x, 
            double y, double radius, UInt16 wheelRadius, UInt16 wheelDistance, float directionAngle) : 
            base(id, weight, movable, x, y, radius)
        {
            WheelRaduis = wheelRadius;
            WheelDistance = wheelDistance;
            DirectionAngle = directionAngle;
        }

        /* TODO: Change drawing procedure, so that user will be able to distinguish betwin robots and static circles */
    }
}
