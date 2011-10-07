using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Windows.Input;
using System.Net.Sockets;
using System.Diagnostics;
using System.Threading;
using Client2D.Utils;

namespace Client2D.ViewModel
{
    class Client : BasicViewModel
    {
        private class StateObject
        {
            public const int BufferSize = 64;
            public byte[] buffer = new byte[BufferSize];
            public StringBuilder sb = new StringBuilder();
        }

        #region Attributes
        private string address_ = "127.0.0.1";
        private string port_ = "16000";
        private string command_;
        private List<string> logs_ = new List<string>();
        private ObservableCollection<string> logs_printed_;
        private TcpClient client_ = null;
        #endregion

        #region Properties

        public string Address
        {
            get
            {
                return (address_);
            }
            set
            {
                if (address_ != value)
                    address_ = value;
                OnPropertyChanged("Address");
            }
        }

        public string Port
        {
            get
            {
                return (port_);
            }
            set
            {
                if (port_ != value)
                    port_ = value;
                OnPropertyChanged("Port");
            }
        }

        public string Command
        {
            get
            {
                return (command_);
            }
            set
            {
                if (command_ != value)
                    command_ = value;
                OnPropertyChanged("Command");
            }
        }

        public ObservableCollection<string> Logs
        {
            get
            {
                return logs_printed_;
            }
            set
            {
                logs_printed_ = value;
                OnPropertyChanged("Logs");
            }
        }


        #endregion

        #region Methods

        private void connect()
        {
            Debug.WriteLine("connect()");
            Debug.WriteLine("Address : " + Address + " Port : " + Port);
            if (Port != null && Address != null)
            {
                try
                {
                    Int32 port = Int32.Parse(Port);
                    if (client_ != null)
                        client_.Close();
                    client_ = new TcpClient(Address, port);
                    start_recv();
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e.Message);
                }
            }
        }

        private void send()
        {
            Debug.WriteLine("send()");
        }

       

        private void start_recv()
        {
            try
            {
                if (client_.Connected)
                {
                    StateObject state = new StateObject();
                    client_.Client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReceiveCallback), state);
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                client_.Close();
            }
        }

        private void ReceiveCallback(IAsyncResult ar)
        {
            try
            {
                StateObject state = (StateObject)ar.AsyncState;

                int bytesRead = client_.Client.EndReceive(ar);
                if (bytesRead > 0)
                {
                    state.sb.Append(Encoding.ASCII.GetString(state.buffer, 0, bytesRead));
                    logs_.Add(state.sb.ToString());
                    state.sb.Clear();
                    if (logs_.Count > 10)
                    {
                        for (int i = 0; i < 5; i++)
                            logs_.RemoveAt(i);
                    }
                    Logs = new ObservableCollection<string>(logs_);
                    client_.Client.BeginReceive(state.buffer, 0, StateObject.BufferSize, 0, new AsyncCallback(ReceiveCallback), state);
                }
                else
                {
                    start_recv();
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
                client_.Close();
            }
        }

        #endregion

        #region Events

        #endregion

        #region Commands

        public RelayCommand connect_ = null;
        public RelayCommand send_ = null;

        public ICommand Connect
        {
            get
            {
                if (connect_ == null)
                    connect_ = new RelayCommand(param => this.connect());
                return (connect_);
            }
        }

        public ICommand Send
        {
            get
            {
                if (send_ == null)
                    send_ = new RelayCommand(param => this.send());
                return (send_);
            }
        }

        #endregion
    }
}
