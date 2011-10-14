using System;
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


        #region Attributes
        private TcpClient client_;
        private GraphicsDeviceManager graphics;
        private SpriteBatch spriteBatch;
        #endregion

        public GameClient()
        {
            graphics = new GraphicsDeviceManager(this);
            Content.RootDirectory = "Content";
            client_ = new TcpClient("127.0.0.1", 16000);
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
            // TODO: Add your update logic here

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
                    nd.sb.Append(Encoding.ASCII.GetString(nd.buffer, 0, receiveSize));
                    Debug.WriteLine(nd.sb.ToString());
                    nd.sb.Clear();
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
        #endregion
    }
}
