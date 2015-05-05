namespace CyPhyMasterInterpreter
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Text;

    /// <summary>
    /// Represents errors that occur during expanding the model when the Test Injection Point's target is not found in the design.
    /// </summary>
    /// <remarks>
    /// For instance
    /// <list type="bullet">
    /// <item>Traceability was lost between the design space and the design.</item>
    /// <item>The referenced object to the design space was not found in the generated design.</item>
    /// </list>
    /// </remarks>
    [Serializable]
    public class AnalysisModelTipNotFoundException : AnalysisModelExpandFailedException
    {
        /// <summary>
        /// Initializes a new instance of the <see cref="AnalysisModelTipNotFoundException"/> class.
        /// </summary>
        public AnalysisModelTipNotFoundException()
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AnalysisModelTipNotFoundException"/> class with a specified
        /// error message.
        /// </summary>
        /// <param name="message">The message that describes the error.</param>
        public AnalysisModelTipNotFoundException(string message)
            : base(message)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AnalysisModelTipNotFoundException"/> class with a specified
        /// error message and a reference to the inner exception that is the cause of
        /// this exception.
        /// </summary>
        /// <param name="message">The error message that explains the reason for the exception.</param>
        /// <param name="inner">The exception that is the cause of the current exception, or a null reference
        /// (Nothing in Visual Basic) if no inner exception is specified.</param>
        public AnalysisModelTipNotFoundException(string message, Exception inner)
            : base(message, inner)
        {
        }

        /// <summary>
        /// Initializes a new instance of the <see cref="AnalysisModelTipNotFoundException"/> class with serialized
        /// data.
        /// </summary>
        /// <param name="info">The System.Runtime.Serialization.SerializationInfo that holds the serialized
        /// object data about the exception being thrown.</param>
        /// <param name="context">The System.Runtime.Serialization.StreamingContext that contains contextual
        /// information about the source or destination.</param>
        protected AnalysisModelTipNotFoundException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context)
            : base(info, context)
        {
        }
    }
}
