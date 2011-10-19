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

namespace ClientXNA
{
    /// <summary>
    /// This is the main type for your game
    /// </summary>
    public class GameClient : Microsoft.Xna.Framework.Game
    {
        private class NetworkData
        {
            public const int BUFFER_SIZE = 512;
            public StringBuilder sb = new StringBuilder();
            public byte[] buffer = new byte[BUFFER_SIZE];
        }


        #region Utils
        private delegate void Command(GameClient gcl, string message);
        #endregion

        #region Attributes
        private TcpClient client_;
        private GraphicsDeviceManager graphics;
        private SpriteBatch spriteBatch;
        private Texture2D   grass_;
        private Vector2 camera_;
        private List<string> incoming_packages_;
        private Command[] commands_ = {new Command(spawn), new Command(move), new Command(eat)
                                    , new Command(die) , new Command(starving) , new Command(clone)
                                    , new Command(board_beg) , new Command(board) , new Command(board_end)};
        private Board board_ = null;
        private Hashtable entities_ = null;
        #endregion

        public GameClient()
        {
            graphics = new GraphicsDeviceManager(this);
            this.graphics.PreferredBackBufferWidth = 600;
            this.graphics.PreferredBackBufferHeight = 480;
            this.IsMouseVisible = true;
            Content.RootDirectory = "Content";
            client_ = new TcpClient("127.0.0.1", 16000);
            incoming_packages_ = new List<string>();
            camera_ = new Vector2();
            entities_ = new Hashtable();
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
            StartRecv();
            base.Initialize();
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
            if (Keyboard.GetState().IsKeyDown(Keys.Escape))
                this.Exit();
            if (Mouse.GetState().LeftButton == ButtonState.Pressed)
            {
                if (Mouse.GetState().X >= 0 && Mouse.GetState().X <= 120)
                    camera_.X++;
                else if (Mouse.GetState().X >= (this.graphics.PreferredBackBufferWidth - 120) && Mouse.GetState().X <= this.graphics.PreferredBackBufferWidth)
                    camera_.X--;
                if (Mouse.GetState().Y >= 0 && Mouse.GetState().Y <= 80)
                    camera_.Y++;
                else if (Mouse.GetState().Y >= (this.graphics.PreferredBackBufferHeight - 80) && Mouse.GetState().Y <= this.graphics.PreferredBackBufferHeight)
                    camera_.Y--;
            }
            // TODO: Add your update logic here
            lock (incoming_packages_)
            {
                while (incoming_packages_.Count > 0)
                {
                    string package = incoming_packages_.First();
                    string[] tokens = package.Split(new char[]{';'});
                    try
                    {
                        int cmd = int.Parse(tokens[0]);
                        StringBuilder sb = new StringBuilder();
                        if (tokens.Count() > 1)
                            commands_[cmd](this, package.Substring(tokens[0].Length + 1));
                        else
                            commands_[cmd](this, package);
                    }
                    catch (Exception e)
                    {
                        Debug.WriteLine(e.Message);
                    }
                    incoming_packages_.RemoveAt(0);
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
            spriteBatch.End();
            base.Draw(gameTime);
        }
        #endregion

        #region Methods
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
                        nd.sb.Append(Encoding.ASCII.GetString(nd.buffer, 0, receiveSize));
                        incoming_packages_.Add(nd.sb.ToString());
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

        private static void spawn(GameClient gcl, string msg)
        {
            string[] tokens = msg.Split(new char[] { ';' });
            if (tokens.Count() < 0)
                return;
            try
            {
                string type = tokens[0];
                int id = int.Parse(tokens[1]);
                int x = int.Parse(tokens[2]);
                int y = int.Parse(tokens[3]);
                if (type == "s")
                {
                    gcl.entities_.Add(id, new Sheep(new Vector2(x, y)));
                }
                else
                {
                    gcl.entities_.Add(id, new Wolf(new Vector2(x, y)));
                }
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }

        private static void move(GameClient gcl, string msg)
        {
            string[] tokens = msg.Split(new char[] { ';' });
            if (tokens.Count() < 0)
                return;
        }

        private static void eat(GameClient gcl, string msg)
        {
            string[] tokens = msg.Split(new char[] { ';' });
            if (tokens.Count() < 0)
                return;
        }

        private static void die(GameClient gcl, string msg)
        {
            string[] tokens = msg.Split(new char[] { ';' });
            if (tokens.Count() < 0)
                return;
            try
            {
                int id = int.Parse(tokens[0]);
                gcl.entities_.Remove(id);
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }

        private static void starving(GameClient gcl, string msg)
        {
            string[] tokens = msg.Split(new char[] { ';' });
            if (tokens.Count() < 0)
                return;
        }

        private static void clone(GameClient gcl, string msg)
        {
            string[] tokens = msg.Split(new char[] { ';' });
            if (tokens.Count() < 0)
                return;
        }

        private static void board_beg(GameClient gcl, string msg)
        {
            string[] tokens = msg.Split(new char[] { ';' });
            if (tokens.Count() < 0)
                return;
            if (gcl.board_ == null)
            {
                try
                {
                    int size = int.Parse(tokens[0]);
                    gcl.board_ = new Board(size);
                }
                catch (Exception e)
                {
                    Debug.WriteLine(e.Message);
                }
            }
        }

        private static void board(GameClient gcl, string msg)
        {
            string[] tokens = msg.Split(new char[] { ';' });
            if (tokens.Count() < 0)
                return;
            try
            {
                int x = int.Parse(tokens[0]);
                int y = int.Parse(tokens[1]);
                int title = int.Parse(tokens[2]);
                gcl.board_.Data[x][y].setHasGrass((title == 1));
            }
            catch (Exception e)
            {
                Debug.WriteLine(e.Message);
            }
        }

        private static void board_end(GameClient gcl, string msg)
        {
            return;
        }

        #endregion
    }
}
