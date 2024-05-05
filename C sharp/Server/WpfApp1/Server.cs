using System;
using System.Collections.Concurrent;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Diagnostics;
using static WpfApp1.MainWindow;
using System.Threading.Tasks;
using System.Diagnostics.Eventing.Reader;
using System.Linq.Expressions;

class Server
{
    private bool isServing;
    TcpListener? tcpListener = null;
    private bool canRestart = false;
    private string ip;
    private int port;
    private TcpClient? connectedClient = null;
    private ProgramManager programManager = null;

    ConcurrentQueue<Tuple<Event, string>> serverEvents;
    public Server(ProgramManager programManager,
                ConcurrentQueue<Tuple<Event, string>> serverEvents,
                string ip, int port) {
        this.programManager = programManager;
        this.serverEvents = serverEvents;
        this.ip = ip;
        this.port = port;

        isServing = false;
        canRestart = true;
    }

    public void setPort(int port) {
        this.port = port;
    }

    public void setIsServing(bool isServing) {
        this.isServing = isServing;
    }

    public Boolean getCanRestart() {
        return canRestart;
    }

    void addServerEvent(string text) {
        serverEvents.Enqueue(new Tuple<Event, string>(Event.ServerEvent, text));
    }
    void addChatEvent(string text) {
        serverEvents.Enqueue(new Tuple<Event, string>(Event.ChatEvent, text));
    }
    public void StartListener() {
        connectedClient = null;
        canRestart = false;
        isServing = true;
        IPAddress localAddr = IPAddress.Parse(ip);
        tcpListener = new TcpListener(localAddr, port);
        tcpListener.Start();
        addServerEvent(String.Format("Server is starting on ip: {0}, port {1}\n", ip, port));
        try
        {
            addServerEvent("Waiting for a connection...\n");
            while (isServing)
            {
                TcpClient? client = null;
                var task = Task.Factory.StartNew(
                    () =>
                    {
                        try
                        {
                            client = tcpListener.AcceptTcpClient();
                        }
                        catch (SocketException e)
                        {
                            try
                            {
                                tcpListener.Stop();
                                tcpListener.Start();
                            }
                            catch (SocketException eListener) {
                                Trace.WriteLine("SocketException: {0}", eListener.ToString());
                            }
                            //Trace.WriteLine("SocketException: {0}", e.ToString());
                        }
                    });

                bool isSuccessful = false;
                try {
                    isSuccessful = task.Wait(2000);
                } catch (SocketException e) {
                    tcpListener.Stop();
                    tcpListener.Start();
                    Thread.Sleep(300);
                    continue;
                }

                if (!isSuccessful) {
                    tcpListener.Stop();
                    tcpListener.Start();
                    Thread.Sleep(300);
                    continue;
                }

                if (client != null)
                {
                    addServerEvent("Connected\n");
                    connectedClient = client;
                    Thread t = new Thread(new ParameterizedThreadStart(HandleDeivce));
                    t.Start(parameter: client);
                    programManager.toggleClientStatus(true);              
                }
            }
        }
        catch (SocketException e)
        {
            //Trace.WriteLine("SocketException: {0}", e.ToString());
        }
        finally
        {
            sendClose(connectedClient);
            if (isDebug)
            {
                Trace.WriteLine("Server loop ended\n");
            }
            addServerEvent("Server stopped\n");
            tcpListener.Stop();
            connectedClient = null;
            isServing = false;
            canRestart = true;
            programManager.toggleClientStatus(false);
        }
    }

    public void HandleDeivce(Object obj)
    {
        if (isDebug) {
            Trace.WriteLine("Handler is called");
        }
        TcpClient client = (TcpClient)obj;
        var stream = client.GetStream();
        Byte[] bytes = new Byte[256];
        int i;
        try
        {
            while (isServing && (i = stream.Read(bytes, 0, bytes.Length)) != 0)
            {
                addChatEvent(Encoding.Default.GetString(bytes, 0, i));
            }
        }
        catch (Exception e)
        {
            //Trace.WriteLine("Exception: {0}", e.ToString());
        }
        finally
        {
            sendClose(client);
            client.Close();
            addServerEvent("Client disconnected\n");
            programManager.toggleClientStatus(false);
        }
    }

    public void sendClose(TcpClient client) {
        if (client != null && client.Connected)
        {
            if (isDebug)
            {
                Trace.WriteLine("CLOSE SENT \n");
            }

            client.GetStream().Write(Encoding.Default.GetBytes("CLOSE"));
        }
    } 

    public bool sendMessage(string message)
    {
        if (connectedClient == null || !isServing) {
            return false;
        }
        connectedClient.GetStream().Write(Encoding.Default.GetBytes(message));
        addChatEvent(message);
        return true;
    }
}
