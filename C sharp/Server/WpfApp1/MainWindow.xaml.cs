using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Net;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Diagnostics;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Collections.Concurrent;

namespace WpfApp1
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        static public bool isDebug = true;
        private ProgramManager programManager;
        public MainWindow()
        {
            InitializeComponent();
            programManager = new ProgramManager(this);
            //programManager.asyncStartServer(9052);
            programManager.asyncManageQueue();
            if (isDebug) {
                Trace.WriteLine("MainWindow() ended");
            }
        }

        public enum Event {
            ServerEvent,
            ChatEvent
        }
        public class ProgramManager
        {
            private MainWindow mainWindow;
            private Server server;
            public ConcurrentQueue<Tuple<Event, string>> serverEvents;
            public ProgramManager(MainWindow mainWindow)
            {
                this.mainWindow = mainWindow;
                serverEvents = new ConcurrentQueue<Tuple<Event, string>>();
                server = new Server("127.0.0.1", -1, serverEvents);
            }

            public void asyncManageQueue()
            {
                var t = new Thread(manageQueue);
                t.Start();
            }

            public void logServerEvent(string text)
            {
                serverEvents.Enqueue(new Tuple<Event, string>(Event.ServerEvent, text));
            }

            public void manageQueue() {
                while (true)
                {
                    Tuple<Event, string> message;
                    while (!serverEvents.IsEmpty)
                    {
                        bool isSuccessful = serverEvents.TryDequeue(out message);
                        if (!isSuccessful)
                        {
                            continue;
                        }
                        
                        switch (message.Item1)
                        {
                            case Event.ServerEvent:
                                Application.Current.Dispatcher.Invoke(new Action(()
                                    => { mainWindow.EventsTextBox.AppendText(message.Item2); }));
                                break;
                            case Event.ChatEvent:
                                Application.Current.Dispatcher.Invoke(new Action(()
                                    => { mainWindow.ChatTextBox.AppendText(message.Item2); }));
                                break;
                        }
                    }
                    Thread.Sleep(200);
                }
            }


            public void asyncStartServer(int port)
            {
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
                if (isDebug)
                {
                    Trace.WriteLine("Restarting server when ready\n");
                }
                while (!server.getCanRestart())
                {
                    if (isDebug)
                    {
                        Trace.WriteLine("Can not restart\n");
                    }
                    Thread.Sleep(1000);
                }
                asyncStartServer(port);
            }
        }

        void logServerEvent(string text)
        {
            programManager.logServerEvent(text);
        }

        private void StartServerButton_Click(object sender, RoutedEventArgs e)
        {
            if (Int32.TryParse(this.PortTextBox.Text, out int port))
            {
                if (isDebug)
                {
                    Trace.WriteLine("Restarting server\n");
                }
                programManager.stopServer();
                programManager.startServerWhenReady(port);
                if (isDebug)
                {
                    Trace.WriteLine("Finish restarting server\n");
                }
            } else {
            programManager.logServerEvent("Wrong port text\n");
            }
        }

        private void StopServerButton_Click(object sender, RoutedEventArgs e)
        {
            programManager.stopServer();
        }
    }
}
