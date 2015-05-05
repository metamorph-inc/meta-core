using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using GME.MGA;

namespace SubTreeMerge
{
    public class ParentChainEnumerator : IEnumerator<IMgaObject>
    {
        #region IEnumerator<IMgaObject> Members

        IMgaObject original;
        public ParentChainEnumerator(IMgaObject obj)
        {
            this.original = obj;
        }
        public IMgaObject Current { get; set; }

        #endregion

        #region IDisposable Members

        public void Dispose()
        {
        }

        #endregion

        #region IEnumerator Members

        object System.Collections.IEnumerator.Current
        {
            get { return Current; }
        }

        public bool MoveNext()
        {
            if (Current == null)
            {
                Current = original;
            }
            else
            {
                GME.MGA.Meta.objtype_enum objType;
                GME.MGA.MgaObject o;
                Current.GetParent(out o, out objType);
                Current = o;
            }
            return Current != null;
        }

        public void Reset()
        {
            Current = null;
        }

        #endregion
    }

    public class ParentChain : IEnumerable<IMgaObject>
    {
        IMgaObject obj;
        public ParentChain(IMgaObject obj)
        {
            this.obj = obj;
        }
        public IEnumerator<IMgaObject> GetEnumerator()
        {
            return new ParentChainEnumerator(obj);
        }
        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return new ParentChainEnumerator(obj);
        }
    }
}
