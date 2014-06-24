using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using JobManagerFramework;
using System.Runtime.Remoting.Channels;
using System.Runtime.Remoting;
using System.Net.Sockets;

namespace MasterInterpreterTest
{
    public class JobManagerFixture
    {
        JobServerImpl server;
        ObjRef serverRef;
        System.Runtime.Remoting.Channels.Tcp.TcpServerChannel chan;
        public JobManagerFixture()
        {
            BinaryServerFormatterSinkProvider serverProv = new BinaryServerFormatterSinkProvider();
            serverProv.TypeFilterLevel = System.Runtime.Serialization.Formatters.TypeFilterLevel.Full;

            System.Collections.IDictionary ClientTcpChannelProperties = new System.Collections.Hashtable();
            ClientTcpChannelProperties["name"] = "ClientTcpChan";
            ChannelServices.RegisterChannel(
                    new System.Runtime.Remoting.Channels.Tcp.TcpClientChannel(ClientTcpChannelProperties, new BinaryClientFormatterSinkProvider()), false);

            int port = 35010;

            System.Collections.IDictionary TcpChannelProperties = new System.Collections.Hashtable();
            TcpChannelProperties["port"] = port;
            try
            {
                chan = new System.Runtime.Remoting.Channels.Tcp.TcpServerChannel(TcpChannelProperties, serverProv);
                ChannelServices.RegisterChannel(chan, false);
                server = new JobServerImpl();
                serverRef = RemotingServices.Marshal(server, "JobServer");
                server.handlersAdded.Set();
            }
            catch (SocketException e)
            {
                if (!e.Message.StartsWith("Only one usage of each socket address"))
                {
                    throw;
                }
            }
        }
    }
}
