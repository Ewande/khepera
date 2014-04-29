using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapEditor
{
    class AddRectangularEntCommand : AddCommand
    {
        public SymEnt Exectue(int leftX, int rightX, int upY, int downY, UInt16 suggestedID)
        {
            AddRectangularEntWindow dialog = new AddRectangularEntWindow(suggestedID);
            dialog.ShowDialog();
  
            return dialog.WasCanceled ? null :
                new RectangularEnt(suggestedID, dialog.GetWeight(), dialog.GetMovable(), leftX, upY, rightX, upY, rightX, downY, leftX, downY);
        }
    }
}
