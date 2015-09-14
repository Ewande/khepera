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
    enum ConnectionResult
    {
        IncorrectParameters,
        ServerUnavailable,
        Connected
    }

    class ConnectionManager
    {
        private const int SERVER_PORT_NUMBER = 6020;
        private const int TYPE_ID_VISUALISER = 1;

        private TcpClient _tcpClient;

        public ConnectionManager()
        {
            _tcpClient = new TcpClient();
        }

        public ConnectionResult Connect(string hostname, int miliSecTimeout)
        {
            ConnectionResult result = ConnectionResult.ServerUnavailable;
            try
            {
                _tcpClient.ConnectAsync(hostname, SERVER_PORT_NUMBER).Wait(miliSecTimeout);
            }
            catch(SocketException)
            {
                result = ConnectionResult.ServerUnavailable;
            }
            if (_tcpClient.Connected)
            {
                _tcpClient.GetStream().WriteByte(TYPE_ID_VISUALISER);
                result = ConnectionResult.Connected;
            }
            if (result != ConnectionResult.Connected)
                Disconnect();

            return result;
        }

        public void Disconnect()
        {
            _tcpClient.Close();
            _tcpClient = new TcpClient();
        }

        public SimulationWorld ReciveWorldDesc()
        {
            SimulationWorld result = new SimulationWorld();

            BinaryReader reader = new BinaryReader(_tcpClient.GetStream());

            result.WorldWidth = (UInt32) IPAddress.NetworkToHostOrder(reader.ReadInt32());
            result.WorldHeight = (UInt32) IPAddress.NetworkToHostOrder(reader.ReadInt32());
            result.Time = reader.ReadDouble();
            bool hasBounds = reader.ReadBoolean(); // information not used in visualisation
            int numberOfEntites = (UInt16) IPAddress.NetworkToHostOrder(reader.ReadInt16());

            for (int i = 0; i < numberOfEntites; i++)
            {
                SimEnt entity = EntityReceiver.ReadNext(reader);
                entity.VertFunc = x => result.WorldHeight - x;
                result.Entities.Add(entity.ID, entity);
            }

            byte numberOfControllers = reader.ReadByte();
            for (int i = 0; i < numberOfControllers; i++)
            {
                UInt16 robotId = (UInt16) IPAddress.NetworkToHostOrder(reader.ReadInt16());
                String port = IPAddress.NetworkToHostOrder(reader.ReadInt16()).ToString();
                String ip = reader.ReadByte().ToString() + '.';
                ip += reader.ReadByte().ToString() + '.';
                ip += reader.ReadByte().ToString() + '.';
                ip += reader.ReadByte().ToString();
                SimEnt entity = null;
                if (result.Entities.TryGetValue(robotId, out entity))
                    entity.ControllerAddr = String.Concat(ip, ':', port);
            }

            return result;
        }
    }
}
