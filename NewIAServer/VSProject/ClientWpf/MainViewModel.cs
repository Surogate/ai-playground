using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Input;
using System.Net.Sockets;
using System.Diagnostics;
using System.Collections.ObjectModel;
using System.Windows;

namespace ClientWpf
{
    public class MainViewModel : BasicViewModel
    {
        #region InnerClass
        public class NetworkData
        {
            public const int BUFFER_SIZE = 256;
            public byte[] buffer = new byte[BUFFER_SIZE];
        }

        public class Metric
        {
            public DateTime Time { get; set; }
            public int WolfNumber { get; set; }
            public double WolfMoy { get; set; }
            public int WolfActionNumber { get; set; }
            public int WolfActionNeural { get; set; }
            public int SheepNumber { get; set; }
            public double SheepMoy { get; set; }
            public int SheepActionNumber { get; set; }
            public int SheepActionNeural { get; set; }
        }
        #endregion

        #region Attributes
        private string ip_;
        private bool wolf_num_line_ = true;
        private bool wolf_moy_line_ = true;
        private bool wolf_act_num_line_ = true;
        private bool wolf_act_neu_line_ = true;
        private bool sheep_num_line_ = true;
        private bool sheep_moy_line_ = true;
        private bool sheep_act_num_line_ = true;
        private bool sheep_act_neu_line_ = true;


        private TcpClient client_;
        private NetworkStream stream_;
        private int packet_size_;

        private ICommand connect_;
        private ICommand clear_;
        #endregion

        #region Constructor
        public MainViewModel()
        {
            IP = "127.0.0.1";
            connect_ = new RelayCommand(param => this.connect());
            clear_ = new RelayCommand(param => this.clear());
            Metrics = new SafeObservableCollection<Metric>();
            packet_size_ = 0;
            client_ = null;
            stream_ = null;
        }
        #endregion

        #region Properties
        public string IP
        {
            get
            {
                return ip_;
            }

            set
            {
                if (ip_ != value)
                    ip_ = value;
                OnPropertyChanged("IP");
            }
        }

        public SafeObservableCollection<Metric> Metrics
        {
            get;
            set;
        }

        public bool WolfNumLine
        {
            get
            {
                return (wolf_num_line_);
            }
            set
            {
                if (wolf_num_line_ != value)
                    wolf_num_line_ = value;
                if (!wolf_num_line_)
                {
                    WolfNumVisibility = Visibility.Hidden;
                }
                else
                    WolfNumVisibility = Visibility.Visible;
                OnPropertyChanged("WolfNumVisibility");
                OnPropertyChanged("WolfNumLine");
            }
        }
        public bool WolfMoyLine
        {
            get
            {
                return (wolf_moy_line_);
            }
            set
            {
                if (wolf_moy_line_ != value)
                    wolf_moy_line_ = value;
                if (!wolf_moy_line_)
                {
                    WolfMoyVisibility = Visibility.Hidden;
                }
                else
                    WolfMoyVisibility = Visibility.Visible;
                OnPropertyChanged("WolfMoyVisibility");
                OnPropertyChanged("WolfMoyLine");
            }
        }

        public bool WolfActNumLine
        {
            get
            {
                return (wolf_act_num_line_);
            }

            set
            {
                if (wolf_act_num_line_ != value)
                    wolf_act_num_line_ = value;
                if (!wolf_act_num_line_)
                {
                    WolfActNumVisibility = Visibility.Hidden;
                }
                else
                    WolfActNumVisibility = Visibility.Visible;
                OnPropertyChanged("WolfActNumVisibility");
                OnPropertyChanged("WolfActNumLine");
            }
        }

        public bool WolfActNeuLine
        {
            get
            {
                return (wolf_act_neu_line_);
            }

            set
            {
                if (wolf_act_neu_line_ != value)
                    wolf_act_neu_line_ = value;
                if (!wolf_act_neu_line_)
                {
                    WolfActNeuVisibility = Visibility.Hidden;
                }
                else
                    WolfActNeuVisibility = Visibility.Visible;
                OnPropertyChanged("WolfActNeuVisibility");
                OnPropertyChanged("WolfActNeuLine");
            }
        }

        public bool SheepNumLine
        {
            get
            {
                return (sheep_num_line_);
            }
            set
            {
                if (sheep_num_line_ != value)
                    sheep_num_line_ = value;
                if (!sheep_num_line_)
                {
                    SheepNumVisibility = Visibility.Hidden;
                }
                else
                    SheepNumVisibility = Visibility.Visible;
                OnPropertyChanged("SheepNumVisibility");
                OnPropertyChanged("SheepNumLine");
            }
        }

        public bool SheepMoyLine
        {
            get
            {
                return (sheep_moy_line_);
            }
            set
            {
                if (sheep_moy_line_ != value)
                    sheep_moy_line_ = value;
                if (!sheep_moy_line_)
                {
                    SheepMoyVisibility = Visibility.Hidden;
                }
                else
                    SheepMoyVisibility = Visibility.Visible;
                OnPropertyChanged("SheepMoyVisibility");
                OnPropertyChanged("SheepMoyLine");
            }
        }

        public bool SheepActNumLine
        {
            get
            {
                return (sheep_act_num_line_);
            }

            set
            {
                if (sheep_act_num_line_ != value)
                    sheep_act_num_line_ = value;
                if (!sheep_act_num_line_)
                {
                    SheepActNumVisibility = Visibility.Hidden;
                }
                else
                    SheepActNumVisibility = Visibility.Visible;
                OnPropertyChanged("SheepActNumVisibility");
                OnPropertyChanged("SheepActNumLine");
            }
        }

        public bool SheepActNeuLine
        {
            get
            {
                return (sheep_act_neu_line_);
            }

            set
            {
                if (sheep_act_neu_line_ != value)
                    sheep_act_neu_line_ = value;
                if (!sheep_act_neu_line_)
                {
                    SheepActNumVisibility = Visibility.Hidden;
                }
                else
                    SheepActNumVisibility = Visibility.Visible;
                OnPropertyChanged("SheepActNeuVisibility");
                OnPropertyChanged("SheepActNeuLine");
            }
        }

        public Visibility WolfNumVisibility { get; set; }
        public Visibility WolfMoyVisibility { get; set; }
        public Visibility WolfActNumVisibility { get; set; }
        public Visibility WolfActNeuVisibility { get; set; }
        public Visibility SheepNumVisibility { get; set; }
        public Visibility SheepMoyVisibility { get; set; }
        public Visibility SheepActNumVisibility { get; set; }
        public Visibility SheepActNeuVisibility { get; set; }


        #endregion

        #region Commands
        public ICommand Connect
        {
            get
            {
                return (connect_);
            }
            set
            {
                connect_ = value;
            }
        }

        public ICommand Clear
        {
            get
            {
                return (clear_);
            }
            set
            {
                clear_ = value;
            }
        }
        #endregion

        #region Methods
        private void clear()
        {
            if (Metrics != null)
                Metrics.Clear();
        }

        private void connect()
        {
            try
            {
                if (client_ != null)
                {
                    stream_.Close();
                    client_.Close();
                    client_ = null;
                    stream_ = null;
                    clear();
                }

                if (ip_ != null && ip_.Length > 0)
                {
                    client_ = new TcpClient(ip_, 16500);
                    stream_ = client_.GetStream();
                    startRecv();
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.StackTrace);
            }
        }

        private void startRecv()
        {
            NetworkData netData = new NetworkData();
            stream_.BeginRead(netData.buffer, 0, NetworkData.BUFFER_SIZE, new AsyncCallback(ReceivedCallback), netData);
        }

        private void ReceivedCallback(IAsyncResult state)
        {
            try
            {
                NetworkData netData = (NetworkData)state.AsyncState;
                int recv_size = stream_.EndRead(state);
                if (recv_size > 0)
                {
                    byte[] recv = new byte[recv_size];
                    for (int i = 0; i < recv_size; i++)
                        recv[i] = netData.buffer[i];
                    if (recv_size == 4)
                    {
                        Array.Reverse(recv);
                        packet_size_ = BitConverter.ToInt32(recv, 0); 
                    }
                    if (recv_size == packet_size_)
                    {
                        int position = 0;
                        Metric metric = new Metric();
                        metric.Time = DateTime.Now;
                        metric.WolfNumber = getInt(netData.buffer, ref position);
                        metric.WolfMoy = getDouble(netData.buffer, ref position);
                        metric.WolfActionNumber = getInt(netData.buffer, ref position);
                        metric.WolfActionNeural = getInt(netData.buffer, ref position);
                        metric.SheepNumber = getInt(netData.buffer, ref position);
                        metric.SheepMoy = getDouble(netData.buffer, ref position);
                        metric.SheepActionNumber = getInt(netData.buffer, ref position);
                        metric.SheepActionNeural = getInt(netData.buffer, ref position);

                        Debug.WriteLine("Wolf : " + metric.WolfNumber);
                        Debug.WriteLine("Wolf Moy : " + metric.WolfMoy);
                        Debug.WriteLine("Wolf Action : " + metric.WolfActionNumber);
                        Debug.WriteLine("Wolf Neural : " + metric.WolfActionNeural);
                        Debug.WriteLine("Sheep : " + metric.SheepNumber);
                        Debug.WriteLine("Sheep Moy : " + metric.SheepMoy);
                        Debug.WriteLine("Sheep Action : " + metric.SheepActionNumber);
                        Debug.WriteLine("Sheep Neural : " + metric.SheepActionNeural);
                        Metrics.Add(metric);
                        OnPropertyChanged("Metrics");
                    }
                    Debug.WriteLine("recv size : " + packet_size_);
                    stream_.BeginRead(netData.buffer, 0, NetworkData.BUFFER_SIZE, new AsyncCallback(ReceivedCallback), netData);
                }
                else
                {
                    startRecv();
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.StackTrace);
            }
        }

        private int getInt(byte[] data, ref int position)
        {
            byte[] toConv = new byte[sizeof(int)];
            for (int i = position; i < position + sizeof(int); i++)
            {
                toConv[i - position] = data[i];
            }
            Array.Reverse(toConv);
            position += sizeof(int);
            return BitConverter.ToInt32(toConv, 0);
        }

        private double getDouble(byte[] data, ref int position)
        {
            byte[] toConv = new byte[sizeof(double)];
            for (int i = position; i < position + sizeof(double); i++)
            {
                toConv[i - position] = data[i];
            }
            position += sizeof(double);
            return BitConverter.ToDouble(toConv, 0);
        }
        #endregion

    }
}
