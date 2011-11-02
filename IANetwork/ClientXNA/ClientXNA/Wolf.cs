using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace ClientXNA
{
    public class Wolf : Entity
    {
        #region Attributes
        private Texture2D image_;
        private Rectangle[][] frames_;
        private int x_frame_;
        private int y_frame_;
        private int width_;
        private int height_;
        private int elapsedTime_;
        private int elapsedTimeR_;
        #endregion

        public Wolf(Vector2 position, Texture2D image) : base(position)
        {
            image_ = image;
            int width = image_.Bounds.Width / 4;
            int height = image_.Bounds.Height / 4;
            frames_ = new Rectangle[4][];
            for (int y = 0; y < 4; y++)
            {
                frames_[y] = new Rectangle[4];
                for (int x = 0; x < 4; x++)
                    frames_[y][x] = new Rectangle(x* width, y* height, width, height);
                if (Action == EntityAction.MOVE_UP)
                    if (Position.Y > NextPosition.Y)
                        position_.Y -= 0.1f;
                if (Action == EntityAction.MOVE_DOWN)
                    if (Position.Y < NextPosition.Y)
                        position_.Y += 0.1f;
                if (Action == EntityAction.MOVE_LEFT)
                    if (Position.X > NextPosition.X)
                        position_.X -= 0.1f;
                if (Action == EntityAction.MOVE_RIGHT)
                    if (Position.X < NextPosition.X)
                        position_.X += 0.1f;
            }
            x_frame_ = 0;
            y_frame_ = 0;
            width_ = width;
            height_ = height;
            elapsedTime_ = 0;
            elapsedTimeR_ = 0;
        }

        public override void Update(GameTime gameTime)
        {
            y_frame_ = (this.Action != EntityAction.EAT &&
                       this.Action != EntityAction.REPRODUCE &&
                       this.Action != EntityAction.NONE) ?
                        ((int)this.Action) : ((int)EntityAction.MOVE_DOWN);
            if (elapsedTime_ - gameTime.ElapsedGameTime.Milliseconds > 150)
            {
                elapsedTime_ = 0;
                x_frame_ = (x_frame_ < 3) ? (x_frame_ + 1) : (0);
                if (Action == EntityAction.MOVE_UP)
                    if (Position.Y > NextPosition.Y)
                        position_.Y -= 0.1f;
                if (Action == EntityAction.MOVE_DOWN)
                    if (Position.Y < NextPosition.Y)
                        position_.Y += 0.1f;
                if (Action == EntityAction.MOVE_LEFT)
                    if (Position.X > NextPosition.X)
                        position_.X -= 0.1f;
                if (Action == EntityAction.MOVE_RIGHT)
                    if (Position.X < NextPosition.X)
                        position_.X += 0.1f;
            }
            if (elapsedTimeR_ - gameTime.ElapsedGameTime.Milliseconds > 1000)
            {
                if (Action == EntityAction.REPRODUCE)
                    Action = EntityAction.NONE;
            }
            elapsedTimeR_ += gameTime.ElapsedGameTime.Milliseconds;
            elapsedTime_ += gameTime.ElapsedGameTime.Milliseconds;
        }

        public override void Draw(SpriteBatch graphics, Vector2 camera)
        {

            graphics.Draw(image_, new Rectangle(((int)(Position.X * 32) - (width_ / 4)) + (int)camera.X
                                                , ((int)(Position.Y * 32) - (height_ / 2)) + (int)camera.Y,
                                                width_, height_),
                          frames_[y_frame_][x_frame_], Color.White);
            if (Action == EntityAction.REPRODUCE)
            {
                Texture2D tex = new Texture2D(graphics.GraphicsDevice, width_, height_);
                Color[] data = new Color[width_ * (height_)];
                for (int i = 0; i < data.Length; i++)
                    data[i] = Color.Black;
                tex.SetData(data);
                graphics.Draw(tex, new Rectangle(((int)(Position.X * 32)) + (int)camera.X
                                                , ((int)(Position.Y * 32)) + (int)camera.Y,
                                                width_, height_), Color.White);
            }
        }
    }
}
