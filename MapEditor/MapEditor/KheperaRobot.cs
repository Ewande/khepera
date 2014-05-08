using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapEditor
{
    class KheperaRobot : CircularEnt
    {
        public UInt16 WheelRaduis { get; set; }
        public UInt16 WheelDistance { get; set; }
        public double DirectionAngle { get; set; }

        public KheperaRobot(UInt16 id, UInt32 weight, bool movable, double x,
            double y, double radius, UInt16 wheelRadius, UInt16 wheelDistance, double directionAngle) :
            base(id, weight, movable, x, y, radius)
        {
            WheelRaduis = wheelRadius;
            WheelDistance = wheelDistance;
            DirectionAngle = directionAngle;
            ShapeID = SymEnt.KHEPERA_ROBOT_ID;
        }

        public override void SaveToFile(BinaryWriter writer)
        {
            base.SaveToFile(writer);

            writer.Write(WheelRaduis);
            writer.Write(WheelDistance);
            writer.Write(DirectionAngle);
        }

        /* TODO: Change drawing procedure, so that user will be able to distinguish betwin robots and static circles */
    }
}
