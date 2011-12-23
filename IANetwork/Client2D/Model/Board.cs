using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Client2D.Model
{
    public class Board
    {
        #region Attributes
        private int size_;
        private Square[][] data_;
        #endregion

        public Board(int size)
        {
            size_ = size;
            data_ = new Square[size][];
            for (int x = 0; x < size; x++)
            {
                data_[x] = new Square[size];
                for (int y = 0; y < size; y++)
                    data_[x][y] = new Square();
            }
        }

        public int Size
        {
            get { return size_; }
            set
            {
                if (size_ != value)
                {
                    size_ = value;
                    resetBoard(size_);
                }
            }
        }

        public Square[][] Data
        {
            get { return data_; }
            private set {}
        }


        #region Methods
        private void resetBoard(int size)
        {
            data_ = new Square[size][];
            for (int i = 0; i < size; i++)
                data_[i] = new Square[size];
        }
        #endregion

    }
}
