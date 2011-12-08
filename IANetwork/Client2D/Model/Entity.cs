using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Microsoft.Xna.Framework;
using Microsoft.Xna.Framework.Graphics;

namespace Client2D.Model
{
    public abstract class Entity
    {
        public enum EntityAction {
            MOVE_UP,
            MOVE_DOWN,
            MOVE_LEFT,
            MOVE_RIGHT,
			EAT,
			REPRODUCE,
            NONE
		};

        #region Attributes
        protected Vector2 position_;
        protected Vector2 nexPosition_;
        protected EntityAction action_;
        #endregion

        public Entity(Vector2 position)
        {
            position_ = position;
            nexPosition_ = position;
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

        public Vector2 NextPosition
        {
            get
            {
                return (nexPosition_);
            }
            set
            {
                nexPosition_ = value;
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
