namespace Client2D.Utils
{
    #region USING Directives
    using System.Xml;
    using Microsoft.Xna.Framework.Content.Pipeline.Serialization.Intermediate;
    #endregion

    /// <summary>
    /// Class to serialize an object to be used in the XNA content pipeline.
    /// </summary>
    static public class XnaSerializer
    {
        #region Fields
        /// <summary>
        /// Settings for the xml writter.
        /// </summary>
        static private XmlWriterSettings settings;
        #endregion

        #region Constructor
        /// <summary>
        /// Instantiates the class.
        /// </summary>
        static XnaSerializer()
        {
            settings = new XmlWriterSettings();
            settings.Indent = true;
        }
        #endregion

        #region Methods
        /// <summary>
        /// Serialize an object into a file.
        /// </summary>
        /// <param name="fileName">Name of the file to save.</param>
        /// <param name="obj">Object to save.</param>
        static public void Serialize(string fileName,
                                     object obj)
        {
            using (XmlWriter writer = XmlWriter.Create(fileName, settings))
            {
                IntermediateSerializer.Serialize(writer, obj, null);
                writer.Flush();
                writer.Close();
            }
        }
        #endregion
    }
}
