using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace ClientXNA
{
    public class Entity
    {
        #region Attributes
        protected Vector2 position_;
        #endregion

        public Entity(Vector2 position)
        {
            position_ = position;
        }

        public Vector2 Position
        {
            get
            {
                return (position_);
            }

            set
            {
                position_ = value;
            }
        }

        public abstract void Draw(SpriteBatch graphics);

    }
}
