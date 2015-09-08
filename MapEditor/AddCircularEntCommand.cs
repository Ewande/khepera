using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapEditor
{
    class AddCircularEntCommand : AddCommand
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

            AddRectangularEntWindow dialog = new AddRectangularEntWindow(suggestedID);
            dialog.ShowDialog();
  
            return dialog.WasCanceled ? null : new CircularEnt(suggestedID, dialog.GetWeight(), dialog.GetMovable(), x, y, radius); 
        }
    }
}
