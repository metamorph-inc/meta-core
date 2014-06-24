using System;

namespace JobManagerFramework
{
    public class InvalidPoolStateException : Exception
    {
        public InvalidPoolStateException()
        {
            
        }

        public InvalidPoolStateException(string reason) : base(reason)
        {

        }
    }
}