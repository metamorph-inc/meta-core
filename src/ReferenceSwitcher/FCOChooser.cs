using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Xml;
using GME.CSharp;
using GME.MGA;

namespace ReferenceSwitcher
{
    partial class FCOChooser : Form
    {
        MgaGateway MgaGateway;
        GMEConsole GMEConsole;
        IMgaObject fco1Object;
        IMgaObject fco2Object;

        public FCOChooser(MgaGateway gateway, GMEConsole console)
        {
            this.MgaGateway = gateway;
            this.GMEConsole = console;
            InitializeComponent();
            this.choose1.Click += new EventHandler(choose_Click);
            this.choose2.Click += new EventHandler(choose_Click);
            this.link.Click += new EventHandler(link_Click);
        }

        void link_Click(object sender, EventArgs e)
        {
            if (fco1Object == null || fco2Object == null)
            {
                MessageBox.Show("Please choose both an old object and a new object");
                return;
            }
            Close();
            MgaGateway.PerformInTransaction(delegate
            {
                Switcher switcher = new Switcher(fco1Object, fco2Object, GMEConsole);
                switcher.UpdateSublibrary();
            });
        }

        public IMgaObjects GetSelectedObjects()
        {
            IEnumerable<GME.IGMEOLEPanel> panels = GMEConsole.gme.Panels.OfType<GME.IGMEOLEPanel>();
            GME.IGMEOLEPanel treeBrowser = panels.First(panel => panel.Name == "Browser");
            IMgaObjects selected = treeBrowser.Interface.GetType().InvokeMember("GetSelectedMgaObjects", System.Reflection.BindingFlags.InvokeMethod,
                null, treeBrowser.Interface, new object[] { }) as IMgaObjects;
            return selected;
        }


        void choose_Click(object sender, EventArgs e)
        {
            IMgaObject selected = GetSelectedObjects().OfType<IMgaObject>().First();
            string selectedText = null;
            MgaGateway.PerformInTransaction(delegate
            {
                selectedText = selected.Name + " (" + selected.ID + ")";
            });
            if (sender == choose1)
            {
                this.fco1.Text = selectedText;
                this.fco1Object = selected;
            }
            else if (sender == choose2)
            {
                this.fco2.Text = selectedText;
                this.fco2Object = selected;
            }
        }
    }

    static class DictionaryExtension
    {
        public static V GetValueOrDefault<K, V>(this Dictionary<K, V> dic, K key)
            where V : new()
        {
            V ret;
            bool found = dic.TryGetValue(key, out ret);
            if (found)
            {
                return ret;
            }
            else
            {
                ret = new V();
                dic[key] = ret;
                return ret;
            }
        }
    }

    public class KeyEqualityComparer<T> : IEqualityComparer<T>
    {
        private readonly Func<T, object> keyExtractor;

        public KeyEqualityComparer(Func<T, object> keyExtractor)
        {
            this.keyExtractor = keyExtractor;
        }

        public bool Equals(T x, T y)
        {
            return this.keyExtractor(x).Equals(this.keyExtractor(y));
        }

        public int GetHashCode(T obj)
        {
            return this.keyExtractor(obj).GetHashCode();
        }
    }
}
