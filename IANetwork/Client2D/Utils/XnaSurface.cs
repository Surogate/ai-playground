namespace Client2D.Utils
{
    #region USING Directives
    using System;
    using System.IO;
    using System.Reflection;
    using System.Windows;
    using System.Windows.Media;
    using System.Windows.Media.Imaging;
    using System.Windows.Threading;
    using Microsoft.Xna.Framework;
    using Microsoft.Xna.Framework.Graphics;
    using XnaColor = Microsoft.Xna.Framework.Color;
    #endregion

    /// <summary>
    /// Represents an XNA surface.
    /// </summary>
    public class XnaSurface
        : Game
    {
        #region Events
        /// <summary>
        /// Event raised when the initialization of the XNA surface is completed.
        /// </summary>
        public event EventHandler InitializationCompleted;

        /// <summary>
        /// Event raised when the XNA surface has been written in a bitmap surface.
        /// </summary>
        public event EventHandler<WritingXnaSurfaceEventArgs> WritingXnaSurfaceCompleted;
        #endregion

        #region Fields
        /// <summary>
        /// Render target for the grid.
        /// </summary>
        private RenderTarget2D baseRenderTarget;

        /// <summary>
        /// Manager of the graphics device.
        /// </summary>
        protected GraphicsDeviceManager graphics;

        /// <summary>
        /// Represents the color to use to clear the backbuffer.
        /// </summary>
        protected XnaColor xnaClearColor;

        /// <summary>
        /// Sprite batch to use to draw sprites.
        /// </summary>
        protected SpriteBatch spriteBatch;

        /// <summary>
        /// Flag to know if the surface need to be redraw.
        /// </summary>
        protected bool needRefresh;

        /// <summary>
        /// Clock to know when update and draw methods are called.
        /// </summary>
        private DispatcherTimer clock;

        /// <summary>
        /// Snapshot of the timing game.
        /// </summary>
        private GameTime gameTime;

        /// <summary>
        /// Width of the surface.
        /// </summary>
        protected int width;

        /// <summary>
        /// Height of the surface.
        /// </summary>
        protected int height;

        /// <summary>
        /// Elapsed time since the last call of update.
        /// </summary>
        private TimeSpan elapsedTime;

        /// <summary>
        /// Total time elapsed since the start of the game.
        /// </summary>
        private TimeSpan totalElapsedTime;

        /// <summary>
        /// Represents 16.66667 miliseconds.
        /// </summary>
        private const double timeStep = (1 / 60);

        /// <summary>
        /// Frequency of the ticks.
        /// </summary>
        private readonly TimeSpan tickFrequency;

        /// <summary>
        /// Represents the colors array of the render target.
        /// </summary>
        protected XnaColor[] xnaColors;

        /// <summary>
        /// Represents the XNA surface in a WPF bitmap.
        /// </summary>
        private WriteableBitmap xnaSurfaceBitmap;
        #endregion

        #region Properties
        /// <summary>
        /// Gets if the surface is initialized.
        /// </summary>
        public bool IsInitializationCompleted
        {
            get;
            private set;
        }

        /// <summary>
        /// Gets or sets the color which clear the surface.
        /// </summary>
        public XnaColor Clear
        {
            get { return xnaClearColor; }
            set
            {
                if (xnaClearColor != value)
                {
                    xnaClearColor = value;
                    needRefresh = true;
                }
            }
        }

        /// <summary>
        /// Gets or sets if the surface is active.
        /// </summary>
        new public bool IsActive
        {
            get { return clock.IsEnabled; }
            set { clock.IsEnabled = value; }
        }
        #endregion

        #region Constructor
        /// <summary>
        /// Instantiates the XNA surface.
        /// </summary>
        public XnaSurface()
        {
            Content.RootDirectory = Path.Combine(Path.GetDirectoryName(Assembly.GetEntryAssembly().Location),
                                     "Content\\");

            graphics = new GraphicsDeviceManager(this);
            clock = new DispatcherTimer();

            totalElapsedTime = TimeSpan.Zero;
            elapsedTime = TimeSpan.Zero;
            tickFrequency = TimeSpan.FromSeconds(timeStep);
            clock.Interval = tickFrequency;

            IsInitializationCompleted = false;
            needRefresh = false;
            xnaClearColor = XnaColor.CornflowerBlue;

            clock.Tick += ClockOnTick;
        }
        #endregion

        #region Methods
        /// <summary>
        /// Occurs when the clock has ticked.
        /// </summary>
        /// <param name="sender">Object which raise this method.</param>
        /// <param name="e">Event associated to this method.</param>
        private void ClockOnTick(object sender, EventArgs e)
        {
            if (!IsInitializationCompleted)
                return;

            if (!IsActive)
                return;

            try
            {
                totalElapsedTime += tickFrequency;
                elapsedTime = tickFrequency;
            }
            catch (OverflowException)
            {
                totalElapsedTime = TimeSpan.Zero;
                elapsedTime = TimeSpan.Zero;
            }

            gameTime = new GameTime(totalElapsedTime, elapsedTime);

            Update(gameTime);
            Draw(gameTime);
        }

        /// <summary>
        /// Initialize the surface.
        /// </summary>
        public void InitializeSurface()
        {
            Initialize();
        }

        /// <summary>
        /// Initialize the surface.
        /// </summary>
        sealed override protected void Initialize()
        {
            InitializeGraphics(1280, 720);
            InitializeOverride();
            base.Initialize();
        }

        /// <summary>
        /// Initialize the graphics device.
        /// </summary>
        /// <param name="w">Width of the rendering surface.</param>
        /// <param name="h">Height of the rendering surface.</param>
        private void InitializeGraphics(int w, int h)
        {
            if (GraphicsAdapter.DefaultAdapter.IsProfileSupported(GraphicsProfile.HiDef))
                graphics.GraphicsProfile = GraphicsProfile.HiDef;
            else
                graphics.GraphicsProfile = GraphicsProfile.Reach;

            graphics.PreferredBackBufferWidth = w;
            graphics.PreferredBackBufferHeight = h;
            graphics.ApplyChanges();
            needRefresh = true;
        }

        /// <summary>
        /// When overriden in the derived class, allow extra initialization.
        /// </summary>
        virtual protected void InitializeOverride()
        { }

        /// <summary>
        /// Load the content for managing the surface.
        /// </summary>
        sealed override protected void LoadContent()
        {
            spriteBatch = new SpriteBatch(GraphicsDevice);
            LoadContentOverride();
            base.LoadContent();
            OnRaiseInitializationCompleted();
        }

        /// <summary>
        /// When overriden in a derived class, allow extra content loading.
        /// </summary>
        virtual protected void LoadContentOverride()
        { }

        /// <summary>
        /// Occurs when the initialization of the surface is completed.
        /// </summary>
        private void OnRaiseInitializationCompleted()
        {
            if (InitializationCompleted != null)
            {
                IsInitializationCompleted = true;
                InitializationCompleted(this, EventArgs.Empty);
            }
        }

        /// <summary>
        /// Unload the content.
        /// </summary>
        sealed override protected void UnloadContent()
        {
            UnloadContentOverride();
        }

        /// <summary>
        /// When overriden in a derived class, allow extra unloading content.
        /// </summary>
        virtual protected void UnloadContentOverride()
        { }

        /// <summary>
        /// Update the surface.
        /// </summary>
        /// <param name="gameTime">Provides snapshots of timing value.</param>
        sealed override protected void Update(GameTime gameTime)
        {
            UpdateOverride(gameTime);
            base.Update(gameTime);
        }

        /// <summary>
        /// When overriden in a derived class, update the overriden surface.
        /// </summary>
        /// <param name="gameTime">Provides snapshots of timing value.</param>
        virtual protected void UpdateOverride(GameTime gameTime)
        { }

        /// <summary>
        /// Draw the surface.
        /// </summary>
        /// <param name="gameTime">Provides snapshot of timing values.</param>
        sealed override protected void Draw(GameTime gameTime)
        {
            if (!needRefresh)
                return;

            if (baseRenderTarget == null)
                return;

            GraphicsDevice.SetRenderTarget(baseRenderTarget);
            GraphicsDevice.Clear(xnaClearColor);
            DrawOverride(gameTime);
            GraphicsDevice.SetRenderTarget(null);

            Commit(baseRenderTarget,
                   xnaColors,
                   out xnaSurfaceBitmap);

            base.Draw(gameTime);
            OnDrawCompleted();
            //needRefresh = true;
        }

        /// <summary>
        /// When overriden in a derived class, perform extra drawing.
        /// </summary>
        /// <param name="gameTime">Provides snapshot of timing values.</param>
        virtual protected void DrawOverride(GameTime gameTime)
        { }

        /// <summary>
        /// Converts the render target into a writeable bitmap.
        /// </summary>
        /// <param name="rt">Rendertarget to convert as bitmap.</param>
        /// <param name="colors">Array of colors which will contains 
        /// the data of the rendertarget.</param>
        /// <param name="bmp">Bitmap which represents the datas present 
        /// on the rendertarget.</param>
        protected void Commit(RenderTarget2D rt,
                              XnaColor[] colors,
                              out WriteableBitmap bmp)
        {
            bmp = null;
            bmp = new WriteableBitmap(rt.Width, rt.Height,
                                         96, 96, PixelFormats.Bgra32, null);

            rt.GetData<XnaColor>(colors);

            bmp.Lock();
            unsafe
            {
                int backbuffer = (int)bmp.BackBuffer;
                foreach (XnaColor c in colors)
                {
                    *((int*)backbuffer) = c.B |
                                          c.G << 8 |
                                          c.R << 16 |
                                          c.A << 24;

                    backbuffer += sizeof(IntPtr);
                }
            }
            bmp.AddDirtyRect(new Int32Rect(0, 0, bmp.PixelWidth, bmp.PixelHeight));
            bmp.Unlock();
        }

        /// <summary>
        /// Occurs when the XNA surface has been converted into a bitmap.
        /// </summary>
        private void OnDrawCompleted()
        {
            if (WritingXnaSurfaceCompleted != null)
                WritingXnaSurfaceCompleted(this,
                    new WritingXnaSurfaceEventArgs(xnaSurfaceBitmap));
        }

        /// <summary>
        /// Dispose the xna colors array.
        /// </summary>
        private void DisposeXnaColors()
        {
            if (xnaColors != null)
            {
                xnaColors = null;
                GC.Collect();
            }
        }

        /// <summary>
        /// Creates a render target.
        /// </summary>
        /// <param name="width">Width of the render target. (Clamped to 1980 px max)</param>
        /// <param name="height">Height of the render target. (Clamped to 1024 px max)</param>
        public void CreateRenderTarget(int width, int height)
        {
            DisposeXnaColors();
            DisposeRenderTarget();

            this.width = Math.Max(width, 1);
            this.height = Math.Max(height, 1);

            this.width = Math.Min(1980, this.width);
            this.height = Math.Min(1024, this.height);

            baseRenderTarget = new RenderTarget2D(GraphicsDevice,
                                                this.width,
                                                this.height);

            xnaColors = new XnaColor[this.width * this.height];
            InitializeGraphics(this.width, this.height);
        }

        /// <summary>
        /// Dispose the render target.
        /// </summary>
        private void DisposeRenderTarget()
        {
            if (baseRenderTarget != null)
                baseRenderTarget.Dispose();

            baseRenderTarget = null;
            GC.Collect();
        }

        /// <summary>
        /// Refresh the surface.
        /// </summary>
        public void Refresh()
        {
            needRefresh = true;
        }
        #endregion
    }
}
