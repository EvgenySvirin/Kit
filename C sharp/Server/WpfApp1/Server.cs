using System;
using System.Collections.Concurrent;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Diagnostics;

class Server
{
    private bool isServing;
    TcpListener? tcpListener = null;
    private bool canRestart = false;
    private string ip;
    private int port;

    ConcurrentQueue<string> serverEvents;
    public Server(string ip, int port, ConcurrentQueue<string> serverEvents) {
        isServing = true;
        this.ip = ip;
        this.port = port;
        this.serverEvents = serverEvents;
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

    public void StartListener() {
        canRestart = false;
        isServing = true;
        IPAddress localAddr = IPAddress.Parse(ip);
        tcpListener = new TcpListener(localAddr, port);
        tcpListener.Start();
        try
        {
            while (isServing)
            {
                Trace.WriteLine("Waiting for a connection...");

                //if (!tcpListener.AcceptTcpClientAsync().Wait(1000))
                //{
                //    continue;
                //}
                TcpClient client = tcpListener.AcceptTcpClient();


                Trace.WriteLine("Connected!");

                Thread t = new Thread(new ParameterizedThreadStart(HandleDeivce));
                t.Start(client);
            }
        }
        catch (SocketException e)
        {
            Trace.WriteLine("SocketException: {0}", e.ToString());
        }
        finally
        {
            tcpListener.Stop();
            canRestart = true;
            isServing = false;
        }
    }

    public void HandleDeivce(Object obj)
    {

        Trace.WriteLine("hANDLER CALLED");
        TcpClient client = (TcpClient)obj;
        var stream = client.GetStream();
        string imei = String.Empty;

        string data = null;
        Byte[] bytes = new Byte[256];
        int i;
        try
        {

            /*
            while ((i = stream.Read(bytes, 0, bytes.Length)) != 0)
            {
                Trace.WriteLine("hANDLER CALLED 3");
                string hex = BitConverter.ToString(bytes);
                data = Encoding.ASCII.GetString(bytes, 0, i);
                //Trace.WriteLine(string.Format("{1}: Received: {0}", data, Thread.CurrentThread.ManagedThreadId));


                string str = "Hey Device!";
                Byte[] reply = System.Text.Encoding.ASCII.GetBytes(str);
                stream.Write(reply, 0, reply.Length);
                Trace.WriteLine(string.Format("{1}: Sent: {0}", str, Thread.CurrentThread.ManagedThreadId));
                
            }
            */
            Thread.Sleep(10000);
            Trace.WriteLine("hANDLER CALLED2");
            int d = stream.Read(bytes, 0, bytes.Length);
            Trace.WriteLine("hANDLER CALLED22");
            data = Encoding.ASCII.GetString(bytes, 0, d);
            Trace.WriteLine(string.Format("{1}: Received: {0}", data, Thread.CurrentThread.ManagedThreadId));

            Trace.WriteLine("hANDLER CALLED4");
        }
        catch (Exception e)
        {
            Trace.WriteLine("Exception: {0}", e.ToString());
            client.Close();
        }
    }
}
