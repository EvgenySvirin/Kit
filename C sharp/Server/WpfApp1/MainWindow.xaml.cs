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
using SQLiteApp;
using Microsoft.EntityFrameworkCore;
using System.Collections.ObjectModel;
using System.Collections.Immutable;
using System.Text.RegularExpressions;

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

            EventsTextBox.IsReadOnly = true;
            ChatTextBox.IsReadOnly = true;

            programManager = new ProgramManager(this);
            programManager.loadDB();
            programManager.startManageQueue();

            
                
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

            private ApplicationContext db;
            private ObservableCollection<Message> DataContext;
            public ProgramManager(MainWindow mainWindow)
            {
                this.mainWindow = mainWindow;
                serverEvents = new ConcurrentQueue<Tuple<Event, string>>();
                server = new Server(this, serverEvents, "127.0.0.1", -1);
            }

            public void startManageQueue()
            {
                var t = new Thread(manageQueue);
                t.Start();
            }

            public void loadDB()
            {
                db = new ApplicationContext();
                // гарантируем, что база данных создана
                db.Database.EnsureCreated();
                // загружаем данные из БД
                db.messages.Load();
                // и устанавливаем данные в качестве контекста
                DataContext = db.messages.Local.ToObservableCollection();
                
                foreach (var entity in db.ChangeTracker.Entries().ToList()) {
                    entity.Reload();
                }
            }
            public void addServerEvent(string text)
            {
                serverEvents.Enqueue(new Tuple<Event, string>(Event.ServerEvent, text));
            }

            public void manageQueue() {
                while (true)
                {
                    Tuple<Event, string> message;
                    while (!serverEvents.IsEmpty)
                    {
                        if (!serverEvents.TryDequeue(out message))
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
                                db.Add(new Message { Text = message.Item2 });
                                db.SaveChanges();
                                break;
                        }
                    }
                    Thread.Sleep(200);
                }
            }
            public void dumpRandomMessages(int n) {
                int skipAmount = new Random().Next(0, this.db.Set<Message>().Count());
                var someMessages = DataContext.Skip(skipAmount).Take(n).ToArray();
                n = Math.Min(n - 1, someMessages.Length - 1);
                while (0 <= n)
                {
                    mainWindow.DumpTextBox.AppendText(someMessages[n].Text);
                    --n;
                }
            }
            public void startServer(int port)
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
                startServer(port);
            }

            public void toggleClientStatus(bool clientStatus)
            {
                Application.Current.Dispatcher.Invoke(new Action(()=> { mainWindow.ClientStatusCheckBox.IsChecked = clientStatus; }));
            }

            public bool sendMessage(string message)
            {
                message = "Server:" + message + "\n";
                return server.sendMessage(message);

            }
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
            programManager.addServerEvent("Wrong port text\n");
            }
        }

        private void StopServerButton_Click(object sender, RoutedEventArgs e)
        {
            programManager.stopServer();
        }

        private void SendMessageButton_Click(object sender, RoutedEventArgs e)
        {
            programManager.sendMessage(MessageTextBox.Text);
        }

        private void DumpButton_Click(object sender, RoutedEventArgs e)
        {
            programManager.dumpRandomMessages(5);
        }
    }
}
