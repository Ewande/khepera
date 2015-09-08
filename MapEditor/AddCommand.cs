using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace MapEditor
{
    interface AddCommand
    {
        // params - selected recangle vetrtexes
        SymEnt Exectue(int leftX, int rightX, int upY, int downY, UInt16 suggestedID);
    }
}
