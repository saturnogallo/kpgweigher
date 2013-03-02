using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Data.SQLite;
using System.ComponentModel;
using System.Data;
using System.Xml.Linq;
using System.IO;
namespace Zddq2
{
    /*
     * define channel info
     */
    public class RX_VAR
    {
        public double vRs;
        public double vRx;
        public double vCrossP;
        public double vCrossN;
        public double iSrc;
        public double rRx;
        public double rRs;
        
        public int iK;  
        public Int32[] track_weight;
        public byte ptr_tw;
        public string errMsg;
        public RX_VAR()
        {
            track_weight = new Int32[4];
            ptr_tw = 0;
        }
        public static int INIT_LOOP = 1000;
        public static int MAX_LOOP = 8191;
        //Get a new K value in non tracking mode
        //return the difference between new K and current k value
        public int calc_capture_nextk()
        {
            int N;
            N = Convert.ToInt32(Math.Floor(0.5+ rRx * INIT_LOOP / rRs));
            if (N < 1)    N = 1;
            if (N > MAX_LOOP) N = MAX_LOOP;
            return N;
        }
        //Get a new K value in tracking mode                     
        //else use the weight algorithm
        public int calc_track_nextk(int iRange)
        {
            double dK = (double)iK / (double)INIT_LOOP;
            rRx = dK * (rRs * RunWnd.syscfg.GetScaleAdjust(iRange)*(1e-6) + rRs + ((vCrossP - vCrossN) / (2.0 * (double)dK * Math.Abs(iSrc))));// todo add the variance

            return calc_capture_nextk();
        }
        public void log_start(int iRange)
        {
            DeviceMgr.SysLog(String.Format("iK={0},Scale={1},vP={2},vN={3},Curr={4},Rx={5}", iK.ToString(),
                Util.FormatData(RunWnd.syscfg.GetScaleAdjust(iRange), 8),
                Util.FormatData(vCrossP, 8),
                Util.FormatData(vCrossN, 8),
                Util.FormatData(iSrc, 8),
                Util.FormatData(rRx, 8)));
        }
    }
    public class RxInfo : IComparable
    {
        public int CompareTo(object obj)
        {
            if (obj is RxInfo)
            {
                RxInfo rx = obj as RxInfo;
                if (this.bEnabled && !rx.bEnabled)
                    return 1;
                if (!this.bEnabled && rx.bEnabled)
                    return -1;

                if (this.cChan < rx.cChan)
                    return -1;
                if (this.cChan > rx.cChan)
                    return 1;
                return 0;
            }
            return 1;
        }
        private SqlConfig config;
        public RX_VAR var;
        public int i_MeasCount //count of measure times
        {
            get;
            set;
        }
        public RUN_STATE i_State     //state of Measurement
        {
            get;
            set;
        }
        public bool bRunning   //whether the Rx is under measurement
        {
            get;
            set;
        }
        public double dRxInput  //target rx value
        {
            get
            {
                try
                {
                    return (double)this["dRxInput"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dRxInput"] = value;
            }
        }
        public RxInfo(int id)
        {
            config = new SqlConfig("RxInfo", id.ToString());
            var = new RX_VAR();
            i_MeasCount = 0;
            bRunning = false;
        }
        public int iMeasDelay
        {
            get
            {
                return (int)this["iMeasDelay"];
            }
            set
            {
                this["iMeasDelay"] = value;
            }
        }
        public int iMeasTimes
        {
            get
            {
                return (int)this["iMeasTimes"];
            }
            set
            {
                this["iMeasTimes"] = value;
            }
        }
        public int iStdChan
        {
            get
            {
                return 1;//int)this["iStdChan"];
            }
            set
            {
                this["iStdChan"] = value;
            }
        }
        public int iIx
        {
            get
            {
                return (int)this["iIx"];
            }
            set
            {
                this["iIx"] = value;
            }
        }
        public int iRRange
        {
            get
            {
                return (int)this["iRRange"];
            }
            set
            {
                this["iRRange"] = value;
            }
        }
        public int iVMode
        {
            get
            {
                return (int)this["iVMode"];
            }
            set
            {
                this["iVMode"] = value;
            }
        }
        public char cStdChan
        {
            get
            {
                return (char)this["cStdChan"];
            }
            set
            {
                this["cStdChan"] = value;
            }
        }
        public char cChan
        {
            get
            {
                return (char)this["cChan"];
            }
            set
            {
                this["cChan"] = value;
            }
        }
        public bool bEnabled
        {
            get
            {
                return (bool)this["bEnabled"];
            }
            set
            {
                this["bEnabled"] = value;
            }
        }
        public bool bSqrt
        {
            get
            {
                return (bool)this["bSqrt"];
            }
            set
            {
                this["bSqrt"] = value;
            }
        }
        public string sSerial
        {
            get
            {
                return (string)this["sSerial"];
            }
            set
            {
                this["sSerial"] = value;
            }
        }
        /*
         * Fixed configuration includes:
         * iMeasDelay :  delay between each sample (second)
         * iMeasTimes :  measure times
         * iStdChan   :  channel index of standard
         * cStdChan   :  A/B of standard channel
         * iChan      :  my channel index
         * cChan      :  A/B of my channel
         * bEnabled   :  whether it is enabled
         * iIx        :  current to use
         * bSqrt      :  sqrt function enabled or not
         * sSerial    :  serial number to use
         */
        public object this[string id]
        {
            get
            {
                return config[id];
            }
            set
            {
                config[id] = value;
            }
        }
    }
    public class RsInfo
    {
        private SqlConfig config;
        public RsInfo(int id)
        {
            config = new SqlConfig("RsInfo", id.ToString());
        }
        public int iRRange
        {
            get
            {
                return (int)this["iRRange"];
            }
            set
            {
                this["iRRange"] = value;
            }
        }
        public double dTValue
        {
            get
            {
                return dValue * (1 + (1e-6) * dAlpha * (RunWnd.syscfg.dTemp - 20) + (1e-6) * dBeta * (RunWnd.syscfg.dTemp - 20) * (RunWnd.syscfg.dTemp - 20)); 
            }
        }
        public double dValue
        {
            get
            {
                return (double)this["dValue"+iRRange.ToString()];
            }
            set
            {
                this["dValue" + iRRange.ToString()] = value;
            }
        }
        public double dAlpha
        {
            get
            {
                return (double)this["dAlpha" + iRRange.ToString()];
            }
            set
            {
                this["dAlpha" + iRRange.ToString()] = value;
            }
        }
        public double dBeta
        {
            get
            {
                return (double)this["dBeta" + iRRange.ToString()];
            }
            set
            {
                this["dBeta" + iRRange.ToString()] = value;
            }
        }
        public string sSerial
        {
            get
            {
                return (string)this["sSerial" + iRRange.ToString()];
            }
            set
            {
                this["sSerial" + iRRange.ToString()] = value;
            }
        }

        /*  Fixed Configuration
         * dValue     : Rs value
         * dAlpha     : Alpha value
         * dBeta      : Beta value
         * sSerial    : Serial number
         */
        public object this[string id]
        {
            get
            {
                return config[id];
            }
            set
            {
                config[id] = value;
            }
        }
    }
    public class UIConfig
    {
        private SqlConfig config;

        public UIConfig(int disp)
        {
            config = new SqlConfig("UIInfo", disp.ToString());
        }

        public RXDATA_MODE iMainData
        {
            get
            {
                return (RXDATA_MODE)this["iMainData"];
            }
            set
            {
                this["iMainData"] = (int)value;
            }
        }
        public RXDATA_MODE iAux1Data
        {
            get
            {
                return (RXDATA_MODE)this["iAux1Data"];
            }
            set
            {
                this["iAux1Data"] = (int)value;
            }
        }
        public RXDATA_MODE iAux2Data
        {
            get
            {
                return (RXDATA_MODE)this["iAux2Data"];
            }
            set
            {
                this["iAux2Data"] = (int)value;
            }
        }
        public RXDISP_MODE iDataMode
        {
            get
            {
                return (RXDISP_MODE)this["iDataMode"];
            }
            set
            {
                this["iDataMode"] = (int)value;
            }
        }
        public object this[string id]
        {
            get
            {
                return config[id];
            }
            set
            {
                config[id] = value;
            }
        }
    }
    /*
     * Define system configuration
     */
    public class SysConfig
    {
        private SqlConfig config;

        public SysConfig()
        {
            config = new SqlConfig("SysInfo", "pc");
        }
        public int iMeasTimes
        {
            get
            {
                return (int)this["iMeasTimes"];
            }
            set
            {
                this["iMeasTimes"] = value;
            }
        }

        public int iSampleTimes
        {
            get
            {
                return (int)this["iSampleTimes"];
            }
            set
            {
                this["iSampleTimes"] = value;
            }
        }
        public int iMeasDelay
        {
            get
            {
                return (int)this["iMeasDelay"];
            }
            set
            {
                this["iMeasDelay"] = value;
            }
        }
        public int iFilter
        {
            get
            {
                return (int)this["iFilter"];
            }
            set
            {
                if(value >= 1)
                    this["iFilter"] = value;
            }
        }
        public double GetCurrent(int iIx, bool bsqrt)
        {
            double val= 0;
            if (iIx == 5)
            {
                if(!bsqrt)
                    val = (double)this["d5A"];
                else
                    val = (double)this["d10A"];
            }
            if (iIx == 4)
            {
                if (!bsqrt)
                    val = (double)this["d1A"];
                else
                    val = (double)this["d2A"];
            }
            if (iIx == 3)
            {
                if (!bsqrt)
                    val = (double)this["d300mA"];
                else
                    val = (double)this["d600mA"];
            }
            if (iIx == 2)
            {
                if (!bsqrt)
                    val = (double)this["d100mA"];
                else
                    val = (double)this["d200mA"];
            }
            if (iIx == 1)
            {
                if (!bsqrt)
                    val = (double)this["d10mA"];
                else
                    val = (double)this["d20mA"];
            }
            if (iIx == 0)
            {
                if (!bsqrt)
                    val = (double)this["d1mA"];
                else
                    val = (double)this["d2mA"];
            }
            if (iIx == -1)
            {
                if (!bsqrt)
                    val = (double)this["dP1mA"];
                else
                    val = (double)this["dP2mA"];
            }
            double stdcurr = GetStdCurrent(iIx, bsqrt);
            if (Math.Abs(stdcurr - val) > stdcurr * 0.1) //too big variance
                return stdcurr;
            else
                return val;
        }
        public double GetStdCurrent(int iIx, bool bsqrt)
        {
            if (iIx == 5)
            {
                if (!bsqrt)
                    return 5;
                else
                    return 10;
            }
            if (iIx == 4)
            {
                if (!bsqrt)
                    return 1;
                else
                    return 2;
            }
            if (iIx == 3)
            {
                if (!bsqrt)
                    return 0.3;
                else
                    return 0.6;
            }
            if (iIx == 2)
            {
                if (!bsqrt)
                    return 0.1;
                else
                    return 0.2;
            }
            if (iIx == 1)
            {
                if (!bsqrt)
                    return 0.01;
                else
                    return 0.02;
            }
            if (iIx == 0)
            {
                if (!bsqrt)
                    return 0.001;
                else
                    return 0.002;
            }
            if (iIx == -1)
            {
                if (!bsqrt)
                    return 0.0001;
                else
                    return 0.0002;
            }
            return 100;
        }

        public void SetCurrent(int iIx, bool bsqrt, double val)
        {
            if (iIx == 5)
            {
                if (!bsqrt)
                    this["d5A"] = Math.Abs(val);
                else
                    this["d10A"] = Math.Abs(val);
            }
            if (iIx == 4)
            {
                if (!bsqrt)
                    this["d1A"] = Math.Abs(val);
                else
                    this["d2A"] = Math.Abs(val);
            }
            if (iIx == 3)
            {
                if (!bsqrt)
                    this["d300mA"] = Math.Abs(val);
                else
                    this["d600mA"] = Math.Abs(val);
            } 
            if (iIx == 2)
            {
                if (!bsqrt)
                    this["d100mA"] = Math.Abs(val);
                else
                    this["d200mA"] = Math.Abs(val);
            }
            if (iIx == 1)
            {
                if (!bsqrt)
                    this["d10mA"] = Math.Abs(val);
                else
                    this["d20mA"] = Math.Abs(val);
            }
            if (iIx == 0)
            {
                if (!bsqrt)
                    this["d1mA"] = Math.Abs(val);
                else
                    this["d2mA"] = Math.Abs(val);
            }
            if (iIx == -1)
            {
                if (!bsqrt)
                    this["dP1mA"] = Math.Abs(val);
                else
                    this["dP2mA"] = Math.Abs(val);
            }
        }
       
        public double GetScaleAdjust(int iRRange)
        {
            try
            {
                return (double)this["dAdjust" + iRRange.ToString()];
            }
            catch
            {
                return 0;
            }
        }
        public void SetScaleAdjust(int iRRange, double val)
        {
            this["dAdjust" + iRRange.ToString()] = val;
        }
        public double dTemp
        {
            get
            {
                double val = 0;
                try
                {

                    val = double.Parse(this["dTemp"].ToString());
                }
                catch
                {
                    
                }
                return val;
            }
            set
            {
                this["dTemp"] = value;
            }
        }
        public string sNavmeter
        {
            get
            {
                string nav = (string)this["sNavmeter"];
                if (nav == "")
                    nav = "PZ158";
                return nav;
            }
            set
            {
                this["sNavmeter"] = value;
            }
        }
        public string sFilterType
        {
            get
            {
                return (string)this["sFilterType"];
            }
            set
            {
                this["sFilterType"] = value;
            }
        }
        public int iDisplay
        {
            get
            {
                return (int)this["iDisplay"];
            }
            set
            {
                this["iDisplay"] = value;
            }
        }
        public int iKTT
        {
            get
            {
                return ((int)this["iKTT"]);
            }
            set
            {
                
                this["iKTT"] = value;
            }
        }
        public bool bThrow
        {
            get
            {
                return (1 == ((int)this["iThrow"]));
            }
            set
            {
                if(value)
                    this["iThrow"] = 1;
                else
                    this["iThrow"] = 0;
            }
        }

        /*  Fixed Configuration
         * iSampleTimes : times per sample
         * iMeasDelay : delay per sample
         * iFilter      : filter length
         * sFilterType  : filter type
         * iDisplay     : display mode
         */
        
        public object this[string id]
        {
            get
            {
                return config[id];
            }
            set
            {
                config[id] = value;
            }
        }
        
    }
    /*
     * Manage Sqlite based config file
     * All configure table has following structure 
     * id value group table
     * for current configuration id='current' grp='current'
     */
    internal class SqlConfig
    {
        static private SQLiteConnection sql_con;
        static private SQLiteCommand sql_cmd;

        private SQLiteDataAdapter DB;
        private DataSet DS = new DataSet();
        private DataTable DT = new DataTable();

        private string sql_tbl;
        private string sql_grp;
        static void SetConnection()
        {
            sql_con = new SQLiteConnection("Data Source=" + StringResource.basedir + "\\Config.db;Version=3;New=False;Compress=True;");
        }
        private Dictionary<string, string> curr_conf; //store all the configuration string
        public SqlConfig(string sql_tbl,string group)
        {
            curr_conf = new Dictionary<string, string>();
            this.sql_tbl = sql_tbl;
            this.sql_grp = group;
            LoadConfigFromFile();
        }

        private bool LoadConfigFromFile()
        {
            try
            {
                string CommandText;
                SetConnection();
                sql_con.Open();

                //retrieve current configure
                CommandText = "select id,value from data where grp='"+this.sql_grp+"' and tbl='" + this.sql_tbl + "'";
                DB = new SQLiteDataAdapter(CommandText, sql_con);
                DS.Reset();
                DB.Fill(DS);
                foreach (DataRow dr in DS.Tables[0].Rows)
                {
                        curr_conf[dr[0].ToString()] = dr[1].ToString();
                }
                sql_con.Close();
                return true;
            }
            catch (System.Exception e)
            {
                Program.MsgShow(e.Message);
                return false;
            }

        }
        private bool SaveConfigToFile(string id, string val)
        {

            try
            {
                //save the current configuration 
                //SetConnection();
                sql_con.Open();
                sql_cmd = new SQLiteCommand();
                sql_cmd.Connection = sql_con;
                sql_cmd.CommandText = "replace into data(id,value,grp,tbl) values('"+id+"','" + val + "','"+this.sql_grp+"','" + this.sql_tbl + "')";
                sql_cmd.ExecuteNonQuery();
                sql_con.Close();
                return true;
            }
            catch (System.Exception e)
            {
                Program.MsgShow(e.Message);
                return false;
            }
        }
        public IEnumerable<String> Keys
        {
            get
            {
                return curr_conf.Keys;
            }
        }
        public object this[string id]
        {
            get
            {
                string val = "";
                if (curr_conf.ContainsKey(id))
                    val = curr_conf[id];

                if (id.StartsWith("b")) //bool 
                    return (val == bool.TrueString);
                if (id.StartsWith("c")) //char
                {
                    if (val.Length > 1)
                        return val[0];
                    else
                        return " ";
                }
                if (id.StartsWith("i")) //int
                {
                    if (val == "")
                        return 0;
                    else
                        return Convert.ToInt32(val);
                }
                if (id.StartsWith("d")) //double
                {
                    if (val == "")
                        return 0.0;
                    else
                        return Convert.ToDouble(val);
                }
                return val;
            }
            set
            {
                string val;
                if (value != null)
                    val = value.ToString();
                else
                    val = "";
                if (curr_conf.ContainsKey(id) && (curr_conf[id]==val))
                    return;
                if(SaveConfigToFile(id,val))
                    curr_conf[id] = val;
            }
        }
        
    }

    internal class StringResource
    {
        public static string udiskdir = "\\Hard Disk";
        public static string basedir = "\\NandFlash\\TSioex";
        private static Dictionary<string, string> str_tbl;
        static public string language;

        static public void SetLanguage(string lang)
        {
            language = lang;
            string lang_file = "Zddq2.Resource." + lang + ".xml";
            StreamReader sr = new StreamReader(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream(lang_file));
            XDocument xml_doc = XDocument.Load(sr);

            IEnumerable<XElement> regs = xml_doc.Descendants("string");
            //str_tbl.Clear();
            foreach (XElement reg in regs)
            {
                str_tbl[reg.Attribute("skey").Value] = reg.Value;
            }
        }
        static StringResource()
        {
            str_tbl = new Dictionary<string, string>();
            if (!File.Exists(StringResource.basedir + "\\history.log"))
                File.Create(StringResource.basedir + "\\history.log").Close();
            FileStream fsLog = new FileStream(StringResource.basedir + "\\history.log", FileMode.Truncate, FileAccess.Write, FileShare.Read);
            fsLog.Close();
            SetLanguage("zh-CN");


            str_tbl["NAV_INIT_PZ158"] = "Un%%01;00\r";
            str_tbl["NAV_10MV_PZ158"] = "Un%%01;12;00\r";
            str_tbl["NAV_120MV_PZ158"] = "Un%%01;12;00\r";
            str_tbl["NAV_1V_PZ158"] = "Un%%01;12;01\r";
            str_tbl["NAV_30V_PZ158"] = "Un%%01;12;02\r";
            str_tbl["NAV_AFLTOFF_PZ158"] = "Un%%01;26\r";
            str_tbl["NAV_AFLTON_PZ158"] = "Un%%01;27\r";
            str_tbl["NAV_ZEROON_PZ158"] = "Un%%01;06\r";
            str_tbl["NAV_READ_PZ158"] = "Un%%01;01\r";

            str_tbl["NAV_INIT_PZ2182"] = "Un%%01;00\r";
            str_tbl["NAV_10MV_PZ2182"] = "Un%%01;12;00\r";
            str_tbl["NAV_120MV_PZ2182"] = "Un%%01;12;01\r";
            str_tbl["NAV_1V_PZ2182"] = "Un%%01;12;02\r";
            str_tbl["NAV_30V_PZ2182"] = "Un%%01;12;02\r";
            str_tbl["NAV_AFLTOFF_PZ2182"] = "Un%%01;26\r";
            str_tbl["NAV_AFLTON_PZ2182"] = "Un%%01;27\r";
            str_tbl["NAV_ZEROON_PZ2182"] = "Un%%01;06\r";
            str_tbl["NAV_READ_PZ2182"] = "Un%%01;01\r";

            str_tbl["NAV_INIT_2182"] = "Un*RST\n*CLS\n:init:cont on;:ABORT\n:sens:func 'volt:dc'\n:sense:chan 1\n:sens:volt:rang:auto on\n:sens:volt:chan1:lpas off\n:SENS:VOLT:DC:NPLC 1\nVOLT:DIG 8\n:syst:azer on\n";
            str_tbl["NAV_10MV_2182"] = "Un:sens:volt:chan1:rang 0.01\n";
            str_tbl["NAV_120MV_2182"] = "Un:sens:volt:chan1:rang 0.1\n";
            str_tbl["NAV_1V_2182"] = "Un:sens:volt:chan1:rang 1\n";
            str_tbl["NAV_30V_2182"] = "Un:sens:volt:chan1:rang 10\n";
            str_tbl["NAV_AFLTOFF_2182"] = "Un:sens:volt:chan1:dfil:stat off\n";
            str_tbl["NAV_AFLTON_2182"] = "Un:sens:volt:chan1:dfil:wind 5\n:sens:volt:chan1:dfil:coun 10\n:sens:volt:chan1:dfil:tcon mov\n:sens:volt:chan1:dfil:stat on\n";
            str_tbl["NAV_ZEROON_2182"] = "Un:sens:volt:ref:acq\n:sens:volt:ref:stat on\n";
            str_tbl["NAV_READ_2182"] = "Un:fetc?\n";
        }
        static public void dolog(string log)
        {
            try
            {
                FileStream fsLog = new FileStream("history.log", FileMode.Append, FileAccess.Write, FileShare.Read);
                StreamWriter sw = new StreamWriter(fsLog);
                sw.WriteLine(DateTime.Now.ToString("G") + "\t" + log);
                sw.Close();
                fsLog.Close();
            }
            catch (System.Exception)
            {

            }

        }

        public static string str(string key)
        {
            if (str_tbl.ContainsKey(key))
            {
                return str_tbl[key];
            }
            else
            {
                return "Invalid String Key";
            }

        }
        private StringResource() { }

    }

}
