using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace ClientXNA
{
    public class Square
    {
        public enum Title
        {
            NONE,
            BLOCK,
            GRASS,
            SHEEP,
            WOLF,
            GRASS_SHEEP,
            GRASS_WOLF,
            GRASS_SHEEP_WOLF,
            SHEEP_WOLF
        };

        #region Attributes
        private Title title_;
        private int   odour_;
        #endregion

        public Square()
        {
            title_ = Title.NONE;
            odour_ = 0;
        }

        public Title OnSquare
        {
            get { return title_; }
            set { title_ = value; }
        }

        public int Odour
        {
            get { return odour_;  }
            set { odour_ = value; }
        }

        public bool hasGrass()
        {
            return (((int)OnSquare == (int)Title.GRASS ||
                     (int)OnSquare == (int)Title.GRASS_SHEEP ||
                     (int)OnSquare == (int)Title.GRASS_WOLF ||
                     (int)OnSquare == (int)Title.GRASS_SHEEP_WOLF));
        }
    }
}
