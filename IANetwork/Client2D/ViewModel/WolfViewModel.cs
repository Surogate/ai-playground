using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Client2D.Utils;
using System.Diagnostics;

namespace Client2D.ViewModel
{
    public class WolfViewModel : BasicViewModel
    {
        private IEnumerable<Model.Population> populations_;
        private IEnumerable<Model.Performance> perfs_;

        private string title_ = "Wolf";

        public string Title
        {
            get { return title_; }
            set { title_ = value; OnPropertyChanged("Title"); }
        }

        public IEnumerable<Model.Population> Population
        {
            get
            {
                return (populations_);
            }

            set
            {
                populations_ = value;
                OnPropertyChanged("Population");
            }
        }

        public IEnumerable<Model.Performance> Performance
        {
            get
            {
                return perfs_;
            }

            set
            {
                perfs_ = value;
                OnPropertyChanged("Performance");
            }
        }

        public void Update()
        {
            OnPropertyChanged("Population");
            OnPropertyChanged("Performance");
        }
    }
}
