using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Xunit;

namespace MasterInterpreterTest
{
    public class JobManagerLibTest
    {
        [Fact]
        public void TestMarshaling()
        {
            int marshelByRefCount = 0;
            foreach (var class_ in typeof(JobManagerFramework.JobImpl).Assembly.DefinedTypes)
            {
                if (class_.Name == "CookieAwareWebClient")
                {
                    // this class isn't used by Remoting
                    continue;
                }
                if (class_.IsAbstract)
                {
                    continue;
                }
                if (class_.IsSubclassOf(typeof(MarshalByRefObject)))
                {
                    System.Reflection.MethodInfo InitializeLifetimeService = null;
                    foreach (var method in class_.DeclaredMethods)
                    {
                        // public override object InitializeLifetimeService()
                        if (method.IsVirtual && method.Name == "InitializeLifetimeService" && method.GetParameters().Length == 0)
                        {
                            InitializeLifetimeService = method;
                            marshelByRefCount++;
                            break;
                        }
                    }
                    if (InitializeLifetimeService == null)
                    {
                        Assert.False(true, String.Format("Type {0} does not define method InitializeLifetimeService. It must, or .NET Remoting calls will fail after 5 minutes", class_.Name));
                    }
                }
            }
            Assert.True(marshelByRefCount >= 4, "Didn't find enough MarshalByRef classes. (Decrease test if some were removed)");
        }
    }
}
