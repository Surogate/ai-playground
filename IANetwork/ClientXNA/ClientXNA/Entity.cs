using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace ClientXNA
{
    public abstract class Entity
    {
        public enum EntityAction {
			MOVE_DOWN,
			MOVE_LEFT,
			MOVE_RIGHT,
            MOVE_UP,
			EAT,
			REPRODUCE,
            NONE
		};

        #region Attributes
        protected Vector2 position_;
        protected EntityAction action_;
        #endregion

        public Entity(Vector2 position)
        {
            position_ = position;
            action_ = EntityAction.NONE;
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

        public EntityAction Action
        {

            get
            {
                return (action_);
            }
            set
            {
                action_ = value;
            }
        }

        public abstract void Update(GameTime gameTime);

        public abstract void Draw(SpriteBatch graphics, Vector2 camera);

    }
}
