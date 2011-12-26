using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections.Concurrent;

namespace ClientXNA.ViewModels
{
    public class ChartsViewModel : BasicViewModel
    {
        public class ChartModel
        {
            public float Value { get; set; }
            public int Time { get; set; }
        }

        #region Attributes
        #endregion

        #region Properties
        public ConcurrentBag<ChartModel> WolfPopulation
        {
            get;
            set;
        }

        public ConcurrentBag<ChartModel> WolfPerformances
        {
            get;
            set;
        }

        public ConcurrentBag<ChartModel> SheepPopulation
        {
            get;
            set;
        }

        public ConcurrentBag<ChartModel> SheepPerformances
        {
            get;
            set;
        }
        #endregion

        #region Constructors
        public ChartsViewModel()
        {
            WolfPopulation = new ConcurrentBag<ChartModel>();
            WolfPerformances = new ConcurrentBag<ChartModel>();
            SheepPopulation = new ConcurrentBag<ChartModel>();
            SheepPerformances = new ConcurrentBag<ChartModel>();
        }
        #endregion

        #region Methods
        public void RefreshProperties()
        {
            OnPropertyChanged("WolfPopulation");
            OnPropertyChanged("WolfPerformances");
            OnPropertyChanged("SheepPopulation");
            OnPropertyChanged("SheepPerformances");
        }
        #endregion
    }
}
