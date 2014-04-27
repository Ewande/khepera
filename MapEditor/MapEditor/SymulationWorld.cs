using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MapEditor
{
    class SymulationWorld
    {
        public SymulationWorld(UInt32 width, UInt32 height)
        {
            _worldWidth = width;
            _worldHeight = height;
            _time = 0;

            _entities = new List<SymEnt>();
        }

        public void AddEnt(SymEnt newEnt)
        {
            _entities.Add(newEnt);
        }

        public void SaveToFile(String filename)
        {
            FileStream fs = File.Create(filename);
            BinaryWriter writer = new BinaryWriter(fs);

            writer.Write(_worldWidth);
            writer.Write(_worldHeight);
            writer.Write(_time);

            UInt16 numOfEntities = (UInt16) _entities.Count();
            writer.Write(numOfEntities);

            foreach (SymEnt ent in _entities)
                ent.SaveToFile(writer);
        }

        private List<SymEnt>    _entities;
        private UInt32          _worldWidth;
        private UInt32          _worldHeight;
        private UInt32          _time;

    }
}
