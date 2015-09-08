using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapEditor
{
    class AddKheperaRobotCommand : AddCommand
    {
        public SymEnt Exectue(int leftX, int rightX, int upY, int downY, UInt16 suggestedID)
        {
            int radius = 0;
            int x = 0;  
            int y = 0;
            int width = rightX - leftX;
            int height = downY - upY;

            radius = Math.Min(width, height) / 2;
            x = leftX + radius;
            y = upY + radius;

            AddKheperaRobotWindow dialog = new AddKheperaRobotWindow(suggestedID);
            dialog.ShowDialog();

            return dialog.WasCanceled ? null : new KheperaRobot(suggestedID, dialog.GetWeight(), dialog.GetMovable(), x, y, radius,
                dialog.GetWheelRadius(), dialog.GetWheelDistance(), dialog.GetStartingAngle());
        }
    }
}
