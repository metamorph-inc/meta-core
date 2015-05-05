// -----------------------------------------------------------------------
// <copyright file="InterpreterException.cs" company="">
// TODO: Update copyright text.
// </copyright>
// -----------------------------------------------------------------------

namespace META
{
    using System;
    using System.Runtime.Serialization;

    // following http://stackoverflow.com/questions/9361491/create-new-exception-type

    [Serializable]
    public class InterpreterException : Exception
    {

        //readonly ComComponent data;

        public InterpreterException() { }

        //public InterpreterException(ComComponent data)
        //    : base(FormatMessage(data))
        //{
        //    this.data = data;
        //}

        public InterpreterException(string message) : base(message) { }

        //public InterpreterException(ComComponent data, Exception inner)
        //    : base(FormatMessage(data), inner)
        //{
        //    this.data = data;
        //}

        public InterpreterException(String message, Exception inner) : base(message, inner) { }

        //protected InterpreterException(SerializationInfo info, StreamingContext context)
        //    : base(info, context)
        //{
        //    if (info == null)
        //    {
        //        throw new ArgumentNullException("info");
        //    }
        //    this.data = info.GetValue("data", typeof(ComComponent)) as ComComponent;
        //}

        //public override void GetObjectData(SerializationInfo info, StreamingContext context)
        //{
        //    if (info == null)
        //    {
        //        throw new ArgumentNullException("info");
        //    }
        //    info.AddValue("data", this.data);
        //    base.GetObjectData(info, context);
        //}

        //public ComComponent Data { get { return this.data; } }

        //private static string FormatMessage(ComComponent data)
        //{
        //    return string.Format("Interpreter exception with data {0}, valid: {1}.", data.ProgId, data.isValid);
        //}

    }
}
