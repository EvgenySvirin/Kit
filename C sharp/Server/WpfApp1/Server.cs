using System;
using System.Collections.Concurrent;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Diagnostics;
using static WpfApp1.MainWindow;
using System.Threading.Tasks;

class Server
{
    private bool isServing;
    TcpListener? tcpListener = null;
    private bool canRestart = false;
    private string ip;
    private int port;
    private int messageSizeLimit;
    private TcpClient? connectedClient = null;
    private ProgramManager? programManager = null;

    ConcurrentQueue<Tuple<Event, string>> serverEvents;
    public Server(ProgramManager programManager,
                ConcurrentQueue<Tuple<Event, string>> serverEvents,
                string ip, int port, int messageSizeLimit) {
        this.programManager = programManager;
        this.serverEvents = serverEvents;
        this.ip = ip;
        this.port = port;
        this.messageSizeLimit = messageSizeLimit;

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
        
        addServerEvent(String.Format("Server is starting on ip: {0}, port {1}\n", ip, port));
        try
        {
            addServerEvent("Waiting for a connection...\n");
            
            while (isServing)
            {
                tcpListener.Start();
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
                            }
                            catch (SocketException eListener) {
                                Trace.WriteLine("SocketException: {0}", eListener.ToString());
                            }
                        }
                    });

                if (!task.Wait(2000)) {
                    tcpListener.Stop();
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
                tcpListener.Stop();
            }
        }
        catch (SocketException e)
        {
            Trace.WriteLine("SocketException: {0}", e.ToString());
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
        Byte[] bytes = new Byte[messageSizeLimit];
        int i;
        try
        {
            while (isServing && (i = stream.Read(bytes, 0, bytes.Length)) != 0)
            {
                addChatEvent(Encoding.ASCII.GetString(bytes, 0, i));
            }
        }
        catch (Exception e)
        {
            Trace.WriteLine("Exception: {0}", e.ToString());
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
                Trace.WriteLine("CLOSE SENT\n");
            }

            client.GetStream().Write(Encoding.ASCII.GetBytes("CLOSE"));
        }
    } 

    public bool sendMessage(string message)
    {
        if (connectedClient == null || !isServing || !connectedClient.Connected) {
            addServerEvent("No connection\n");
            return false;
        }
        if (200 < message.Length) {
            addServerEvent("Message is too large");
            return false;
        }
        connectedClient.GetStream().Write(Encoding.ASCII.GetBytes(message));
        addChatEvent(message);
        return true;
    }
}
