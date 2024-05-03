using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace WpfApp1
{
    class ProgramManager
    {
        private Server server;
        private ConcurrentQueue<string> serverEvents;
        public ProgramManager(int port) {
            
            serverEvents = new ConcurrentQueue<string>();
            server = new Server("127.0.0.1", port, serverEvents);
        }

        public void startServer(int port) {
            server.setPort(port);
            var t = new Thread(server.StartListener);
            t.Start();
        }

        public void stopServer()
        {
            server.setIsServing(false);
        }

        public void startServerWhenReady(int port)
        {
            while (!server.getCanRestart())
            {
                Thread.Sleep(1000);
            }
            startServer(port);
        }
    }
}
