using System;
using System.Collections.Generic;
using System.Collections;
using System.Linq;
using System.Text;
using Finisar.SQLite;
using System.ComponentModel;
using System.Data;
using System.Xml.Linq;
using System.IO;
using System.Collections.ObjectModel;
namespace QJApp
{
    public enum ResType
    {
        RESISTOR,
        PRT,
        THERMISTOR
    }
    public enum TCoefType
    {
        EMPTY,
        POINT_0,
        POINT_23,
        POINT_25
    }
    public enum RPosition
    {
        mR1,
        mR2,
        mR3,
        mR4,
        mR5,
        mR6,
        mR7,
        mR8,
        mR9,
        mR10,
        mR11,
        mR12,
        mR13,
        mR14,
        mR15,
        mR16,
        mR17,
        mR18,
        mR19,
        mR20,
        mR21,
        mR22,
        mR23,
        mR24,
        mR25,
        mR26,
        mR27,
        mR28,
        mR29,
        mR30,
        mR31,
        mR32,
        mR33,
        mR34,
        mR35,
        mR36,
        mR37,
        mR38,
        mR39,
        mR40,
        sR1,
        sR2,
        sR3,
        sR4,
        sR5,
        sR6,
        sR7,
        sR8,
        sR9,
        sR10,
        sR11,
        sR12,
        sR13,
        sR14,
        sR15,
        sR16,
        sR17,
        sR18,
        sR19,
        sR20,
        sR21,
        sR22,
        sR23,
        sR24,
        sR25,
        sR26,
        sR27,
        sR28,
        sR29,
        sR30,
        sR31,
        sR32,
        sR33,
        sR34,
        sR35,
        sR36,
        sR37,
        sR38,
        sR39,
        sR40
    }
    public enum PRTRange
    {
        NONE,
        H_TO_ZERO,
        NE_TO_ZERO,
        O_TO_ZERO,
        AR_TO_ZERO,
        ZERO_TO_AG,
        ZERO_TO_AL,
        ZERO_TO_ZN,
        ZERO_TO_SN,
        ZERO_TO_IN,
        ZERO_TO_GA,
        HG_TO_GA
    }
    internal class InfoManager<T> : ObservableCollection<T>, IEnumerable
    {
        public Dictionary<string, T> infolist;
        private string _table = "";
        private string[] _groups;
        private string _current = "";
        private Func<string, string, string, T> _func;
        public string Current {
            get{
                return _current;
            }
            set{
                _current = value;
                LoadGroup();
            }
        }
        public InfoManager(string table, Func<string, string, string, T> func)
        {
            _func = func;
            _table = table;
            infolist = new Dictionary<string, T>();
            Init();
        }
        private void Init()
        {
            _groups = SqlConfig.DistinctGroups(_table);
            if (_groups.Length > 0)
            {
                _current = _groups[0];
                LoadGroup();
            }
        }
        private void LoadGroup()
        {
            infolist.Clear();
            
            foreach (string idx in SqlConfig.DistinctIndexs(_table, _current))
            {
                infolist[idx] = _func(_table,_current,idx);
            }
        }
        public void DeleteIndex(string idx)
        {
            if (infolist.Count == 0)
                return;
            if (infolist.ContainsKey(idx))
            {
                SqlConfig.DeleteIndex(_table, _current, idx);
                infolist.Remove(idx);
            }
        }

        public void DeleteGroup()
        {
            if (_groups.Length <= 1)
                return;
            SqlConfig.DeleteGroup(_table, _current);
            Init();
        }
        public T CreateNewItem(string newidx)
        {
            if(!infolist.ContainsKey(newidx))
                infolist[newidx] = _func(_table, _current, newidx);
            return infolist[newidx];
        }
        public bool ContainsKey(string idx)
        {
            return infolist.ContainsKey(idx);
        }
        public T this[string idx]
        {
            get
            {
                if (infolist.ContainsKey(idx))
                    return infolist[idx];
                else
                    return CreateNewItem(idx);
            }
        }
        public new IEnumerator GetEnumerator()
        {
            return infolist.Values.GetEnumerator();
        }
    }
    internal class ConfigInfo
    {
        protected SqlConfig config;
    }

    internal class SysInfo : ConfigInfo, IComparable, INotifyPropertyChanged
    {
        public event PropertyChangedEventHandler PropertyChanged;
        public static SysInfo CreateNew(string table, string grp, string idx)
        {
            return new SysInfo(table, grp, idx);
        }
        private void Notify(string propname)
        {
            if (PropertyChanged != null)
            {
                PropertyChanged.Invoke(this, new PropertyChangedEventArgs(propname));
            }
        }
        public SysInfo(string table, string grp, string idx)
        {
            config = new SqlConfig(table, grp, idx);
        }
        public int CompareTo(object obj)
        {
            if (obj is SysInfo)
            {
                return 0;
            }
            return 1;
        }
        #region master
        public int iMasterAddr  //address of controller
        {
            get
            {
                return (int)this["iMasterAddr"];
            }
            set
            {
                this["iMasterAddr"] = value;
            }
        }
        public string sMasterType  //type of controller
        {
            get
            {
                return (string)this["sMasterType"];
            }
            set
            {
                this["sMasterType"] = value;
            }
        }
        public string sMasterSerial  //type of controller
        {
            get
            {
                return (string)this["sMasterSerial"];
            }
            set
            {
                this["sMasterSerial"] = value;
            }

        }
        #endregion
        #region scanner1
        public int iScanner1Addr  //address of scanner1
        {
            get
            {
                return (int)this["iScanner1Addr"];
            }
            set
            {
                this["iScanner1Addr"] = value;
            }
        }
        public string sScanner1Type  //address of scanner1
        {
            get
            {
                return (string)this["sScanner1Type"];
            }
            set
            {
                this["sScanner1Type"] = value;
            }
        }
        public string sScanner1Serial  //address of scanner1
        {
            get
            {
                return (string)this["sScanner1Serial"];
            }
            set
            {
                this["sScanner1Serial"] = value;
            }
        }
#endregion
        #region scanner2
        public int iScanner2Addr  //address of scanner2
        {
            get
            {
                return (int)this["iScanner2Addr"];
            }
            set
            {
                this["iScanner2Addr"] = value;
            }
        }
        public string sScanner2Type  //address of scanner2
        {
            get
            {
                return (string)this["sScanner2Type"];
            }
            set
            {
                this["sScanner2Type"] = value;
            }
        }
        public string sScanner2Serial  //address of scanner1
        {
            get
            {
                return (string)this["sScanner2Serial"];
            }
            set
            {
                this["sScanner2Serial"] = value;
            }
        }
        #endregion
        #region oven
        public int iOvenAddr  //address of oven
        {
            get
            {
                return (int)this["iOvenAddr"];
            }
            set
            {
                this["iOvenAddr"] = value;
            }
        }
        public string sOvenType  //type of oven
        {
            get
            {
                return (string)this["sOvenType"];
            }
            set
            {
                this["sOvenType"] = value;
            }
        }
        
        public string sOvenSerial  //type of oven
        {
            get
            {
                return (string)this["sOvenSerial"];
            }
            set
            {
                this["sOvenSerial"] = value;
            }
        }
        #endregion
        #region current extension
        public string sCurrentExtType //model of current extension
        {
            get
            {
                return (string)this["sCurrentExtType"];
            }
            set
            {
                this["sCurrentExtType"] = value;
            }
        }

        
        public string sCurrentExtSerial //model of current extension
        {
            get
            {
                return (string)this["sCurrentExtSerial"];
            }
            set
            {
                this["sCurrentExtSerial"] = value;
            }
        }
        public int iCurrentExtAddr  //address of current extention
        {
            get
            {
                return (int)this["iCurrentExtAddr"];
            }
            set
            {
                this["iCurrentExtAddr"] = value;
            }
        }
        #endregion
        public object this[string id]
        {
            get
            {
                return config[id];
            }
            set
            {
                config[id] = value;
                Notify(id);
            }
        }
    }

    internal class RxInfo : ConfigInfo, IComparable
    {
        public static RxInfo CreateNew(string table, string grp, string idx)
        {
            return new RxInfo(table, grp, idx);
        }
        public RxInfo(string table, string grp, string idx)
        {
            config = new SqlConfig(table, grp, idx);
        }
        public int CompareTo(object obj)
        {
            if (obj is RxInfo)
            {
                return 0;
            }
            return 1;
        }
        public double dRxInput  //target rx value
        {
            get
            {
                return (double)this["dRxInput"];
            }
            set
            {
                this["dRxInput"] = value;
            }
        }
        public double dMaxCurrent //maximum current in A
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
        public PRTRange ePRTRange
        {
            get
            {
                return (PRTRange)(this["iPRTRange"]);
            }
            set
            {
                this["iPRTRange"] = (int)value;
            }
        }
        public RPosition eThermPos
        {
            get
            {
                return (RPosition)(this["iThermPos"]);
            }
            set
            {
                this["iThermPos"] = (int)value;
            }
        }
        public RPosition eResPos
        {
            get
            {
                return (RPosition)(this["iResPos"]);
            }
            set
            {
                this["iResPos"] = (int)value;
            }
        }
        public double dResAlpha //alpha for resistor temperature coef
        {
            get
            {
                return (double)this["dResAlpha"];
            }
            set
            {
                this["dResAlpha"] = value;
            }
        }
        public double dResBeta //beta for temperature coef
        {
            get
            {
                return (double)this["dResBeta"];
            }
            set
            {
                this["dResBeta"] = value;
            }
        }

        public double dPRTA //A for PRT 
        {
            get
            {
                return (double)this["dPRTA"];
            }
            set
            {
                this["dPRTA"] = value;
            }
        }
        public double dPRTB //B for PRT
        {
            get
            {
                return (double)this["dPRTB"];
            }
            set
            {
                this["dPRTB"] = value;
            }
        }
        public double dPRTC //C for PRT
        {
            get
            {
                return (double)this["dPRTC"];
            }
            set
            {
                this["dPRTC"] = value;
            }
        }

        public double dThermA //A for Thermistor
        {
            get
            {
                return (double)this["dThermA"];
            }
            set
            {
                this["dThermA"] = value;
            }
        }
        public double dThermB //B for Thermistor
        {
            get
            {
                return (double)this["dThermB"];
            }
            set
            {
                this["dThermB"] = value;
            }
        }
        public double dThermC //C for Thermistor
        {
            get
            {
                return (double)this["dThermC"];
            }
            set
            {
                this["dThermC"] = value;
            }
        }
        public double dUncerntainty //
        {
            get
            {
                return (double)this["dUncerntainty"];
            }
            set
            {
                this["dUncerntainty"] = value;
            }
        }
        public int iUncertainDim //dimension of uncertainty
        {
            get
            {
                return (int)this["iUncertainDim"];
            }
            set
            {
                this["iUncertainDim"] = value;
            }
        }
        public TCoefType eTempCoef
        {
            get
            {
                return (TCoefType)(this["iTempCoef"]);
            }
            set
            {
                this["iTempCoef"] = (int)value;
            }
        }
        public ResType eType
        {
            get
            {
                return (ResType)(this["iType"]);
            }
            set
            {
                this["iType"] = (int)value;
            }
        }
        public string sIdentity
        {
            get
            {
                return (string)this["sIdentity"];
            }
            set
            {
                this["sIdentity"] = value;
            }
        }
        public string sModel
        {
            get
            {
                return (string)this["sModel"];
            }
            set
            {
                this["sModel"] = value;
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

    internal class RsInfo : ConfigInfo, IComparable
    {
        public static RsInfo CreateNew(string table, string grp, string idx)
        {
            return new RsInfo(table, grp, idx);
        }
        public RsInfo(string table,string grp,string idx)
        {
            config = new SqlConfig(table,grp,idx);
        }
        public int CompareTo(object obj)
        {
            if (obj is RsInfo)
            {
                return 0;
            }
            return 1;
        }
        public double dRxInput  //target rx value
        {
            get
            {
                 return (double)this["dRxInput"];
            }
            set
            {
                this["dRxInput"] = value;
            }
        }
        public double dMaxCurrent //maximum current in A
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
        public PRTRange ePRTRange
        {
            get
            {
                return (PRTRange)(this["iPRTRange"]);
            }
            set
            {
                this["iPRTRange"] = (int)value;
            }
        }
        public RPosition eThermPos
        {
            get
            {
                return (RPosition)(this["iThermPos"]);
            }
            set
            {
                this["iThermPos"] = (int)value;
            }
        }
        public RPosition eResPos
        {
            get
            {
                return (RPosition)(this["iResPos"]);
            }
            set
            {
                this["iResPos"] = (int)value;
            }
        }
        public double dResAlpha //alpha for resistor temperature coef
        {
            get
            {
                return (double)this["dResAlpha"];
            }
            set
            {
                this["dResAlpha"] = value;
            }
        }
        public double dResBeta //beta for temperature coef
        {
            get
            {
                return (double)this["dResBeta"];
            }
            set
            {
                this["dResBeta"] = value;
            }
        }

        public double dPRTA //A for PRT 
        {
            get
            {
                return (double)this["dPRTA"];
            }
            set
            {
                this["dPRTA"] = value;
            }
        }
        public double dPRTB //B for PRT
        {
            get
            {
                return (double)this["dPRTB"];
            }
            set
            {
                this["dPRTB"] = value;
            }
        }
        public double dPRTC //C for PRT
        {
            get
            {
                return (double)this["dPRTC"];
            }
            set
            {
                this["dPRTC"] = value;
            }
        }

        public double dThermA //A for Thermistor
        {
            get
            {
                return (double)this["dThermA"];
            }
            set
            {
                this["dThermA"] = value;
            }
        }
        public double dThermB //B for Thermistor
        {
            get
            {
                return (double)this["dThermB"];
            }
            set
            {
                this["dThermB"] = value;
            }
        }
        public double dThermC //C for Thermistor
        {
            get
            {
                return (double)this["dThermC"];
            }
            set
            {
                this["dThermC"] = value;
            }
        }
        public double dUncerntainty //
        {
            get
            {
                return (double)this["dUncerntainty"];
            }
            set
            {
                this["dUncerntainty"] = value;
            }
        }
        public int iUncertainDim //dimension of uncertainty
        {
            get
            {
                return (int)this["iUncertainDim"];
            }
            set
            {
                this["iUncertainDim"] = value;
            }
        }
        public TCoefType eTempCoef
        {
            get
            {
                return (TCoefType)(this["iTempCoef"]);
            }
            set
            {
                this["iTempCoef"] = (int)value;
            }
        }
        public ResType eType
        {
            get
            {
                return (ResType)(this["iType"]);
            }
            set
            {
                this["iType"] = (int)value;
            }
        }
        public string sIdentity
        {
            get
            {
                return (string)this["sIdentity"];
            }
            set
            {
                this["sIdentity"] = value;
            }
        }
        public string sModel
        {
            get
            {
                return (string)this["sModel"];
            }
            set
            {
                this["sModel"] = value;
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

        public DateTime tLastCalibDate //date of last calibration
        {
            get
            {
                return DateTime.Parse((string)this["sLastCalibDate"]);
            }
            set
            {
                this["sLastCalibDate"] = value.ToString();
            }
        }
        public double dCalibValue //value of calibration
        {
            get
            {
                return (double)this["dMonthlyDrift"];
            }
            set
            {
                this["dMonthlyDrift"] = value;
            }
        }

        public double dMonthlyDrift //monthly drift
        {
            get
            {
                return (double)this["dMonthlyDrift"];
            }
            set
            {
                this["dMonthlyDrift"] = value;
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
     * Manage Sqlite based config file
     * All configure table has following structure 
     * id value group table
     * for current configuration id='current' grp='current'
     */
    internal class SqlConfig
    {
        static private SQLiteConnection sql_con;
        static private SQLiteCommand sql_cmd;

        static private DataSet DS = new DataSet();

        private string sql_tbl;
        private string sql_grp;
        private string sql_idx;
        static void SetConnection()
        {
            sql_con = new SQLiteConnection("Data Source=" + Utility.basedir + "\\Config.db;Version=3;New=False;Compress=True;");
        }
        private Dictionary<string, string> curr_conf; //store all the configuration string
        public SqlConfig(string table,string group,string index)
        {
            curr_conf = new Dictionary<string, string>();
            this.sql_tbl = table;
            this.sql_grp = group;
            this.sql_idx = index;
            LoadConfigFromFile();
        }
        public static void DeleteGroup(string table, string group)
        {
            try
            {
                sql_con.Open();
                sql_cmd = new SQLiteCommand();
                sql_cmd.Connection = sql_con;
                sql_cmd.CommandText = "delete from data where tbl='" + table + "' and grp='" + group + "'";
                sql_cmd.ExecuteNonQuery();
                sql_con.Close();
            }
            catch (System.Exception e)
            {
                App.MsgShow(e.Message);
            }
        }
        public static void DeleteIndex(string table, string group, string idx)
        {
            try
            {
                sql_con.Open();
                sql_cmd = new SQLiteCommand();
                sql_cmd.Connection = sql_con;
                sql_cmd.CommandText = "delete from data where tbl='" + table + "' and grp='" + group + "' and idx='"+ idx+ "'";
                sql_cmd.ExecuteNonQuery();
                sql_con.Close();
            }
            catch (System.Exception e)
            {
                App.MsgShow(e.Message);
            }

        }
        public static string[] DistinctIndexs(string table, string group)
        {
            try
            {
                string CommandText;
                SQLiteDataAdapter DB;
                SetConnection();
                sql_con.Open();

                //retrieve current configure
                CommandText = "select distinct idx from data where tbl='" + table + "' and grp='" + group + "'";
                DB = new SQLiteDataAdapter(CommandText, sql_con);
                DS.Reset();
                DB.Fill(DS);
                string[] ret = new string[DS.Tables[0].Rows.Count];
                for (int i = 0; i < ret.Length; i++)
                {
                    ret[i] = DS.Tables[0].Rows[i][0].ToString();
                }
                sql_con.Close();
                return ret;
            }
            catch (System.Exception e)
            {
                App.MsgShow(e.Message);
                return new string[] { "Default" };
            }
        }
        public static string[] DistinctGroups(string table)
        {
            try
            {
                string CommandText;
                SQLiteDataAdapter DB;
                SetConnection();
                sql_con.Open();

                //retrieve current configure
                CommandText = "select distinct grp from data where tbl='" + table + "'";
                DB = new SQLiteDataAdapter(CommandText, sql_con);
                DS.Reset();
                DB.Fill(DS);
                string[] ret = new string[DS.Tables[0].Rows.Count];
                for(int i = 0; i < ret.Length; i++)
                {
                    ret[i] = DS.Tables[0].Rows[i][0].ToString();
                }
                sql_con.Close();
                return ret;
            }
            catch (System.Exception e)
            {
                App.MsgShow(e.Message);
                return new string[]{"Default"};
            }
        }
        private bool LoadConfigFromFile()
        {
            try
            {
                string CommandText;
                SQLiteDataAdapter DB;
                SetConnection();
                sql_con.Open();

                //retrieve current configure
                CommandText = "select id,value from data where grp='"+this.sql_grp+"' and tbl='" + this.sql_tbl + "' and idx='" + this.sql_idx + "'";
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
                App.MsgShow(e.Message);
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
                sql_cmd.CommandText = "replace into data(id,value,grp,tbl,idx) values('" + id + "','" + val + "','" + this.sql_grp + "','" + this.sql_tbl + "','" + this.sql_idx + "')";
                sql_cmd.ExecuteNonQuery();
                sql_con.Close();
                return true;
            }
            catch (System.Exception e)
            {
                App.MsgShow(e.Message);
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
                    int ires;
                    if (val == "" || !Int32.TryParse(val, out ires))
                        return 0;
                    else
                        return Convert.ToInt32(val);
                }
                if (id.StartsWith("d")) //double
                {
                    double dres;
                    if (val == "" || !Double.TryParse(val, out dres))
                        return 0;
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
