using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace AVM
{
    public static class ExtensionMethods
    {
        /// <summary>
        /// Template method to allow *.AddItem(item) for AVM.Feature and AVM.Association
        /// </summary>
        /// <typeparam name="T"></typeparam>
        /// <param name="source"></param>
        /// <param name="item"></param>
        public static void AddItem<T>(this IEnumerable<T> source, T item)
        {
            if (source == null)
            {
                source = new List<T>();
            }

            if (source is IList<T>)
            {
                (source as IList<T>).Add(item);
            }
            else
            {
                var r = source.ToList();
                r.Add(item);
                source = r;
            }
        }

        public static void RemoveItem<T>(this IEnumerable<T> source, T item)
        {
            if (source == null)
            {
                source = new List<T>();
            }

            if (source is IList<T>)
            {
                (source as IList<T>).Remove(item);
            }
            else
            {
                var r = source.ToList();
                r.RemoveItem(item);
                source = r;
            }
        }
    }
}
