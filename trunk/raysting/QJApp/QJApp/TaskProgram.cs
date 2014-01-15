using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Runtime.Serialization.Formatters.Binary;

namespace QJApp
{
    [Serializable]
    internal class RsRecord
    {
        internal static RsRecord CreateFromRsInfo(RsInfo rs)
        {
            return new RsRecord();
        }
    }
    [Serializable]
    internal class RxRecord
    {
        internal static RsRecord CreateFromRxInfo(RsInfo rs)
        {
            return new RsRecord();
        }
    }
    [Serializable]
    internal class PrgItem
    {
        internal string Comments;
    }
    [Serializable]
    internal class PrgMeasureItem:PrgItem
    {
        internal RsRecord Rs;
        internal RxRecord Rx;
        internal bool Exchange;
        internal double Result;
        internal DateTime datestamp; //when the measurement is completed.
        internal string datapath;// path to datafile
    }
    [Serializable]
    internal class PrgWaitItem : PrgItem
    {
        internal int Delay;
    }
    [Serializable]
    internal class PrgOvenItem : PrgItem
    {
        internal double Temperature;
    }
    [Serializable]
    public class OneProgram
    {
        internal List<PrgItem> prgItems;
        public string Name;
    }
    [Serializable]
    public class OneTask
    {
        internal List<OneProgram> programs;
        public string Name;
    }
}
