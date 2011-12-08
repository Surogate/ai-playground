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
using Microsoft.Xna.Framework;
using Client2D.View;

namespace Client2D.ViewModel
{
    public class Client : BasicViewModel
    {
        private class StateObject
        {
            public const int BufferSize = 512;
            public byte[] buffer = new byte[BufferSize];
            public StringBuilder sb = new StringBuilder();
        }

        public delegate void ConnectDelegate(string host, string port);

        #region Attributes
        private string address_ = "127.0.0.1";
        private string port_ = "16000";
        private ViewWindow.SheepPopWindow windowS_;
        private ViewWindow.WolfPopWindow windowW_;
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

        public ConnectDelegate ConnectTo
        {
            get;
            set;
        }

        public WolfViewModel WolfPop
        {
            get;
            set;
        }

        public SheepViewModel SheepPop
        {
            get;
            set;
        }

        public XNAView ViewXNA
        {
            get;
            set;
        }
        #endregion

        #region Constructor

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
                    ConnectTo(Address, Port);
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e.Message);
                }
            }
        }

        // Allows the game to exit
        //if (Keyboard.GetState().IsKeyDown(Keys.Up))
        //    Camera.X += 2;
        //if (Keyboard.GetState().IsKeyDown(Keys.Down))
        //    Camera.X -= 2;
        //if (Keyboard.GetState().IsKeyDown(Keys.Right))
        //        Camera.Y+=2;
        //if (Keyboard.GetState().IsKeyDown(Keys.Left))
        //        Camera.Y-=2;

        private void moveCamUp()
        {
            ViewXNA.Game.Camera.Y += 20;
        }

        private void moveCamDown()
        {
            ViewXNA.Game.Camera.Y -= 20;
        }

        private void moveCamLeft()
        {
            ViewXNA.Game.Camera.X += 20;
        }

        private void moveCamRight()
        {
            ViewXNA.Game.Camera.X -= 20;
        }

        private void showPopulationWolf()
        {
            ViewWindow.WolfPopWindow window = new ViewWindow.WolfPopWindow();
            if (windowW_ == null)
                this.WolfPop = (WolfViewModel)window.DataContext;
            if (windowW_ != null)
                windowW_.Close();
            windowW_ = window;
            window.DataContext = this.WolfPop;
            window.Show();
            this.WolfPop.Update();
        }

        private void showPopulationSheep()
        {
            ViewWindow.SheepPopWindow window = new ViewWindow.SheepPopWindow();
            if (windowS_ == null)
                this.SheepPop = (SheepViewModel)window.DataContext;
            if (windowS_ != null)
                windowS_.Close();
            windowS_ = window;
            window.DataContext = this.SheepPop;
            window.Show();
            this.SheepPop.Update();
        }

        #endregion

        #region Events

        #endregion

        #region Commands

        public RelayCommand connect_ = null;
        public RelayCommand up_ = null;
        public RelayCommand down_ = null;
        public RelayCommand left_ = null;
        public RelayCommand right_ = null;
        public RelayCommand populateW_ = null;
        public RelayCommand populateS_ = null;


        public ICommand Connect
        {
            get
            {
                if (connect_ == null)
                    connect_ = new RelayCommand(param => this.connect());
                return (connect_);
            }
        }

        public ICommand Up
        {
            get
            {
                if (up_ == null)
                    up_ = new RelayCommand(param => this.moveCamUp());
                return (up_);
            }
        }

        public ICommand Down
        {
            get
            {
                if (down_ == null)
                    down_ = new RelayCommand(param => this.moveCamDown());
                return (down_);
            }
        }

        public ICommand Left
        {
            get
            {
                if (left_ == null)
                    left_ = new RelayCommand(param => this.moveCamLeft());
                return (left_);
            }
        }

        public ICommand Right
        {
            get
            {
                if (right_ == null)
                    right_ = new RelayCommand(param => this.moveCamRight());
                return (right_);
            }
        }

        public ICommand ShowPopW
        {
            get
            {
                if (populateW_ == null)
                    populateW_ = new RelayCommand(param => this.showPopulationWolf());
                return (populateW_);
            }
        }

        public ICommand ShowPopS
        {
            get
            {
                if (populateS_ == null)
                    populateS_ = new RelayCommand(param => this.showPopulationSheep());
                return (populateS_);
            }
        }

        #endregion
    }
}
