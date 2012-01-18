using System;

namespace ClientXNA
{
#if WINDOWS || XBOX
    static class Program
    {
        /// <summary>
        /// The main entry point for the application.
        /// </summary>
        [STAThread]
        static void Main(string[] args)
        {
            using (GameClient game = new GameClient())
            {
                game.Run();
            }
        }
    }
#endif
}

