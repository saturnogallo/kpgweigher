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
using DotNetMatrix;
namespace Jmbo
{
    internal enum REPORT_TYPE
    { 
        RAW_DATA,
        CALI_REPORT,
        INFO_REPORT,
        JIAOZHUN_REPORT
    }
    internal class TestDocument
    {
        private JObject data_record;
        private DocTool doctool;
        public const int MAX_PT_1PAGE =3;
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
        internal string ProbeType;
        
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
        internal void FillInFendu(SaveFileDialog saveFileDialog1)
        {
            bool docopen = false;
            Input askdlg = new Input();
            decimal t_start;
            decimal t_end;

            Dictionary<Decimal, Decimal> points = new Dictionary<Decimal, Decimal>();
            /*points[-55] = 78.2953M;
            points[-40] = 84.2496M;
            points[-20] = 92.1416M;
            points[0] = 99.9841M;
            points[50] = 119.4089M;
            points[100] = 138.5204M;
            points[125] = 147.9672M;
             */
            #region input

            askdlg.Message = "请输入铂电阻的出厂编号";
            if (askdlg.ShowDialog() != DialogResult.OK)
                return;


            string ccbh = askdlg.result;
            string ibcid = "";
            string pageid = "";
            //search the corresponding page id and ibc ids
            for (int ibc = 0; ibc < 12; ibc++)
            {
                pageid = "abcdef"[ibc / 4].ToString();
                if (!IsValueAvailable(new string[] { "m_" + (ibc + 1).ToString() + "_ccbh" }))
                    continue;
                if (StrValue("m_" + (ibc + 1).ToString() + "_ccbh") == ccbh)
                {
                    ibcid = (ibc + 1).ToString();
                    for (int iwd = 1; iwd <= 3; iwd++)
                    {
                        if (!IsValueAvailable(new string[]{ "m_" + pageid + "_"+iwd.ToString()+"jddwd",
                                "c_" + (ibc + 1).ToString() + "_" + iwd.ToString() + "bc"}))
                            continue;
                        string wdd = StrValue("m_" + pageid + "_" + iwd.ToString() + "jddwd").Trim();

                        if (wdd == "")
                            continue;
                        string sjz = FormatValue(data_record.Property("c_" + (ibc + 1).ToString() + "_" + iwd.ToString() + "sjzbc"), false);
                        points[Decimal.Parse(wdd)] = Decimal.Parse(sjz);
                    }
                }
            }
            if (points.Count <= 0)
            {
                MessageBox.Show("未找到出厂编号为" + ccbh + "的铂电阻，请检查后再试");
                return;
            }
            #endregion
            double r0 = 999;
            if (points.ContainsKey(0))
            {
                r0 = Convert.ToDouble(points[0]);
            }
            else
            {
                MessageBox.Show("未发现0℃温度点实际值，无法计算分度表");
                return;
            }
            double[][] mn = new double[3][];
            double[] yn = new double[3];
            decimal ptn1, ptn2, ptn3;
            ptn1 = 1000;
            ptn2 = 0;
            ptn3 = -1000;
            mn[0] = new double[3];
            mn[1] = new double[3];
            mn[2] = new double[3];

            decimal ptp1, ptp2;
            ptp1 = 1000;
            ptp2 = -1000;
            double[][] mp = new double[2][];
            mp[0] = new double[2];
            mp[1] = new double[2];

            double[] yp = new double[2];

            //count points for range
            int np = 0;
            int nn = 0;
            foreach (Decimal d in points.Keys)
            {
                double dv;
                if ((d < 0) && (d >= -200))
                {
                    if (d < ptn1)
                    {
                        ptn2 = ptn1;
                        ptn1 = d;
                    }
                    if (d > ptn3)
                    {
                        ptn2 = ptn3;
                        ptn3 = d;
                    }
                    if (points.ContainsKey(ptn1))
                    {
                        dv = Convert.ToDouble(ptn1);
                        mn[0][0] = dv; mn[0][1] = dv * dv; mn[0][2] = (dv - 100) * dv * dv * dv;
                        yn[0] = Convert.ToDouble(points[ptn1]) / r0 - 1;
                    }
                    if (points.ContainsKey(ptn2))
                    {
                        dv = Convert.ToDouble(ptn2);
                        mn[1][0] = dv; mn[1][1] = dv * dv; mn[1][2] = (dv - 100) * dv * dv * dv;
                        yn[1] = Convert.ToDouble(points[ptn2]) / r0 - 1;
                    }
                    if (points.ContainsKey(ptn3))
                    {
                        dv = Convert.ToDouble(ptn3);
                        mn[2][0] = dv; mn[2][1] = dv * dv; mn[2][2] = (dv - 100) * dv * dv * dv;
                        yn[2] = Convert.ToDouble(points[ptn3]) / r0 - 1;
                    }

                    nn++;
                }
                if ((d > 0) && (d <= 850))
                {
                    if (d < ptp1)
                    {
                        ptp1 = d;
                        dv = Convert.ToDouble(ptp1);
                        mp[0][0] = dv; mp[0][1] = dv * dv;
                        yp[0] = Convert.ToDouble(points[ptp1]) / r0 - 1;
                    }
                    if (d > ptp2)
                    {
                        ptp2 = d;
                        dv = Convert.ToDouble(ptp2);
                        mp[1][0] = dv; mp[1][1] = dv * dv;
                        yp[1] = Convert.ToDouble(points[ptp2]) / r0 - 1;
                    }
                    np++;
                }
            }
            if (ptn1 > 0)
                ptn1 = 0;
            if (ptp2 < 0)
                ptp2 = 0;

            askdlg.Message = "请输入分度表起始温度";
            if (askdlg.ShowDialog() != DialogResult.OK)
                return;
            if (!Decimal.TryParse(askdlg.result, out t_start) || (t_start < ptn1))
            {
                MessageBox.Show("无效的起始温度,分度表不能外插");
                return;
            }

            askdlg.Message = "请输入分度表结束温度";
            if (askdlg.ShowDialog() != DialogResult.OK)
                return;
            if (!Decimal.TryParse(askdlg.result, out t_end) || (t_end > ptp2))
            {
                MessageBox.Show("无效的结束温度,分度表不能外插");
                return;
            }

            Decimal s = t_start;
            Decimal e = t_end;

            if ((s < 0) && (nn < 3))
            {
                MessageBox.Show("检定点不足。-200℃ ~ 0 ℃的分度表计算至少有需要3个检定点.");
                return;
            }
            if ((e > 0) && (np < 2))
            {
                MessageBox.Show("检定点不足。0℃ ~ 850 ℃的分度表计算至少有需要2个检定点.");
                return;
            }

            double[] xn = new double[3]; //A,B,C
            double[] xp = new double[2]; //A,B
            GeneralMatrix gxn;
            GeneralMatrix gxp;
            if (nn >= 3)
            {
                GeneralMatrix gm = new GeneralMatrix(mn);
                gxn = gm.Solve(new GeneralMatrix(yn, yn.Length));
                xn[0] = gxn.GetElement(0, 0);
                xn[1] = gxn.GetElement(1, 0);
                xn[2] = gxn.GetElement(2, 0);
            }
            if (np >= 2)
            {
                GeneralMatrix gm = new GeneralMatrix(mp);
                gxp = gm.Solve(new GeneralMatrix(yp, yp.Length));
                xp[0] = gxp.GetElement(0, 0);
                xp[1] = gxp.GetElement(1, 0);
            }

            List<string> rows = new List<string>();
            while (s <= e)
            {
                double sv = Convert.ToDouble(s);
                if (s < 0)
                {
                    rows.Add(String.Format("{0}\t{1:F4}", s.ToString(), r0 * (1 + xn[0] * sv + xn[1] * sv * sv + xn[2] * (sv - 100) * sv * sv * sv)));
                }
                else
                {
                    rows.Add(String.Format("{0}\t{1:F4}", s.ToString(), r0 * (1 + xp[0] * sv + xp[1] * sv * sv)));
                }
                s = s + 1;
            }
            saveFileDialog1.DefaultExt = ".doc";
            saveFileDialog1.Filter = "DOC File(*.doc)|*.doc|All Files(*.*)|*.*";


            saveFileDialog1.FileName = DateTime.Now.ToString("yyyy-MM-dd") + "分度表证书.doc";
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            string tofile = saveFileDialog1.FileName;

            try
            {
                string tmpl = "分度表";
                string src = Path.Combine(Util.basedir, "报告模板\\" + tmpl + ".doc");
                File.Copy(src, tofile, true);
                doctool.PrepareWord(tofile);
                docopen = true;

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
        internal void FillInRawData(string tofile)
        {
            bool docopen = false;
            string tmpl = "原始记录单";
            try
            {
                string src = Path.Combine(Util.basedir, "报告模板\\"+tmpl+".doc");
                File.Copy(src, tofile, true);
                doctool.PrepareWord(tofile);
                docopen = true;

                doctool.FillInField(tofile, data_record.Properties());

                doctool.FillInHeader(tofile, data_record.Properties(), Microsoft.Office.Interop.Word.WdSeekView.wdSeekCurrentPageHeader);
                doctool.FillInHeader(tofile, data_record.Properties(), Microsoft.Office.Interop.Word.WdSeekView.wdSeekPrimaryHeader);

                doctool.SaveWord(tofile);
                MessageBox.Show("导出" + tmpl + "成功");
                docopen = false;
            }
            catch(Exception ex)
            {
                MessageBox.Show("导出报告失败: " + ex.Message);
                if (docopen)
                    doctool.SaveWord(tofile);
                docopen = false;
            }
        }
        internal void FillInDocument( REPORT_TYPE rule, string tofile)
        {
            bool docopen = false;
            try
            {
                Input askdlg = new Input();
                askdlg.Message = "请输入被测铂电阻的出厂编号";
                if (askdlg.ShowDialog() != DialogResult.OK)
                    return;
                string pageid= "";
                string ccbh = askdlg.result;
                string ibcid = "";
                List<string> rows = new List<string>();
                Hashtable addkeys = new Hashtable();
                //search the corresponding page id and ibc ids
                for (int ibc = 0; ibc < 12; ibc++)
                {
                    if (!IsValueAvailable(new string[] { "m_" + (ibc + 1).ToString() + "_ccbh" }))
                        continue;
                    if (StrValue("m_" + (ibc + 1).ToString() + "_ccbh") == ccbh)
                    {
                        if (pageid == "")
                        {
                            pageid = String.Format("abcdef")[ibc / 4].ToString();
                            ibcid = (ibc + 1).ToString();
                        }
                        for (int iwd = 1; iwd <= 3; iwd++)
                        {
                            if(!IsValueAvailable(new string[]{ "m_" + pageid + "_"+iwd.ToString()+"jddwd",
                                "c_" + (ibc + 1).ToString() + "_" + iwd.ToString() + "bc"}))
                                continue;
                            string wdd = StrValue("m_" + pageid + "_" + iwd.ToString() + "jddwd").Trim();

                            if (wdd == "")
                                continue;
                            string sjz = FormatValue(data_record.Property("c_" + (ibc + 1).ToString() + "_" + iwd.ToString() + "sjzbc"), false);
                            wdd = ("   " + wdd);
                            wdd = wdd.Remove(0,wdd.Length - 3);
                            rows.Add(String.Format("R（{0}℃）\t{1}", wdd, sjz));
                        }
                        string key = "c_" + (ibc + 1).ToString() + "_alphabc";
                        if (!IsValueAvailable(new string[] { key }))
                            continue;
                        string alpha = FormatValue(data_record.Property(key),false);
                        if (alpha != "")
                        {
                            rows.Add(String.Format("α\t{0}", alpha));
                        }
                    }
                }
                if (pageid == "")
                {
                    MessageBox.Show("未找到出厂编号为" + ccbh + "的铂电阻，请检查后再试");
                    return;
                }
                string tmpl = "";
                if (rule == REPORT_TYPE.INFO_REPORT)
                    tmpl = "结果通知书";
                if (rule == REPORT_TYPE.CALI_REPORT)
                    tmpl = "检定证书";
                if (rule == REPORT_TYPE.JIAOZHUN_REPORT)
                    tmpl = "校准证书";
                
                    string src = Path.Combine(Util.basedir, "报告模板\\"+tmpl+".doc");
                    File.Copy(src, tofile, true);
                    doctool.PrepareWord(tofile);
                    docopen = true;

                    doctool.FillInField(tofile, data_record.Properties(), pageid,ibcid);
                    doctool.FillInTableByBookMarks("ITEMS", rows);

                    doctool.FillInHeader(tofile, data_record.Properties(), ibcid, Microsoft.Office.Interop.Word.WdSeekView.wdSeekCurrentPageHeader);
                    doctool.FillInHeader(tofile, data_record.Properties(), ibcid,Microsoft.Office.Interop.Word.WdSeekView.wdSeekPrimaryHeader);
                    
                    doctool.SaveWord(tofile);
                    MessageBox.Show("导出"+tmpl+"成功");
                    docopen = false;
                    return;
            }
            catch (Exception ex)
            {
                MessageBox.Show("导出报告失败: "+ex.Message);
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
        
        public bool CopyToOtherPages(string key, Object value)
        {
            bool ret = false;
            Match m = Regex.Match(key, "m_(a|b|c|d)_");
            if (!m.Success)
                return false;
            char currid = m.Groups[1].Value.ToString()[0];
            string pageid = "abcdef";
            for (int i = 0; i < pageid.Length; i++)
            {
                if (pageid[i] > currid)
                {
                    data_record["m_" + pageid[i] + "_" + key.Remove(0,4)] = ToJValue(value);
                    ret = true;
                }
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

        internal void AutoCaculation(bool min)
        {
            
            #region 3 WIRE caculation
            //this["c_" + sch.ToString() + "_" + ipoint.ToString() + "bcw3_" + round.ToString()] = rnow*GetMultiply();
            for (int round = 1; round <= 4; round++)
            {
                for (int ipoint = 1; ipoint <= MAX_PT_1PAGE; ipoint++)
                {
                    Decimal sum = 0;
                    Decimal count = 0;
                    for (int sch = 0; sch < 24; sch++)
                    {
                        string w3 = "c_" + sch.ToString() + "_" + ipoint.ToString() + "bcw3_" + round.ToString();
                        string w2 = "c_" + sch.ToString() + "_" + ipoint.ToString() + "bcw2_" + round.ToString();
                        if (IsValueAvailable(new string[] { w3,w2}))
                        {
                            this["c_" + sch.ToString() + "_" + ipoint.ToString() + "bc_" + round.ToString()]= 2 * Value(w3)-Value(w2);
                        }
                    }
                }
            }
            #endregion

            if (min)
                return;
            #region average caculation
            //c_1_1sjwd, c_1_1bc1,c_1_2bc1,c_1_3bc1 ... c_1_3bc4
            string[] avg_key = new string[] { "a_{0}sjwd", "b_{0}sjwd","c_{0}sjwd","d_{0}sjwd", "1_{0}bc", "2_{0}bc", "3_{0}bc", "4_{0}bc", "5_{0}bc", "6_{0}bc", "7_{0}bc", "8_{0}bc", "9_{0}bc", "10_{0}bc","11_{0}bc","12_{0}bc" };
            for (int i = 0; i < avg_key.Length; i++)
            {
                for (int ipoint = 1; ipoint <= MAX_PT_1PAGE; ipoint++)
                {
                    Decimal sum = 0;
                    Decimal count = 0;
                    for (int j = 1; j < 5; j++)
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

            string pageid = "abcdef";
            for (int page = 0; page < pageid.Length; page++)
            {
                string pg = pageid[page].ToString() ;
                string mkey;
                string r0_prefix = "";
                string r100_prefix = "";
                ABCParam param = new ABCParam();

                for (int ipoint = 1; ipoint <= MAX_PT_1PAGE; ipoint++)
                {

                    string prefix = "c_" + pg + "_" + ipoint.ToString();
                    

                    mkey = "m_" + pg  +"_"+ ipoint.ToString() + "jddwd";
                    if (!IsValueAvailable(new string[] { mkey , prefix+"sjwd"})) //no setpoint information
                        continue;

                    if (StrValue(mkey).ToString().Trim() == "0")
                        r0_prefix = "c_" + pg + "_" + ipoint.ToString();
                    if (StrValue(mkey).ToString().Trim() == "100")
                        r100_prefix = "c_" + pg + "_" + ipoint.ToString();

                    this[prefix + "xzwd"] = Decimal.Parse(StrValue(mkey)) - Value(prefix + "sjwd"); //修正值


                    if (!param.LoadFromIni("PROBE_" + StrValue("m_a_bzqbh")))
                        throw new Exception("无效的标准器编号");
                    double dydz = Wendu.TValueToRValue(ToDouble(Value(mkey)), param, ConvertMethod.DIN90) -
                                  Wendu.TValueToRValue(ToDouble(Value(prefix + "sjwd")), param, ConvertMethod.DIN90); //对应电阻值
                    this[prefix + "dydz"] = dydz;

                    
                    //caculate all bei ce related column
                    for (int ibc = 4*page+1; ibc <= 4*page+4; ibc++)
                    {
                        prefix = "c_" + ibc.ToString() + "_" + ipoint.ToString();
                        if (!IsValueAvailable(new string[] { prefix + "bc" }))
                            continue;
                        this[prefix + "sjzbc" ] = Value(prefix + "bc" ) + ToDecimal(dydz);
                    }
                }
                if (r0_prefix != "" && r100_prefix != "")
                {
                    for (int ibc = 4 * page + 1; ibc <= 4 * page + 4; ibc++)
                    {
                        if (!IsValueAvailable(new string[] { r100_prefix.Replace("c_" + pg + "_", "c_" + ibc.ToString() + "_") + "sjzbc",
                        r0_prefix.Replace("c_" + pg + "_", "c_" + ibc.ToString() + "_") + "sjzbc",
                        "m_" + ibc.ToString() + "_dj"}))
                            continue;
                        double r100 = ToDouble(Value(r100_prefix.Replace("c_" + pg + "_", "c_" + ibc.ToString() + "_") + "sjzbc"));
                        double r0 = ToDouble(Value(r0_prefix.Replace("c_" + pg + "_", "c_" + ibc.ToString() + "_") + "sjzbc"));
                        double alpha = (r100 - r0) / (100 * r0);
                        this["c_" + ibc.ToString() + "_alphabc"] = ToDecimal(alpha);
                        double deltat0 = (r0 - Wendu.TValueToRValue(ToDouble(0), param, ConvertMethod.DIN90)) / 0.39083;
                        bool valid1 = false;
                        bool valid2 = false;
                        bool valid3 = false;
                        string dg = StrValue("m_" + ibc.ToString() + "_dj").ToString().Trim().ToUpper();
                        
                        if (dg == "AA")
                            valid1 = (Math.Abs(r0 - 100) <= 0.039);
                        if (dg == "A")
                            valid1 = (Math.Abs(r0 - 100) <= 0.059);
                        if (dg == "B")
                            valid1 = (Math.Abs(r0 - 100) <= 0.117);
                        if (dg == "C")
                            valid1 = (Math.Abs(r0 - 100) <= 0.234);

                        if (dg == "AA")
                            valid2 = (Math.Abs(r100 - 138.506) <= 0.102) ;
                        if (dg == "A")
                            valid2 = (Math.Abs(r100 - 138.506) <= 0.133) ;
                        if (dg == "B")
                            valid2 = (Math.Abs(r100 - 138.506) <= 0.303) ;
                        if (dg == "C")
                            valid2 = (Math.Abs(r100 - 138.506) <= 0.607) ;

                        if (dg == "AA")
                            valid3 = (Math.Abs(alpha - 0.003851 + deltat0 * 30 * 1e-6) <= 7.0);
                        if (dg == "A")
                            valid3 = (Math.Abs(alpha - 0.003851 + deltat0 * 23 * 1e-6) <= 7.0);
                        if (dg == "B")
                            valid3 = (Math.Abs(alpha - 0.003851 + deltat0 * 21 * 1e-6) <= 14.0);
                        if (dg == "C")
                            valid3 = (Math.Abs(alpha - 0.003851 + deltat0 * 21 * 1e-6) <= 32.0);

                        if (valid1 && valid2 && valid3)
                            this["c_" + ibc.ToString() + "_jl"] = "合格";
                        else
                        {
                            string fail = "";
                            if (!valid1)
                                fail = "0度超差";
                            if (!valid2)
                                fail = fail + ";100度超差";
                            if (!valid3)
                                fail = fail + ";α超差";

                            this["c_" + ibc.ToString() + "_jl"] = fail.Trim(new char[]{';'});
                        }
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
            
            return data_record.Property(item).Value["value"].ToObject<Decimal>();
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
                        if(StrValue(item) == "")
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
            if (jp.Name.StartsWith("m_") || jp.Name.EndsWith("_jl"))
            {   //manual input case
                return jp.Value["value"].ToString();
            }
            string dg = "";
            Match m = Regex.Match(jp.Name,"(m|c)_(\\d)_");
            if(m.Success)
                dg = this["m_" + m.Groups[2].Value + "_dj"].ToString().Trim().ToUpper();

            if (Regex.IsMatch(jp.Name, ("\\d(sjwd)_\\d")) ||
                Regex.IsMatch(jp.Name, ("\\d(sjwd|xzwd|sjzwd)")))
            {   //resistance case or sjwd
                if (dg == "AAA")
                    return Decimal.Parse(jp.Value["value"].ToString()).ToString("F03");
                else
                    return Decimal.Parse(jp.Value["value"].ToString()).ToString("F02");
            }

            if (Regex.IsMatch(jp.Name,("\\d(bc|sjwd)_\\d")) ||
                Regex.IsMatch(jp.Name, ("\\d(bc|xzbc|dydz|dydzbc|sjzbc)")))
            {   //resistance case or sjwd
                if(dg == "AAA")
                    return Decimal.Parse(jp.Value["value"].ToString()).ToString("F04");
                else
                    return Decimal.Parse(jp.Value["value"].ToString()).ToString("F03");
            }
            if (Regex.IsMatch(jp.Name, "alphabc"))
            {   //resistance case or sjwd
                return Decimal.Parse(jp.Value["value"].ToString()).ToString("F07");
            }
            if (Regex.IsMatch(jp.Name, "jdrq_(n|y|r)") || Regex.IsMatch(jp.Name, "yxqz_(n|y|r)"))
                return jp.Value["value"].ToString();
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
