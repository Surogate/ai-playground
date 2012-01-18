using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Net.Sockets;
using System.Diagnostics;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Audio;
using Microsoft.Xna.Framework.Content;
using Microsoft.Xna.Framework.GamerServices;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework.Input;
using Microsoft.Xna.Framework.Media;
using System.Windows.Forms;
using System.Windows.Forms.Integration;
using System.Windows.Controls.DataVisualization.Charting;

using ButtonState = Microsoft.Xna.Framework.Input.ButtonState;
using Keys = Microsoft.Xna.Framework.Input.Keys;

namespace ClientXNA
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class GameClient : Microsoft.Xna.Framework.Game
    {
        private class NetworkData
        {
            public const int BUFFER_SIZE = 256;
            public StringBuilder sb = new StringBuilder();
            public byte[] buffer = new byte[BUFFER_SIZE];
        }

        #region Constants
        private const int TIME_TO_WAIT = 10000;
        #endregion

        #region Utils
        private delegate void Command(GameClient gcl, byte[] message);
        #endregion

        #region Attributes
        private TcpClient client_;
        private GraphicsDeviceManager graphics;
        private SpriteBatch spriteBatch;
        private Texture2D   grass_;
        private Vector2 camera_;
        private List<byte[]> incoming_packages_;
        private Command[] commands_ = {new Command(spawn), new Command(move), new Command(eat)
                                    , new Command(die), new Command(clone)
                                    , new Command(board_beg) , new Command(board) , new Command(board_end), new Command(perf)};
        private Board board_ = null;
        private Hashtable entities_ = null;
        private int sheepNumber_;
        private int wolfNumber_;
        private float sheepLastPerf_;
        private float wolfLastPerf_;
        private int elaspedTime_ = 0;
        private int chartTime_ = 0;
        #endregion

        #region WPF

        #region Host
        private ElementHost controlBar_;
        private ElementHost chartHost_;
        #endregion

        #region Models
        ViewModels.ChartsViewModel chartsModels_;
        #endregion

        #region Views
        private Views.ControlBar vControlBar_;
        private Views.Chart vChart_;
        #endregion

        int cdView = 0;
        #endregion

        public GameClient()
        {
            graphics = new GraphicsDeviceManager(this);
            this.graphics.PreferredBackBufferWidth = 1024;
            this.graphics.PreferredBackBufferHeight = 720;
            this.IsMouseVisible = true;
            Content.RootDirectory = "Content";
            incoming_packages_ = new List<byte[]>();
            camera_ = new Vector2();
            entities_ = new Hashtable();
            chartsModels_ = new ViewModels.ChartsViewModel();
        }

        #region OverrideMethods
        /// <summary>
        /// Allows the game to perform any initialization it needs to before starting to run.
        /// This is where it can query for any required services and load any non-graphic
        /// related content.  Calling base.Initialize will enumerate through any components
        /// and initialize them as well.
        /// </summary>
        protected override void Initialize()
        {
            // TODO: Add your initialization logic here
            //StartRecv();
            vControlBar_ = new Views.ControlBar();
            vControlBar_.Connect.Click += new System.Windows.RoutedEventHandler(Connect_Click);
            vControlBar_.CloseChart.Click += new System.Windows.RoutedEventHandler(CloseChart_Click);
            vControlBar_.ChartPopW.Click += new System.Windows.RoutedEventHandler(ChartPopW_Click);
            vControlBar_.ChartPopS.Click += new System.Windows.RoutedEventHandler(ChartPopS_Click);
            vControlBar_.Reset.Click += new System.Windows.RoutedEventHandler(Reset_Click);

            // View LineSeries....
            vChart_ = new Views.Chart();
            vChart_.DataContext = chartsModels_;

            controlBar_ = new ElementHost();
            controlBar_.Size = new System.Drawing.Size(GraphicsDevice.Viewport.Width, 40);
            controlBar_.Child = vControlBar_;
            Control.FromHandle(Window.Handle).Controls.Add(controlBar_);
            chartHost_ = new ElementHost();
            chartHost_.Location = new System.Drawing.Point(0, 40);
            chartHost_.Size = new System.Drawing.Size(GraphicsDevice.Viewport.Width, GraphicsDevice.Viewport.Height - 40);
            chartHost_.Child = vChart_;
            //Control.FromHandle(Window.Handle).Controls.Add(chartHost_);
            //Control.FromHandle(Window.Handle).Controls.Remove(chartHost_);
            base.Initialize();
        }

        void Reset_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            if (board_ == null) return;
            camera_.X = (board_.Size * 32) / 4;
            camera_.Y = (board_.Size * 32) / 10;
        }

        void CloseChart_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            if (Control.FromHandle(Window.Handle).Controls.Contains(chartHost_))
                Control.FromHandle(Window.Handle).Controls.Remove(chartHost_);
        }

        void ChartPopS_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            if (Control.FromHandle(Window.Handle).Controls.Contains(chartHost_))
                Control.FromHandle(Window.Handle).Controls.Remove(chartHost_);
            vChart_.chart.Title = "Sheep Population";
            vChart_.line.ItemsSource = chartsModels_.SheepPopulation;
            Control.FromHandle(Window.Handle).Controls.Add(chartHost_);
        }

        void ChartPopW_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            if (Control.FromHandle(Window.Handle).Controls.Contains(chartHost_))
                Control.FromHandle(Window.Handle).Controls.Remove(chartHost_);
            vChart_.chart.Title = "Wolf Population";
            vChart_.line.ItemsSource = chartsModels_.WolfPopulation;
            Control.FromHandle(Window.Handle).Controls.Add(chartHost_);
        }

        void Connect_Click(object sender, System.Windows.RoutedEventArgs e)
        {
            ConnectTo(vControlBar_.Address.Text, vControlBar_.Port.Text);
        }

        /// <summary>
        /// LoadContent will be called once per game and is the place to load
        /// all of your content.
        /// </summary>
        protected override void LoadContent()
        {
            // Create a new SpriteBatch, which can be used to draw textures.
            spriteBatch = new SpriteBatch(GraphicsDevice);
            grass_ = this.Content.Load<Texture2D>("grass");
            // TODO: use this.Content to load your game content here
        }

        /// <summary>
        /// UnloadContent will be called once per game and is the place to unload
        /// all content.
        /// </summary>
        protected override void UnloadContent()
        {
            // TODO: Unload any non ContentManager content here
        }

        /// <summary>
        /// Allows the game to run logic such as updating the world,
        /// checking for collisions, gathering input, and playing audio.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Update(GameTime gameTime)
        {

            // Allows the game to exit
            if (GamePad.GetState(PlayerIndex.One).Buttons.Back == ButtonState.Pressed)
                this.Exit();
            if (Keyboard.GetState().IsKeyDown(Microsoft.Xna.Framework.Input.Keys.Escape))
                this.Exit();

            #region UIControl
            if (vControlBar_.Address.IsFocused)
            {
                cdView--;
                if (cdView < 0)
                    cdView = 5;
                if (cdView == 0)
                {
                    foreach (Keys key in Keyboard.GetState().GetPressedKeys())
                    {
                        if (key >= Keys.D0 && key <= Keys.D9 || key.ToString().Length == 1 || key == Keys.Decimal || key == Keys.OemPeriod)
                            if (key == Keys.Decimal || key == Keys.OemPeriod)
                                vControlBar_.Address.Text += '.';
                            else
                                vControlBar_.Address.Text += (char)key;
                    }
                }
            }
            if (vControlBar_.Port.IsFocused)
            {
                cdView--;
                if (cdView < 0)
                    cdView = 5;
                if (cdView == 0)
                {
                    foreach (Keys key in Keyboard.GetState().GetPressedKeys())
                    {
                        if (key >= Keys.D0 && key <= Keys.D9 || key.ToString().Length == 1 || key == Keys.Decimal || key == Keys.OemPeriod)
                            if (key == Keys.Decimal || key == Keys.OemPeriod)
                                vControlBar_.Port.Text += '.';
                            else
                                vControlBar_.Port.Text += (char)key;
                    }
                }
            }
            #endregion

            if (Mouse.GetState().LeftButton == ButtonState.Pressed && IsActive)
            {
                if (Mouse.GetState().X >= 0 && Mouse.GetState().X <= 120)
                    camera_.X+=2;
                else if (Mouse.GetState().X >= (this.graphics.PreferredBackBufferWidth - 120) && Mouse.GetState().X <= this.graphics.PreferredBackBufferWidth)
                    camera_.X-=2;
                if (Mouse.GetState().Y >= 41 && Mouse.GetState().Y <= 100)
                    camera_.Y+=2;
                else if (Mouse.GetState().Y >= (this.graphics.PreferredBackBufferHeight - 80) && Mouse.GetState().Y <= this.graphics.PreferredBackBufferHeight)
                    camera_.Y-=2;
            }

            #region ChartsUpdate
            if (client_ != null && client_.Connected)
                elaspedTime_ += gameTime.ElapsedGameTime.Milliseconds;
            if (elaspedTime_ > TIME_TO_WAIT)
            {
                chartTime_ += TIME_TO_WAIT / 1000;
                chartsModels_.WolfPopulation.Add(new ViewModels.ChartsViewModel.ChartModel() { Time = chartTime_, Value = wolfNumber_ });
                chartsModels_.SheepPopulation.Add(new ViewModels.ChartsViewModel.ChartModel() { Time = chartTime_, Value = sheepNumber_ });
                chartsModels_.WolfPerformances.Add(new ViewModels.ChartsViewModel.ChartModel() { Time = chartTime_, Value = wolfLastPerf_ });
                chartsModels_.SheepPerformances.Add(new ViewModels.ChartsViewModel.ChartModel() { Time = chartTime_, Value = sheepLastPerf_ });
                elaspedTime_ = 0;
            }
            #endregion

            #region ManagingPackage
            lock (incoming_packages_)
            {
                while (incoming_packages_.Count > 0)
                {
                    byte[] package = incoming_packages_.First();
                    //string[] tokens = package.Split(new char[]{';'});
                    try
                    {
                        byte cmd = package[0];
                        Debug.WriteLine("Command : " + (int)cmd);
                        commands_[cmd](this, package);
                        //int cmd = int.Parse(tokens[0])
                        //if (tokens.Count() > 1)
                        //    commands_[cmd](this, package.Substring(tokens[0].Length + 1));
                        //else
                        //    commands_[cmd](this, package);
                    }
                    catch (Exception e)
                    {
                        Debug.WriteLine(e.Message);
                    }
                    incoming_packages_.RemoveAt(0);
                }
            }
            #endregion

            lock (entities_)
            {
                IDictionaryEnumerator it = entities_.GetEnumerator();
                while (it.MoveNext())
                {
                    ((Entity)it.Value).Update(gameTime);
                }
            }
            base.Update(gameTime);
        }

        /// <summary>
        /// This is called when the game should draw itself.
        /// </summary>
        /// <param name="gameTime">Provides a snapshot of timing values.</param>
        protected override void Draw(GameTime gameTime)
        {
            GraphicsDevice.Clear(Color.CornflowerBlue);

            // TODO: Add your drawing code here
            spriteBatch.Begin();
            if (board_ != null)
            {
                for (int x = 0; x < board_.Data.Count(); x++)
                    for (int y = 0; y < board_.Data.Count(); y++)
                    {
                        spriteBatch.Draw(grass_, new Rectangle((int)camera_.X + (x * 32), (int)camera_.Y + (y * 32), 32, 32), new Rectangle(0, 0, 32, 32), Color.White);
                        if (board_.Data[x][y].hasGrass())
                            spriteBatch.Draw(grass_, new Rectangle((int)camera_.X + (x * 32), (int)camera_.Y + (y * 32), 32, 32), new Rectangle(32, 0, 32, 32), Color.White);
                    }
            }
            lock (entities_)
            {
                IDictionaryEnumerator it = entities_.GetEnumerator();
                while (it.MoveNext())
                {
                    ((Entity)it.Value).Draw(spriteBatch, camera_);
                }
            }
            spriteBatch.End();
            base.Draw(gameTime);
        }
        #endregion

        #region Methods
        public void ConnectTo(string host, string port)
        {
            try
            {
                if (client_ != null)
                {
                    client_.Close();
                }
                client_ = new TcpClient(host, Int32.Parse(port));
                incoming_packages_.Clear();
                entities_.Clear();
                StartRecv();
                sheepNumber_ = 0;
                wolfNumber_ = 0;
                sheepLastPerf_ = 0;
                wolfLastPerf_ = 0;
                chartsModels_.WolfPopulation = new System.Collections.Concurrent.ConcurrentBag<ViewModels.ChartsViewModel.ChartModel>();
                chartsModels_.WolfPerformances = new System.Collections.Concurrent.ConcurrentBag<ViewModels.ChartsViewModel.ChartModel>();
                chartsModels_.SheepPopulation = new System.Collections.Concurrent.ConcurrentBag<ViewModels.ChartsViewModel.ChartModel>();
                chartsModels_.SheepPerformances = new System.Collections.Concurrent.ConcurrentBag<ViewModels.ChartsViewModel.ChartModel>();
                chartsModels_.WolfPopulation.Add(new ViewModels.ChartsViewModel.ChartModel() { Time = 0, Value = 0 });
                chartsModels_.WolfPerformances.Add(new ViewModels.ChartsViewModel.ChartModel() { Time = 0, Value = 0 });
                chartsModels_.SheepPopulation.Add(new ViewModels.ChartsViewModel.ChartModel() { Time = 0, Value = 0 });
                chartsModels_.SheepPerformances.Add(new ViewModels.ChartsViewModel.ChartModel() { Time = 0, Value = 0 });
                chartTime_ = 0;
            }
            catch (Exception e)
            {

            }
        }

        private void StartRecv()
        {
            try
            {
                if (client_.Connected)
                {
                    NetworkData nd = new NetworkData();
                    client_.Client.BeginReceive(nd.buffer, 0, NetworkData.BUFFER_SIZE, 0, new AsyncCallback(ReceiveCallBack), nd);
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }

        private void ReceiveCallBack(IAsyncResult iar)
        {
            try
            {
                NetworkData nd = (NetworkData)iar.AsyncState;
                int receiveSize = client_.Client.EndReceive(iar);
                if (receiveSize > 0)
                {
                    lock (incoming_packages_)
                    {
                        incoming_packages_.Add(nd.buffer);
                        nd.sb.Clear();
                    }
                    client_.Client.BeginReceive(nd.buffer, 0, NetworkData.BUFFER_SIZE, 0, new AsyncCallback(ReceiveCallBack), nd);
                }
                else
                {
                    StartRecv();
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }

        static Int32 dataToInt32(byte[] msg, int startIndex)
        {
            byte[] array = {
                               msg[startIndex],
                               msg[startIndex + 1],
                               msg[startIndex + 2],
                               msg[startIndex + 3]
                           };
            array.Reverse();

            return BitConverter.ToInt32(array, 0);
        }

        static UInt32 dataToUInt32(byte[] msg, int startIndex)
        {
            byte[] array = {
                               msg[startIndex],
                               msg[startIndex + 1],
                               msg[startIndex + 2],
                               msg[startIndex + 3]
                           };
            array.Reverse();

            return BitConverter.ToUInt32(array, 0);
        }

        private static void spawn(GameClient gcl, byte[] msg)
        {
            try
            {
                int startIndex = 1;
                UInt32 id = dataToUInt32(msg, startIndex);
                startIndex += sizeof(int);
                char type = (char)msg[startIndex];
                startIndex += sizeof(char);
                Int32 x = dataToInt32(msg, startIndex);
                startIndex += sizeof(int);
                Int32 y = dataToInt32(msg, startIndex);

                Debug.WriteLine("Spawn "  + id + " " + type + " " + x + " " + y);

                lock (gcl.entities_)
                {
                    if (type == 0)
                    {
                        gcl.entities_.Add(id, new Sheep(new Vector2(y, x), gcl.Content.Load<Texture2D>("sheep")));
                        gcl.sheepNumber_++;
                    }
                    else
                    {
                        gcl.entities_.Add(id, new Wolf(new Vector2(y, x), gcl.Content.Load<Texture2D>("wolf")));
                        gcl.wolfNumber_++;
                    }
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }

        private static void move(GameClient gcl, byte[] msg)
        {
            //try
            //{
            //    int id = int.Parse(tokens[0]);
            //    int action = int.Parse(tokens[1]);
            //    int x = int.Parse(tokens[2]);
            //    int y = int.Parse(tokens[3]);
            //    ((Entity)gcl.entities_[id]).Action = (Entity.EntityAction)action;
            //    ((Entity)gcl.entities_[id]).NextPosition = new Vector2(y, x);
            //}
            //catch (Exception e)
            //{
            //    Debug.WriteLine(e.Message);
            //}
        }

        private static void eat(GameClient gcl, byte[] msg)
        {
            //string[] tokens = msg.Split(new char[] { ';' });
            //if (tokens.Count() < 0)
            //    return;
        }

        private static void die(GameClient gcl, byte[] msg)
        {
            //try
            //{
            //    lock (gcl.entities_)
            //    {
            //        int id = int.Parse(msg);
            //        if (gcl.entities_[id] is Sheep)
            //        {
            //            gcl.sheepNumber_--;
            //        }
            //        if (gcl.entities_[id] is Wolf)
            //        {
            //            gcl.wolfNumber_--;
            //        }
            //        gcl.entities_.Remove(id);
            //    }
            //}
            //catch (Exception e)
            //{
            //    Debug.WriteLine(e.Message);
            //}
        }

        private static void starving(GameClient gcl, byte[] msg)
        {
            //string[] tokens = msg.Split(new char[] { ';' });
            //if (tokens.Count() < 0)
            //    return;
        }

        private static void clone(GameClient gcl, byte[] msg)
        {
            //string[] tokens = msg.Split(new char[] { ';' });
            //if (tokens.Count() < 0)
            //    return;
            //try
            //{
            //    int id = int.Parse(tokens[0]);
            //    ((Entity)gcl.entities_[id]).Action = Entity.EntityAction.REPRODUCE;
            //}
            //catch (Exception e)
            //{
            //    Debug.WriteLine(e.Message);
            //}
        }

        private static void board_beg(GameClient gcl, byte[] msg)
        {
            try
            {
                Int32 size = dataToInt32(msg, 1);
                Debug.WriteLine("New Board " + size);
                gcl.board_ = new Board(size);
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }

        private static void board(GameClient gcl, byte[] msg)
        {
            try
            {
                int startIndex = 1;
                Int32 x = dataToInt32(msg, startIndex);
                startIndex += sizeof(int);
                Int32 y = dataToInt32(msg, startIndex);
                startIndex += sizeof(int);
                Int32 title = dataToInt32(msg, startIndex);
                startIndex += sizeof(int);
                Int32 odour = dataToInt32(msg, startIndex);
                gcl.board_.Data[y][x].Odour = odour;
                gcl.board_.Data[y][x].setHasGrass((title == 1));
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
            //string[] tokens = msg.Split(new char[] { ';' });
            //if (tokens.Count() < 0)
            //    return;
            //try
            //{
            //    int x = int.Parse(tokens[0]);
            //    int y = int.Parse(tokens[1]);
            //    int title = int.Parse(tokens[2]);
            //    int odour = int.Parse(tokens[3]);
            //    gcl.board_.Data[y][x].Odour = odour;
            //    gcl.board_.Data[y][x].setHasGrass((title == 1));
            //}
            //catch (Exception e)
            //{
            //    Debug.WriteLine(e.Message);
            //}
        }

        private static void board_end(GameClient gcl, byte[] msg)
        {
            return;
        }

        private static void perf(GameClient gcl, byte[] msg)
        {
            //msg = msg.Replace('.', ',');
            //string[] tokens = msg.Split(new char[] { ';' });
            //if (tokens.Count() < 0)
            //    return;
            //try
            //{
            //    float sheep = float.Parse(tokens[0]);
            //    float wolf = float.Parse(tokens[1]);
            //    gcl.sheepLastPerf_ = sheep;
            //    gcl.wolfLastPerf_ = wolf;
            //}
            //catch (Exception e)
            //{
            //    Debug.WriteLine(e.Message);
            //}

        }

        #endregion
    }
}
