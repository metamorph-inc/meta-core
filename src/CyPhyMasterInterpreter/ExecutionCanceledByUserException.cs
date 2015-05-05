using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace CyPhyMasterInterpreter
{
    [Serializable]
    public class ExecutionCanceledByUserException : Exception
    {
        public ExecutionCanceledByUserException() { }
        public ExecutionCanceledByUserException(string message) : base(message) { }
        public ExecutionCanceledByUserException(string message, Exception inner) : base(message, inner) { }
        protected ExecutionCanceledByUserException(
          System.Runtime.Serialization.SerializationInfo info,
          System.Runtime.Serialization.StreamingContext context)
            : base(info, context) { }
    }
}
