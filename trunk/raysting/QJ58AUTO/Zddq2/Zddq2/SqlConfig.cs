using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Finisar.SQLite;
using System.ComponentModel;
using System.Data;
using System.Xml.Linq;
using System.IO;
using Raysting.Common;
using Raysting.Controls;
namespace Zddq2
{
    /*
     * define channel info
     * Store the temperary variables during Rx measuring
     */
    public class RX_VAR
    {
        public double vRs;  //voltage on Rs
        public double vRx;  //voltage on Rs
        public double vCrossP; //cross voltage P
        public double vCrossN; //cross voltage N
        public double iSrc;    //current from current source , equals to (current go through Rx * iK ?);
        public double rRx;     //measured rRx
        public double rRs;     //Rs setting
        
        public int iK;         //current K value
        public string errMsg;   //store error message
        public RxInfo rx;
        public RX_VAR(RxInfo rxinfo)
        {
            rx = rxinfo;
            Reset();
        }
        public void Reset()
        {
            i_MeasCount = 0;
            i_State = RUN_STATE.IDLE;
            bRunning = false;
        }
        public static int INIT_LOOP = 800; //default K value on bootup
        public static int MAX_LOOP = 8191; //max loop available.
        //Get a new K value in non tracking mode
        //return the difference between new K and current k value
        public int calc_capture_nextk()
        {
            int N;
            N = Convert.ToInt32(Math.Floor(0.5+ rRx * INIT_LOOP / rRs));
            if (N < 1)    N = 1;
            if (N > MAX_LOOP) N = MAX_LOOP;
            return N - iK;
        }
        //Get a new K value in tracking mode                     
        //else use the weight algorithm
        public int calc_track_nextk()
        {
            int N;
            int sum;
            double dK = (double)iK / (double)INIT_LOOP;
            rRx = (dK * (rRs + ((vCrossP - vCrossN) / (2.0 * Math.Abs(iSrc)))));
            return calc_capture_nextk();
        }
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
    }

    //Information related to Rx
    /*
     * Fixed configuration includes:
     * iMeasDelay :  delay between each sample (second)
     * iMeasTimes :  measure times
     * iChan      :  Rx channel index
     * cChan      :  A/B of my channel, only used for self check
     * bEnabled   :  whether it is enabled
     * iIx        :  current to use
     * bSqrt      :  sqrt function enabled or not
     * sSerial    :  serial number to use
     * dCoefA,B,C :  for temperature conversion
     * sPTType    :  PT value
     */
    public class RxInfo : IComparable
    {
        //for sorting of Rx according to channel number, and enabled channel goes first.
        public int CompareTo(object obj)
        {
            if (obj is RxInfo)
            {
                RxInfo rx = obj as RxInfo;
                if (this.bEnabled && !rx.bEnabled)
                    return 1;
                if (!this.bEnabled && rx.bEnabled)
                    return -1;

                if (this.iChan < rx.iChan)
                    return -1;
                if (this.iChan > rx.iChan)
                    return 1;
                return 0;
            }
            return 1;
        }
        private SqlConfig config; //configuration dictionary
        public RX_VAR var; //run time variable

        private bool _bSelected = true;
        public bool bSelected
        {
            get
            {
                if (!bEnabled)
                    return false;
                return _bSelected;
            }
            set
            {
                if (!bEnabled)
                    return;
                _bSelected = value;
            }
        }
        public RxInfo(int id)
        {
            iChan = id;
            config = new SqlConfig("RxInfo", id.ToString());
            var = new RX_VAR(this);
        }
        public double dRxInput  //target rx value, no use now
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

        public bool bTempProbe  // whether the probe is a standard temperature probe , instead of a unknown probe
        {
            get
            {
                return (bool)this["bTempProbe"];
            }
            set
            {
                this["bTempProbe"] = value;
            }
        } 
        #region temperature conversion related parameters
        public double dCoefA  //Coeficient A for abcparam
        {
            get
            {
                try
                {
                    return (double)this["dCoefA"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dCoefA"] = value;
            }
        }
        public double dCoefB  //Coeficient B
        {
            get
            {
                try
                {
                    return (double)this["dCoefB"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dCoefB"] = value;
            }
        }
        public double dCoefC  //Coeficient C
        {
            get
            {
                try
                {
                    return (double)this["dCoefC"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dCoefC"] = value;
            }
        }
        public TempRange tRange //temperature range for convert use
        {
            get
            {
                try
                {
                    return (TempRange)this["iTempRange"];
                }
                catch
                {
                    return 0;
                }
            }
            set
            {
                this["iTempRange"] = (int)value;
            }
        }
        public double dRtp  //Rtp value
        {
            get
            {
                try
                {
                    return (double)this["dRtp"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dRtp"] = value;
            }
        }
        #endregion
        
        public int iMeasDelay   //delay between each voltage reading (second)
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
        public int iMeasTimes   //total measurement times
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
        public double dIx   //real current setting based on iIx
        {
            get
            {
                switch (iIx)
                {
                    //TODO
                    case 0: return 2e-6;
                    case 1: return 5e-6;
                    case 2: return 1e-5;
                    case 3: return 1e-4;
                    case 4: return 1e-3;
                    case 5: return 3e-3;
                    case 6: return 1e-3;
                }
                throw new Exception("Expected Current Setting");
            }
        }
        public int iIx  //index of current setting.
        {
            get
            {
                return (int)this["iIx"];
            }
            set
            {
                if(value >=0 && value <= 6)
                    this["iIx"] = value;
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
        } //whether use sqrt current value

        public int iRRange  //index of range for resistance
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
        /// <summary>
        /// A or B , for self check only
        /// </summary>
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

        public int iChan
        {
            get;set;
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

        
        /// <summary>
        /// Serial number for the probe
        /// </summary>
        public string sSerial
        {
            get
            {
                string ret = (string)this["sSerial"];
                if (ret == "")
                    return "PROBE" + config.sql_grp;
                else
                    return ret;
            }
            set
            {
                this["sSerial"] = value;
            }
        }

        /// <summary>
        /// PT type for probe
        /// </summary>
        public string sPTType
        {
            get
            {
                return (string)this["sPTType"];
            }
            set
            {
                if (value.StartsWith("PT"))
                {
                    if (value == "PT10")
                    {
                        dRxInput = 10;
                        //todo set other default parameters
                    }

                    if (value == "PT25")
                    {
                        dRxInput = 25;
                        //todo set other default parameters
                    }
                    if (value == "PT100")
                    {
                        dRxInput = 100;
                        //todo set other default parameters
                    }
                    if (value == "PT1000")
                    {
                        dRxInput = 1000;
                        //todo set other default parameters
                    }
                    if (value == "标准电阻")
                    {
                        bTempProbe = false;
                    }
                    else
                    {
                        bTempProbe = true;
                    }
                }
                this["sPTType"] = value;
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
    public class RsInfo
    {
        private SqlConfig config;

        public bool bSelected = true;
        public int iChan;
        public RsInfo(int id)
        {
            iChan = id;
            config = new SqlConfig("RsInfo", id.ToString());
        }

        /// <summary>
        /// PT type for probe
        /// </summary>
        public string sPTType
        {
            get
            {
                return (string)this["sPTType"];
            }
            set
            {
                if (value.StartsWith("PT"))
                {
                    if (value == "PT10")
                    {
                        dValue = 10;
                        //todo set other default parameters
                    }

                    if (value == "PT25")
                    {
                        dValue = 25;
                        //todo set other default parameters
                    }
                    if (value == "PT100")
                    {
                        dValue = 100;
                        //todo set other default parameters
                    }
                    if (value == "PT1000")
                    {
                        dValue = 1000;
                        //todo set other default parameters
                    }
                    if (value == "标准电阻")
                    {
                        bTempProbe = false;
                    }
                    else
                    {
                        bTempProbe = true;
                    }
                }
                this["sPTType"] = value;
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
        public double dTValue   //adjusted value based on temperature
        {
            get
            {
                if (bStdRs)
                    return dValue * (1 + (1e-6) * dAlpha * (Program.sysinfo.dTemp - 20) + (1e-6) * dBeta * (Program.sysinfo.dTemp - 20) * (Program.sysinfo.dTemp - 20));
                else
                    return dValue;
            }
        }

        public bool bStdRs  //whether it is a known standard resistance
        {
            get
            {
                return (bool)this["bStdRs"];
            }
            set
            {
                this["bStdRs"] = value;
            }
        }
        #region standared resistance related setting

        public double dValue    //nominal value
        {
            get
            {
                    return (double)this["dValue"];
                
            }
            set
            {
                this["dValue"] = value;
            }
        }
        public double dMaxCurr    //Maximum current
        {
            get
            {
                return (double)this["dMaxCurrent"];
            }
            set
            {
                this["dMaxCurrent"] = value;
            }
        }
        public double dAlpha    //temperature coefficient of alpha
        {
            get
            {
                return (double)this["dAlpha"];
            }
            set
            {
                this["dAlpha"] = value;
            }
        }
        public double dBeta     //temperature coefficient of beta
        {
            get
            {
                return (double)this["dBeta"];
            }
            set
            {
                this["dBeta"] = value;
            }
        }
        #endregion
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
        public bool bTempProbe  // whether the probe is a standard temperature probe , instead of a unknown probe
        {
            get
            {
                return (bool)this["bTempProbe"];
            }
            set
            {
                this["bTempProbe"] = value;
            }
        }
        #region temperature conversion related parameters
        public double dCoefA  //Coeficient A for abcparam
        {
            get
            {
                try
                {
                    return (double)this["dCoefA"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dCoefA"] = value;
            }
        }
        public double dCoefB  //Coeficient B
        {
            get
            {
                try
                {
                    return (double)this["dCoefB"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dCoefB"] = value;
            }
        }
        public double dCoefC  //Coeficient C
        {
            get
            {
                try
                {
                    return (double)this["dCoefC"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dCoefC"] = value;
            }
        }
        public TempRange tRange //temperature range for convert use
        {
            get
            {
                try
                {
                    return (TempRange)this["iTempRange"];
                }
                catch
                {
                    return 0;
                }
            }
            set
            {
                this["iTempRange"] = (int)value;
            }
        }
        public double dRtp  //Rtp value
        {
            get
            {
                try
                {
                    return (double)this["dRtp"];
                }
                catch
                {
                    return 0.000001;
                }
            }
            set
            {
                this["dRtp"] = value;
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
        #endregion

        /*  Fixed Configuration
         * dValue     : Rs value
         * dAlpha     : Alpha value
         * dBeta      : Beta value
         * sSerial    : Serial number
         * dMaxCurrent: maximum current value     
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
    //variable for display setting configuration
    public class UIConfig
    {
        private SqlConfig config;

        public UIConfig(int disp)
        {
            config = new SqlConfig("UIInfo", disp.ToString());
        }

        public int iDataMode    //current display mode
        {
            get
            {
                return (int)this["iDataMode"];
            }
            set
            {
                this["iDataMode"] = (int)value;
            }
        }
        public string sChannel  //current top channel display
        {
            get
            {
                return (string)this["sChannel"];
            }
            set
            {
                this["sChannel"] = (string)value;
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

    public enum RXDISP_MODE
    {

        GRID,
        GRAPH,
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

        public RXDISP_MODE iDispMode = RXDISP_MODE.GRID;
        
        public int iRsCnts
        {
            get
            {
                int idx = Util.FindStringIndex(this.sRsscanner, StringResource.str("lst_rsscanner"));
                if (idx == 1)
                    return 12;
                else if (idx == 2)
                    return 24;
                else
                    return 2;
            }
        }

        public int iRxCnts
        {
            get
            {
                int idx = Util.FindStringIndex(this.sRxscanner,StringResource.str("lst_rxscanner"));
                if (idx == 1)
                    return 12;
                else if (idx == 2)
                    return 24;
                else
                    return 4;
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
        public int iShiftTimes
        {
            get
            {
                return (int)this["iShiftTimes"];
            }
            set
            {
                this["iShiftTimes"] = value;
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
                this["iFilter"] = value;
            }
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
                if(Util.FindStringIndex(value, StringResource.str("lst_navmeter")) >= 0)
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
                if (Util.FindStringIndex(value, StringResource.str("lst_flttype")) >= 0)
                this["sFilterType"] = value;
            }
        }
        public string sRxscanner
        {
            get
            {
                return (string)this["sRxscanner"];
            }
            set
            {
                if (Util.FindStringIndex(value, StringResource.str("lst_rxscanner")) >= 0)
                    this["sRxscanner"] = value;
            }
        }

        public string sRsscanner
        {
            get
            {
                return (string)this["sRsscanner"];
            }
            set
            {
                if (Util.FindStringIndex(value, StringResource.str("lst_rsscanner")) >= 0)
                    this["sRsscanner"] = value;
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
     * for current configuration grp='current'
     */
    internal class SqlConfig
    {
        static private SQLiteConnection sql_con;
        static private SQLiteCommand sql_cmd;

        private SQLiteDataAdapter DB;
        private DataSet DS = new DataSet();
        private DataTable DT = new DataTable();

        private string sql_tbl; //current table name 
        public string sql_grp; //current group name
        static void SetConnection()
        {
            sql_con = new SQLiteConnection("Data Source="+Path.Combine(StringResource.str("basedir"),"Config.db2")+";Version=3;New=False;Compress=True;");
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
              MsgDlg.Show(e.Message);
                return false;
            }
        }
        private bool SaveConfigToFile(string id, string val)
        {
            try
            {
                //save the current configuration 
                SetConnection();
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
                MsgDlg.Show(e.Message);
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
                        return ' ';
                }
                if (id.StartsWith("i")) //int
                {
                    if (val == "")
                        return (int)0;
                    else
                        return Convert.ToInt32(val);
                }
                if (id.StartsWith("d")) //double
                {
                    if (val == "")
                        return (double)0;
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

}
