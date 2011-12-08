using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework.Graphics;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Input;
using System.Diagnostics;

namespace ClientXNA
{
    class GUI
    {
        private Texture2D background_;
        private StringBuilder command_;
        private bool keypressed = false;

        public delegate void Connect(string host, string port);

        public Connect ConnectTo
        { 
            get;
            set; 
        }

        public bool Visible
        {
            get;
            set;
        }

        public SpriteFont Font
        {
            get;
            set;
        }

        public int Width
        {
            get;
            set;
        }

        public int Height
        {
            get;
            set;
        }

        public int WolfNumberALive
        {
            get;
            set;
        }

        public int WolfNumberDeath
        {
            get;
            set;
        }

        public int SheepNumberALive
        {
            get;
            set;
        }

        public int SheepNumberDeath
        {
            get;
            set;
        }

        public GUI()
        {
            
        }

        public void Init(GraphicsDevice GraphicsDevice)
        {
            keypressed = false;
            command_ = new StringBuilder();
            background_ = new Texture2D(GraphicsDevice, Width, Height / 2);
            Color[] data = new Color[Width * (Height / 2)];
            for (int i = 0; i < data.Length; i++)
                data[i] = new Color(0, 0, 0, 0.80f);
            background_.SetData(data);
        }
       
        public void Update(GameTime gametime)
        {
            try
            {
                Keys[] keys = Keyboard.GetState().GetPressedKeys();
                if (keys.Count() > 0 && !keypressed)
                {
                    if (Keyboard.GetState().IsKeyDown(Keys.Enter))
                    {
                        string[] tokens = command_.ToString().Split(' ');
                        command_.Clear();
                        if (tokens.Count() >= 1)
                        {
                            if (tokens[0].Equals("CONNECT") && tokens.Count() >= 3)
                                ConnectTo(tokens[1], tokens[2]);
                            else if (tokens[0].Equals("CONNECT") && tokens.Count() == 2)
                                ConnectTo(tokens[1], "16000");
                            else if (tokens[0].Equals("CONNECT"))
                                ConnectTo("localhost", "16000");
                        }
                    }
                    if (keys.Count() == 1)
                    {
                        if (keys[0] == Keys.Back)
                        {
                            int pos = command_.Length - 1;
                            if (command_.Length > 0)
                                command_.Remove(pos, 1);
                        }
                        int keynum = (int)keys[0];
                        if ((keynum >= 65 && keynum <= 90) || (keynum >= 48 && keynum <= 57))
                            command_.Append((char)keynum);
                        if (keynum == 32)
                            command_.Append((char)keynum);
                        if (keys[0] == Keys.RightShift)
                            command_.Append(".");
                    }
                    keypressed = true;
                }
                else if (keys.Count() == 0)
                    keypressed = false;
            }
            catch (Exception e)
            {

            }
        }

        public void Draw(SpriteBatch graphics)
        {
            graphics.Draw(background_, new Rectangle(0, 0, Width, Height / 2), Color.White);
            graphics.DrawString(Font, "Sheep a live :  " + SheepNumberALive + " | Sheep dead : " + SheepNumberDeath, new Vector2(5, 10), Color.White);
            graphics.DrawString(Font, "Wolf a live : " + WolfNumberALive + " | Wolf dead : " + WolfNumberDeath, new Vector2(5, 10+Font.LineSpacing), Color.White);
            graphics.DrawString(Font, "> " + command_.ToString(), new Vector2(5, (Height / 2) - Font.LineSpacing), Color.White);
        }

        
    }
}
