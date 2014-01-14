using System;
using System.Collections.Generic;
using System.Text;
using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System.IO;
using System.Windows.Forms;
using System.Diagnostics;
using System.Threading;
using System.ComponentModel;
using System.Collections;
using System.Text.RegularExpressions;
namespace Jmbo
{
    internal enum REPORT_TYPE
    { 
        RAW_DATA,
        CALI_REPORT
    }
    internal class TestDocument
    {
        private JObject data_record;
        private DocTool doctool;
        public const int MAX_PT_1PAGE =5;
        public const int MAX_BC = 4; //maximum 4 beice in one report
        public bool IsDirty //whether there is data added to the test result
        {
            get;
            set;
        }
        public bool IsViewMode{
            get;
            set;
        }
        public TestDocument()
        {

            IsDirty = false;
            IsViewMode = false;
            doctool = new DocTool(this);
        }
        internal bool SaveToDisk(string tofile)
        {
            return JObjTool.ExportToFile(data_record, tofile);
        }
        internal bool LoadHistory(string fromfile)
        {
            try
            {
                data_record = JObject.Parse(File.ReadAllText(fromfile));
                IsViewMode = true;
                IsDirty = false;
                return true;
            }
            catch(Exception ex)
            {
                Logger.Log("Load History failed:" + ex.Message + ex.StackTrace);
            }
            return false;
        }
        internal JProperty Property(string property)
        {
            try
            {
                return data_record.Property(property);
            }
            catch
            {
                return new JProperty(property, ToJValue("0.0"));
            }
        }
        internal void FillInDocument( REPORT_TYPE rule, string tofile)
        {
            bool docopen = false;
            try
            {
                Input askdlg = new Input();
                askdlg.Message = "请输入被测控温仪的样品编号";
                if (askdlg.ShowDialog() != DialogResult.OK)
                    return;
                string ibcid = "";

                string ypbh = askdlg.result;
                List<string> rows = new List<string>();
                //search the corresponding page id and ibc ids
                for (int ibc = 1; ibc <= 4; ibc++)
                {
                    if (!IsValueAvailable(new string[] { "m_" + (ibc + 1).ToString() + "_ypbh" }))
                        continue;
                    if (StrValue("m_" + (ibc + 1).ToString() + "_ypbh") == ypbh)
                    {
                        ibcid = ibc.ToString();
                        break;
                    }
                }
                if (ibcid == "")
                {
                    MessageBox.Show("未找到样品编号为" + ypbh + "的控温仪，请检查后再试");
                    return;
                }

                for (int iwd = 1; iwd <= 5; iwd++)
                {
                    string wdkey = "m_" + ibcid + "_" + iwd.ToString() + "wdd";
                    string bckey = "c_" + ibcid + "_" + iwd.ToString() + "bc";
                    if (!IsValueAvailable(new string[] { wdkey, bckey }))
                        continue;
                    string wdd = StrValue(wdkey).Trim();

                    if (wdd == "")
                        continue;


                    //search for qiehua
                    string qid = "";
                    string qhprefix = "";
                    string cqhprefix = "";
                    for (int iqhwd = 1; iqhwd <= 3; iqhwd++)
                    {
                        qhprefix = "m_" + ibcid + "_" + iqhwd.ToString();
                        cqhprefix = "c_" + ibcid + "_" + iqhwd.ToString();
                        string key = qhprefix + "qhwd";
                        if (IsValueAvailable(new string[] { key }) && StrValue(key) == wdd)
                        {
                            qid = iqhwd.ToString();
                            break;
                        }
                    }
                    if (qid == "")
                        rows.Add(String.Format("{0}\t{1}\t/\t/\t/\t/", FormatValue(data_record.Property(wdkey), false),
                            FormatValue(data_record.Property(bckey), false)));
                    else
                    {
                        
                        rows.Add(String.Format("{0}\t{1}\t{2}\t{3}\t{4}\t{5}",
                            FormatValue(data_record.Property(wdkey), false),
                            FormatValue(data_record.Property(bckey), false),
                            FormatValue(data_record.Property(cqhprefix + "sqh"), false),
                            FormatValue(data_record.Property(cqhprefix + "xqh"), false),
                            FormatValue(data_record.Property(cqhprefix + "qhc"), false),
                            FormatValue(data_record.Property(cqhprefix + "qhwc"), false)));
                    }
                }
                string tmpl = "";
                if (rule == REPORT_TYPE.CALI_REPORT)
                    tmpl = "检测报告";

                string src = Path.Combine(Util.basedir, "报告模板\\" + tmpl + ".doc");
                File.Copy(src, tofile, true);
                doctool.PrepareWord(tofile);
                docopen = true;

                doctool.FillInField(tofile, data_record.Properties(), ibcid);
                doctool.FillInTableByBookMarks("ITEMS", rows);

                doctool.FillInHeader(tofile, data_record.Properties(), ibcid, Microsoft.Office.Interop.Word.WdSeekView.wdSeekCurrentPageHeader);
                doctool.FillInHeader(tofile, data_record.Properties(), ibcid, Microsoft.Office.Interop.Word.WdSeekView.wdSeekPrimaryHeader);

                doctool.SaveWord(tofile);
                MessageBox.Show("导出" + tmpl + "成功");
                docopen = false;
                return;
            }
            catch (Exception ex)
            {

                MessageBox.Show("导出报告失败: " + ex.Message);
                if (docopen)
                    doctool.SaveWord(tofile);
                docopen = false;
            }
        }
        //clear all caculated data
        internal void ClearData()
        {
            List<string> jps = new List<string>();
            foreach (JProperty jp in data_record.Properties())
            {
                if (jp.Name.StartsWith("m_"))
                    return;
                jps.Add(jp.Name);
            }
            foreach (string js in jps)
                data_record.Remove(js);
        }
        
        internal void SetStartInfo()
        {
            test_start = DateTime.Now;

            this["jdrq_n"] = DateTime.Now.Year.ToString();
            this["jdrq_y"] = DateTime.Now.Month.ToString("D2");
            this["jdrq_r"] = DateTime.Now.Day.ToString("D2");

            DateTime yxq = test_start.AddYears(1).AddDays(-1);
            this["yxqz_n"] = yxq.Year.ToString();
            this["yxqz_y"] = yxq.Month.ToString("D2");
            this["yxqz_r"] = yxq.Day.ToString("D2");

        }
        internal DateTime test_start;
        internal void CreateNewRecord()
        {
            data_record = new JObject();
            data_record["create_time"] = new JValue(DateTime.Now);
        }
        internal object this[string key]
        {
            get
            {
                try{
                    if (data_record is JObject)
                        return data_record.Property(key).Value;
                }catch{
                }
                return "";
            }
            set
            {
                if (IsViewMode)
                    return;
                if (data_record is Object)
                {
                    data_record.Remove(key);
                    data_record[key] = ToJValue(value);
                    if(key == "create_time")
                        IsDirty = true;
                }
            }
        }
        private string pageid = "123456";
        public bool CopyToOtherPages(string key, object value)
        {
            bool ret = false;
            Match m = Regex.Match(key, "m_(\\d)_");
            if (!m.Success)
                return false;
            if (key.EndsWith("_fdz"))
                return false;
            char currid = m.Groups[1].Value.ToString()[0];
            for (int i = 0; i < pageid.Length; i++)
            {
                if (pageid[i] > currid)
                {
                    data_record["m_" + pageid[i] + "_"+key.Remove(0,4)] = ToJValue(value);
                    ret = true;
                }
            }
            if (key.EndsWith("wdd"))
            {
                data_record[key.Replace("wdd", "bcsz")] = ToJValue(value);
                ret = true;
            }
            return ret;
        }
        private JObject ToJValue(object value)
        {
            JObject subjt = new JObject();
            if (value is object[])
                subjt["value"] = new JArray(value);
            else if (value is string)
                subjt["value"] = new JValue((string)value);
            else if (value is double)
                subjt["value"] = new JValue((double)value);
            else if (value is TimeSpan)
                subjt["value"] = new JValue((TimeSpan)value);
            else if (value is long)
                subjt["value"] = new JValue((long)value);
            else if ((value is int) || (value is short))
                subjt["value"] = new JValue(Convert.ToInt64((int)value));
            else if (value is ulong)
                subjt["value"] = new JValue((ulong)value);
            else if ((value is uint) || (value is ushort))
                subjt["value"] = new JValue(Convert.ToUInt64((int)value));
            else if (value is bool)
                subjt["value"] = new JValue((bool)value);
            else if (value is Uri)
                subjt["value"] = new JValue((Uri)value);
            else if (value is DateTime)
                subjt["value"] = new JValue((DateTime)value);
            else
                subjt["value"] = new JValue(value.ToString());

            subjt["_logtime"] = new JValue(DateTime.Now);
            return subjt;
        }
        internal static Double ToDouble(Decimal d)
        {
            return Convert.ToDouble(d);
        }
        internal static Decimal ToDecimal(Double d)
        {
            return Convert.ToDecimal(d);
        }

        internal void AutoCaculation()
        {
            #region average caculation

            //c_1_1sjwd, c_1_1bc1,c_1_2bc1,c_1_3bc1 ... c_1_3bc4
            string[] avg_key = new string[] { "1_{0}bc", "2_{0}bc", "3_{0}bc", "4_{0}bc", "1_{0}wd", "2_{0}wd", "3_{0}wd", "4_{0}wd",
            "1_{0}sqh", "2_{0}sqh", "3_{0}sqh", "4_{0}xqh","1_{0}xqh", "2_{0}xqh", "3_{0}xqh", "4_{0}xqh"};
            for (int i = 0; i < avg_key.Length; i++)
            {
                for (int ipoint = 1; ipoint <= MAX_PT_1PAGE; ipoint++)
                {
                    Decimal sum = 0;
                    Decimal count = 0;
                    int rounds = 4;
                    if(avg_key[i].IndexOf("qh") > 0)
                        rounds = 2;
                    for (int j = 1; j < rounds; j++)
                    {
                        string key = "c_" + avg_key[i] + "_" + j.ToString();
                        key = key.Replace("{0}", ipoint.ToString());
                        if (IsValueAvailable(new string[] { key }))
                        {
                            sum = sum + Value(key);
                            count = count + 1;
                        }
                        else
                        {
                            break;
                        }
                    }
                    if (count > 0)
                    {
                        sum = sum / count;
                        string avgid = ("c_" + avg_key[i]).Replace("{0}", ipoint.ToString());
                        this[avgid] = sum;
                    }
                }
            }
            #endregion
            //caculate all temperature related column
            string[] temp_key = new string[] { "" };
            string mkey;
            ABCParam param = new ABCParam();

            for (int ibc = 1; ibc < MAX_BC; ibc++)
            {
                for (int ipoint = 1; ipoint <= MAX_PT_1PAGE; ipoint++)
                {
                    string prefix = "c_" + ibc.ToString()+"_"+ipoint.ToString();
                    mkey = "m_"+ibc.ToString() +"_" + ipoint.ToString() + "wdd";
                    if (IsValueAvailable(new string[] { mkey, prefix + "wd" })) //has setpoint information
                    {
                        this[prefix + "sjwd"] = Value(prefix + "wd");
                    }

                    if (IsValueAvailable(new string[] { mkey, prefix + "bc" , prefix + "sjwd"})) //no setpoint information
                    {
                        this[prefix + "bcwc"] = Value(prefix + "sjwd") - Value(prefix + "bc"); //误差值
                    }

                    mkey = "m_" + ibc.ToString() +"_" +ipoint.ToString() + "qhwd"; //上切换
                    if (IsValueAvailable(new string[] { mkey, prefix + "sqh", prefix + "xqh" })) //no setpoint information
                    {
                        this[prefix + "qhc"] = Value(prefix + "sqh") - Value(prefix + "xqh"); //误差值
                        this[prefix + "qhwc"] = (Value(prefix + "sqh") + Value( prefix + "xqh"))/2 - Value(mkey);
                    }
                }
            }
        }
  
        public string StrValue(string item)
        {
            string abc = data_record.Property(item).Value["value"].ToString();
            return abc;
        }
        private Decimal Value(string item)
        {
            try
            {
                return data_record.Property(item).Value["value"].ToObject<Decimal>();
            }
            catch
            {
                return Decimal.Parse(data_record.Property(item).Value["value"].ToString());
            }
        }
        internal bool IsValueAvailable(string[] items)
        {
            try
            {
                foreach (string item in items)
                {
                    if (item.StartsWith("c_"))
                        Value(item);
                    else
                    {
                        if (StrValue(item) == "")
                            return false;
                    }
                }
                return true;
            }
            catch
            {
                
            }
            return false;
        }
        internal string FormatValue(JProperty jp,bool Display)
        {
            if (!(jp is JProperty))
                return "";
            if (jp.Name.StartsWith("m_"))
            {   //manual input case
                return jp.Value["value"].ToString();
            }
            Decimal fdz = 0.1M;
            Match m = Regex.Match(jp.Name,"(c)_(\\d)_");
            if(m.Success)
                fdz = Decimal.Parse(StrValue("m_" + m.Groups[2].Value + "_fdz").ToString().Trim()) / 10;
            if (Regex.IsMatch(jp.Name, ("\\d(bc)_\\d")) ||
                Regex.IsMatch(jp.Name, ("\\d(bc|bcwc)$")) ||
                jp.Name.EndsWith("qhwc"))
            { //align with the fdz
                return Util.FengDuData(Decimal.Parse(jp.Value["value"].ToString()), fdz);
            }
            if (Regex.IsMatch(jp.Name, ("\\d(sqh|xqh)_\\d"))||
                jp.Name.EndsWith("sqh")||
                jp.Name.EndsWith("xqh")||
                jp.Name.EndsWith("qhc")||
                jp.Name.EndsWith("qhwc"))
            {
                return Decimal.Parse(jp.Value["value"].ToString()).ToString("F02");
            }

            if (Regex.IsMatch(jp.Name,("\\d(wd)_\\d")) ||
                Regex.IsMatch(jp.Name, ("\\d(sjwd|wd)$")))
            {   
                return Decimal.Parse(jp.Value["value"].ToString()).ToString("F03");
            }
            if (Regex.IsMatch(jp.Name, "alphabc"))
            {   //resistance case or sjwd
                return Decimal.Parse(jp.Value["value"].ToString()).ToString("F04");
            }
            return "";

        }
        internal void FillInPage(Form1 frm)
        {
            foreach (JProperty jp in data_record.Properties())
            {
                if (jp.Type != JTokenType.Array)
                {
                    frm.setvalue(jp.Name,FormatValue(jp,true));
                }
            }
        }

    }
}
