using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;

namespace ClientXNA
{
    public class Square
    {
        enum Title {
			USEABLE = 1,
			GRASS = 2,
			SHEEP = 4,
			WOLF = 8
		};

        #region Attributes
        private bool hasGrass_;
        private int   odour_;
        #endregion

        public Square()
        {
            hasGrass_ = false;
            odour_ = 0;
        }

        public int Odour
        {
            get { return odour_;  }
            set { odour_ = value; }
        }

        public void setHasGrass(bool hasGrass)
        {
            hasGrass_ = hasGrass;
        }

        public bool hasGrass()
        {
            return (hasGrass_);
        }

    }
}
