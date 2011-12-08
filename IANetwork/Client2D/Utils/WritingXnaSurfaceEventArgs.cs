namespace Client2D.Utils
{
    #region USING Directives
    using System;
    using System.Windows.Media.Imaging;
    #endregion

    /// <summary>
    /// Event args for the Xna surface.
    /// </summary>
    sealed public class WritingXnaSurfaceEventArgs
        : EventArgs
    {
        #region Property
        /// <summary>
        /// Gets the generated bitmap.
        /// </summary>
        public BitmapSource RenderBitmap
        {
            get;
            private set;
        }
        #endregion

        #region Constructor
        /// <summary>
        /// Instantiates the event args.
        /// </summary>
        /// <param name="source">Generated bitmap.</param>
        public WritingXnaSurfaceEventArgs(BitmapSource source)
        {
            this.RenderBitmap = source;
        }
        #endregion
    }
}
