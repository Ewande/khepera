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
            _tcpClient.Connect(hostname, SERVER_PORT_NUMBER);

            return _tcpClient.Connected;
        }

        public SymulationWorld ReciveWorldDesc() // retrives world description from server, blocks until whole message is received
        {
            SymulationWorld result = new SymulationWorld();

            NetworkStream stream = _tcpClient.GetStream();
            BinaryReader reader = new BinaryReader(stream);

            result.WorldWidth = (UInt32) IPAddress.NetworkToHostOrder(reader.ReadInt32());
            result.WorldHeight = (UInt32)IPAddress.NetworkToHostOrder(reader.ReadInt32());
            result.Time = (UInt32)IPAddress.NetworkToHostOrder(reader.ReadInt32());

            int numberOfEntites = (UInt16)IPAddress.NetworkToHostOrder(reader.ReadInt16());

            MessageBox.Show("WorldWidth: " + result.WorldWidth + " #entites: " + numberOfEntites);

            for (int i = 0; i < numberOfEntites; i++)
            {
                SymEnt entity = EntityReceiver.ReadNext(reader);
                result.Entities.Add(entity._id, entity);
            }

            return result;
        }
    }
}
