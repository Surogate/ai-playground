using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using Client2D.View;
using Client2D.Utils;
using Client2D.ViewModel;

namespace Client2D
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        //private XNAView surface;

        public MainWindow()
        {
            InitializeComponent();
            //surface = new XNAView();
            //surface.WritingXnaSurfaceCompleted += surface_WritingXnaSurfaceCompleted;
            //surface.InitializationCompleted += new EventHandler(surface_InitializationCompleted);
            //surface.InitializeSurface();
            //surface.IsActive = true;
            ((Client)DataContext).ConnectTo = new Client.ConnectDelegate(xnaView.Game.ConnectTo);
            ((Client)DataContext).ViewXNA = xnaView;
            xnaView.Game.ClientVM = (Client)DataContext;
        }

        //void surface_InitializationCompleted(object sender, EventArgs e)
        //{
        //    surface.CreateRenderTarget((int)ActualWidth, (int)ActualHeight);
        //}

        //void surface_WritingXnaSurfaceCompleted(object sender, WritingXnaSurfaceEventArgs e)
        //{
        //    xnaView.Source = e.RenderBitmap;
        //}


        //private void xnaView_SizeChanged(object sender, SizeChangedEventArgs e)
        //{
        //    if (!surface.IsInitializationCompleted)
        //        return;

        //    surface.CreateRenderTarget((int)e.NewSize.Width, (int)e.NewSize.Height);
        //}
    }
}
