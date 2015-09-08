using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace Visualiser
{
    class ConnectionManager
    {
        private const int SERVER_PORT_NUMBER = 6020;

        private TcpClient _tcpClient;

        public ConnectionManager()
        {
            _tcpClient = new TcpClient();
        }

        public bool Connect(string hostname)
        {
            try
            {
                _tcpClient.Connect(hostname, SERVER_PORT_NUMBER); /* TODO: Catch exception, when connecting fails */
            }
            catch(Exception e)
            {
                Console.WriteLine(e);
            }
            if (_tcpClient.Connected)
                _tcpClient.GetStream().WriteByte(1); // identify as visualiser

            return _tcpClient.Connected;
        }

        public void Disconnect()
        {
            if (_tcpClient.Connected)
            {
                _tcpClient.Close();
                _tcpClient = new TcpClient();
            }
        }

        public SimulationWorld ReciveWorldDesc() // retrives world description from server
        {
            SimulationWorld result = new SimulationWorld();

            NetworkStream stream = _tcpClient.GetStream();
            BinaryReader reader = new BinaryReader(stream);

            result.WorldWidth = (UInt32) IPAddress.NetworkToHostOrder(reader.ReadInt32());
            result.WorldHeight = (UInt32)IPAddress.NetworkToHostOrder(reader.ReadInt32());
            result.Time = reader.ReadDouble();

            int numberOfEntites = (UInt16)IPAddress.NetworkToHostOrder(reader.ReadInt16());

            for (int i = 0; i < numberOfEntites; i++)
            {
                SimEnt entity = EntityReceiver.ReadNext(reader);
                entity.VertFunc = x => result.WorldHeight - x;
                result.Entities.Add(entity.ID, entity);
            }

            return result;
        }
    }
}
