using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Text;
using System.IO;
using System.Xml.Linq;
using System.Windows;
using System.ComponentModel;
using System.Threading;
using System.Diagnostics;
using Finisar.SQLite;
namespace ioex_cs
{
    enum NodeStatus
    {
        ST_IDLE,
        ST_LOST,
        ST_BUSY,
        ST_DISABLED
    }

    enum NodeType : byte
    {
        BOARD_TYPE_VIBRATE = 0,
        BOARD_TYPE_WEIGHT = 1,
        //BOARD_TYPE_COLLECT = 2,
        BOARD_TYPE_INVALID = 15
    }
    enum NodeGroup : byte
    {
        BOARD_GROUP_1 = 0,
        BOARD_GROUP_2,
        BOARD_GROUP_3,
        BOARD_GROUP_4
    }

    internal class VibrateNode : SubNode
    {
        public Intf intf_byte;
        public bool OnOff
        {
            get { return true; }
            set{}
        }
        public UInt16 Amp { get; set; }
        public UInt16 Freq { get; set; }
        public UInt16 Time { get; set; }
        public UInt16 PulseWidth { get; set; }
        internal VibrateNode(SPort _port,byte node_addr): base(_port,node_addr)
        {
            intf_byte = new Intf(0);
        }


        /*
         * transform the XML node Element string into data structure
         * Include: Amplitude, Frequency, Time
         *       Handshake, Hasdelay, Hasmem, delay_length, feed_times, fmt_input, fmt_output
         */
        public void FromElement(XElement cfgNode)
        {
            Amp = UInt16.Parse(cfgNode.Element("Amplitude").Value);
            Freq = UInt16.Parse(cfgNode.Element("Frequency").Value);
            Time = UInt16.Parse(cfgNode.Element("Time").Value);

            PulseWidth = UInt16.Parse(cfgNode.Element("PulseWidth").Value);
            intf_byte.b_Handshake = bool.Parse(cfgNode.Element("Handshake").Value);
            intf_byte.b_Hasdelay = bool.Parse(cfgNode.Element("Hasdelay").Value);
            intf_byte.b_Hasmem = bool.Parse(cfgNode.Element("Hasmem").Value);
            intf_byte.delay_length = UInt16.Parse(cfgNode.Element("delay_length").Value);
            intf_byte.feed_times = UInt16.Parse(cfgNode.Element("feed_times").Value);
            intf_byte.fmt_input = UInt16.Parse(cfgNode.Element("fmt_input").Value);
            intf_byte.fmt_output = UInt16.Parse(cfgNode.Element("fmt_output").Value);
        }
        /*
         * transform data structure into XML node Element
         * Include: Amplitude, Frequency,
         *      Handshake, Hasdelay, Hasmem, delay_length, feed_times, fmt_input, fmt_output
         */
        public void ToElement(XElement cfgNode)
        {
            cfgNode.Add(new XElement("Amplitude", Amp.ToString()));
            cfgNode.Add(new XElement("Frequency", Freq.ToString()));
            cfgNode.Add(new XElement("Time", Time.ToString()));
            cfgNode.Add(new XElement("Handshake", intf_byte.b_Handshake.ToString()));
            cfgNode.Add(new XElement("Hasdelay", intf_byte.b_Hasdelay.ToString()));
            cfgNode.Add(new XElement("Hasmem", intf_byte.b_Hasmem.ToString()));
            cfgNode.Add(new XElement("delay_length", intf_byte.delay_length.ToString()));
            cfgNode.Add(new XElement("feed_times", intf_byte.feed_times.ToString()));
            cfgNode.Add(new XElement("fmt_input", intf_byte.fmt_input.ToString()));
            cfgNode.Add(new XElement("fmt_output", intf_byte.fmt_output.ToString()));
            cfgNode.Add(new XElement("PulseWidth", PulseWidth.ToString()));
        }
    }
    internal class Intf
    {
        public UInt16 buf
        {
            get;
            set;
        }
        public Intf(UInt16 ini_intf)
        {
            buf = ini_intf;
        }
        private void clrbits(byte start, byte end) //set several bits based on bit position start from 0
        {
            UInt16 i = getbits(start, end);
            while(end-- > 0)
            {
                i = (UInt16) (i*2);
            }
            buf = (UInt16)(buf - i);
        }
        private UInt16 getbits(byte start, byte end) //get several bits based on bit position start from 0
        {
            UInt16 i = (UInt16)1;
            UInt16 j = (UInt16)1;
            while (start-- > end)
            {
                j = (UInt16)(j * 2);
            }
            while (end-- > 0)
            {
                i = (UInt16)(i * 2);
            }
            i = (UInt16)(buf / i);
            return (UInt16)( i - (UInt16)((UInt16)( i / j) * j));
        }
        public bool b_Handshake
        {
            get
            {
                return (getbits(8, 7) == 1);
            }
            set
            {
                clrbits(8, 7);
                if (value)
                {
                    buf += 128;
                }
            }
        }
        public bool b_Hasmem
        {
            get
            {
                return (getbits(2, 1) == 1);
            }
            set
            {
                clrbits(2, 1);
                if (value)
                {
                    buf += 2;
                }
            }
        }
        public bool b_Hasdelay
        {
            get
            {
                return (getbits(3, 2) == 1);
            }
            set
            {
                clrbits(3, 2);
                if (value)
                {
                    buf += 4;
                }
            }
        }
        public UInt16 fmt_input
        {
            get { return getbits(7, 5); }
            set
            {
                if (value < 4)
                {
                    clrbits(7, 5);
                    buf += (UInt16)(value * 32);
                }
            }
        }
        public UInt16 fmt_output
        {
            get { return getbits(5, 3); }
            set
            {
                if (value < 4)
                {
                    clrbits(5, 3);
                    buf += (UInt16)(value * 8);
                }
            }
        }
        public UInt16 feed_times
        {
            get { return getbits(11, 8); }
            set
            {
                if (value < 8)
                {
                    clrbits(11, 8);
                    buf += (UInt16)(value * 256);
                }
            }
        }
        public UInt16 delay_length
        {
            get { return getbits(16, 11); }
            set
            {
                if (value < 32)
                {
                    clrbits(16, 11);
                    buf += (UInt16)(value*2048);
                }
            }
        }
    }
        
    internal class WeighNode : SubNode
    {
        private double _lastweight;
        public bool bRelease;
        public int cnt_match;
        public const double INAVLID_WEIGHT =    1000000.0;
        public const double NOREADING_WEIGHT = -10000.0;
        public const UInt32 MAX_VALID_WEIGHT = 65520;
        
        private int cnt_aderr; //count for ad error
        Random rand;
        public double weight
        {
            get{
                try
                {
                    if (NodeAgent.IsDebug)
                        return rand.NextDouble() * 20;
                    if ((!this["mtrl_weight_gram"].HasValue) || (!this["mtrl_weight_gram"].HasValue))
                    {
                        return WeighNode.NOREADING_WEIGHT;
                    }
                    UInt32 value = this["mtrl_weight_gram"].Value;
                    if (value > WeighNode.MAX_VALID_WEIGHT) //special message
                    {
                        if (value == 65531) //filtering
                        {
                            //Debug.Write("f"+this.node_id);
                            return WeighNode.INAVLID_WEIGHT + 1;
                        }
                        if (value == 65534) //invalid weight
                        {
                            //Debug.Write("a" + this.node_id);
                            return WeighNode.INAVLID_WEIGHT + 2;
                        }
                        //try 10 times before an AD error is issued.
                        if (cnt_aderr < 10)
                        {
                            cnt_aderr++;

                            return WeighNode.INAVLID_WEIGHT + 3;
                        }
                        if (value == 65529) //overweight
                        {
                            if (errlist.IndexOf("err_ow1;") < 0)
                                errlist = errlist + "err_ow1;";

                        }
                        if (value == 65532) //AD overflow
                        {
                            if (errlist.IndexOf("err_ad;") < 0)
                                errlist = errlist + "err_ad;";
                        }
                        if (value == 65530) //divide zero
                        {
                            //                      return WeighNode.INAVLID_WEIGHT+4;
                            if (errlist.IndexOf("err_dz;") < 0)
                                errlist = errlist + "err_dz;";
                        }
                        _lastweight = value;
                        return _lastweight;
                    }
                    cnt_aderr = 0;
                    if (errlist != "")  //clear all the error message
                    {
                        errlist = errlist.Replace("65529;", "");
                        errlist = errlist.Replace("65532;", "");
                        errlist = errlist.Replace("65530;", "");
                        errlist = errlist.Replace("err_ow1;", "");
                        errlist = errlist.Replace("err_ad;", "");
                        errlist = errlist.Replace("err_dz;", "");

                    }
                    bRelease = false;
                    double w = (double)(this["mtrl_weight_gram"].Value) + (double)this["mtrl_weight_decimal"].Value / (double)64.0;
                    //                if (_lastweight != w)
                    //                    StringResource.dolog(w.ToString("F2") + "#" + this.node_id);

                    //                if (Math.Abs(_lastweight-w) > 0.1)
                    //                {
                    _lastweight = w;
                    //                }
                    return _lastweight;
                }
                catch
                {
                    return WeighNode.NOREADING_WEIGHT;
                }
            }
        }
        public void ClearWeight()
        {
            curr_conf["mtrl_weight_gram"] = null;
            _lastweight = INAVLID_WEIGHT;
        }
        
        public void Query()
        {
            if (NodeAgent.IsDebug)
                return;
            //read_regs(new string[] { "mtrl_weight_gram", "mtrl_weight_decimal", "status1","status2" });
            read_regs(new string[] { "mtrl_weight_gram", "mtrl_weight_decimal" });
            if (!NodeAgent.IsDebug)
                status = NodeStatus.ST_BUSY;
        }
        

        internal WeighNode(SPort _port, byte node_addr)
            : base(_port,node_addr)
        {
            _lastweight = -1;
            rand = new Random();
            cnt_match = 0;
            cnt_aderr = 0;
        }
    }
    /*
     * Manage Sqlite based config file
     * All configure table has following structure 
     * id value group 
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
        static void SetConnection()
        {
             sql_con = new SQLiteConnection("Data Source=Config.db;Version=3;New=False;Compress=True;");;
        }
        private Dictionary<string, XElement> curr_conf; //store all the configuration string
        public string cfg_name; //store the config name of current selection
        private string[] regs;
        public SqlConfig(string sql_tbl)
        {
            curr_conf = new Dictionary<string, XElement>();
            this.sql_tbl = sql_tbl;
        }
        
        public bool LoadConfigFromFile()
        {
            try
            {
                string CommandText;
                SetConnection();
                sql_con.Open();

                //retrieve all the regs
                CommandText = "select distinct id from data where tbl='" + this.sql_tbl + "'";
                DB = new SQLiteDataAdapter(CommandText, sql_con);
                DS.Reset();
                DB.Fill(DS);
                regs = new string[DS.Tables[0].Rows.Count];
                int i = 0;
                foreach (DataRow dr in DS.Tables[0].Rows)
                {
                    if ((dr[0].ToString() != "current") && (dr[0].ToString() != "null"))
                        regs[i++] = dr[0].ToString();
                }
                //retrieve all the names
                CommandText = "select distinct grp from data where tbl='" + this.sql_tbl+"'";
                DB = new SQLiteDataAdapter(CommandText, sql_con);
                DS.Reset();
                DB.Fill(DS);
                XElement xe = new XElement("Item");
                foreach (DataRow dr in DS.Tables[0].Rows)
                {
                    if (dr[0].ToString() != "current")
                        curr_conf[dr[0].ToString()] = LoadDBConfig(dr[0].ToString());
                }

                
                //retrieve current configure
                CommandText = "select value from data where grp='current' and tbl='"+this.sql_tbl+"'";
                DB = new SQLiteDataAdapter(CommandText, sql_con);
                DS.Reset();
                DB.Fill(DS);
                if (DS.Tables[0].Rows.Count > 0)
                    cfg_name = DS.Tables[0].Rows[0][0].ToString();
                else
                    cfg_name = "default";

                sql_con.Close();
                

                return true;
            }
            catch (System.Exception e)
            {
                MessageBox.Show(e.Message);
                return false;
            }
        }
        public bool SaveConfigToFile()
        {
            try{
            //save the current configuration 
            //SetConnection();
            sql_con.Open();
            
            sql_cmd = new SQLiteCommand();
            sql_cmd.Connection = sql_con;
            sql_cmd.CommandText = "replace into data(id,value,grp,tbl) values('current','" + cfg_name + "','current','"+this.sql_tbl+"')";
            sql_cmd.ExecuteNonQuery();
            sql_con.Close();
            return true;
            }
            catch (System.Exception e)
            {
                MessageBox.Show(e.Message);
                return false;
            }
        }
        public void AddConfig(string cfgname, XElement e)
        {
            //SetConnection();
            sql_con.Open();
            sql_cmd = new SQLiteCommand();
            sql_cmd.Connection = sql_con;
            foreach (string reg in regs)
            {
                if (reg == null)
                    continue;
                sql_cmd.CommandText = "replace into data(id,value,grp,tbl) values('" + reg + "','" + e.Element(reg).Value + "','" + cfgname + "','" + this.sql_tbl + "')";
                sql_cmd.ExecuteNonQuery();
            }
            sql_con.Close();
            curr_conf[cfgname] = e;
            return;
        }
        public XElement LoadConfig(string newcfg)
        {
            if (curr_conf.ContainsKey(newcfg))
            {
                if (cfg_name != newcfg)
                {
                    cfg_name = newcfg;
                    SaveConfigToFile();
                }
                
                return curr_conf[newcfg];
            }
            return null;
        }
        private XElement LoadDBConfig(string newcfg)
        {
//            SetConnection();
//            sql_con.Open();
            //retrieve all the names
            string CommandText = "select id,value from data where grp='" + newcfg + "' and tbl='" + this.sql_tbl + "'";
            SQLiteDataAdapter DB2 = new SQLiteDataAdapter(CommandText, sql_con);
            DataSet DS2 = new DataSet();
            DS2.Reset();
            DB2.Fill(DS2);
            XElement xe = new XElement("Item");
            foreach (string reg in regs)
            {
                xe.Add(reg, "0");
            }
            foreach (DataRow dr in DS2.Tables[0].Rows)
            {
                if (regs.Contains(dr[0].ToString()))
                    xe.SetElementValue(dr[0].ToString(), dr[1].ToString());
            }
            //sql_con.Close();
            return xe;           
        }
        public void RemoveConfig(string oldcfg)
        {
            //SetConnection();
            sql_con.Open();
            sql_cmd = new SQLiteCommand("delete from data where grp='"+ oldcfg +"' and tbl='" + this.sql_tbl+ "'");
            sql_cmd.Connection = sql_con;
            sql_cmd.ExecuteNonQuery();
            sql_con.Close();
            curr_conf.Remove(oldcfg);
        }
        public XElement Current
        {
            get
            {
                return LoadConfig(cfg_name);
            }
        }
        public IEnumerable<String> Keys
        {
            get
            {
                return curr_conf.Keys;
            }
        }
        public XElement this[string val]
        {
            get
            {
                return curr_conf[val];
            }
        }
    }
    /*
     * Manage XML file based config file, see app_config.xml for details
     * <Item Name="Id">1
     *  <value>xxx</value>
     * </Item>
     */
    internal class XmlConfig
    {
        private Dictionary<string, string> curr_conf; //store all the configuration string
        
        public string cfg_name; //store the config name of current selection
        public XmlConfig(string xml_file)
        {
            curr_conf = new Dictionary<string, string>();
            this.xml_file = xml_file;
        }
        private string xml_file;
        
        public bool LoadConfigFromFile()
        {
            try
            {
                XDocument xml_doc;
                xml_doc = XDocument.Load(xml_file);
                cfg_name = xml_doc.Element("Root").Attribute("current").Value;
                IEnumerable<XElement> elem = xml_doc.Descendants("Item");
                foreach (XElement e in elem)
                {
                    curr_conf[e.Attribute("Name").Value] = e.ToString();
                }

                return true;
            }
            catch (System.Exception e)
            {
                MessageBox.Show(e.Message);
                return false;
            }
        }
        public bool SaveConfigToFile()
        {

            try
            {
                if (cfg_name == null)
                {
                    cfg_name = "001";
                }
                XElement all = new XElement("Root", new XAttribute("current", cfg_name));
                foreach (string cfg in curr_conf.Keys)
                {
                    XElement ecfg = XElement.Parse(curr_conf[cfg]);
                    ecfg.ReplaceAttributes(new XAttribute("Name", cfg));
                    all.Add(ecfg);
                }
                XDocument newdoc = new XDocument(
                    new XDeclaration("1.0", "utf-8", "yes"),
                    all);
                newdoc.Save(xml_file);
                return true;
            }
            catch (System.Exception e)
            {
                MessageBox.Show(e.Message);
                return false;
            }
            
        }
        public void AddConfig(string cfgname,XElement e)
        {
            curr_conf[cfgname] = e.ToString();
        }
        public XElement LoadConfig(string newcfg)
        {
            if (curr_conf.ContainsKey(newcfg))
            {
                cfg_name = newcfg;
                return XElement.Parse(curr_conf[newcfg]);
            }
            return null;
        }
        public XElement RemoveConfig(string oldcfg)
        {
            if (curr_conf.Count > 1 && curr_conf.ContainsKey(oldcfg))
            {
                curr_conf.Remove(oldcfg);
                cfg_name = curr_conf.Keys.First<string>();
                return LoadConfig(cfg_name);
            }
            return null;
        }
        public XElement Current {
            get
            {
                return LoadConfig(cfg_name);
            }
        }
        public IEnumerable<String> Keys {
            get{
                return curr_conf.Keys;
            }
        }
        public string this[string val]{
            get
            {
                return curr_conf[val];
            }
        }
    }
    
    internal struct RegType
    {
        public byte pos;    //start position of the register
        public byte size;   //size of the register
        public char rw;     //read/write property of the register, 'r','w','b'="rw","v"=volatile
        public char update; //indicate whether download/save at startup 
                     //'s': setting , 'i': initial setting, 'p': report 'a': action
    }

    internal abstract class SubNode : Object
    {

        public static Dictionary<string, RegType> reg_type_tbl; //table to map the reg name to details, should be loaded from xml file setting
        
        private static Dictionary<byte, string> reg_pos2name_tbl; //format, <2,baudrate> where 1 the multiplier, table to map the reg position to name, should be loaded from xml file
        private static Dictionary<byte, UInt32> reg_mulitply_tbl; //multiplier of each register
        private Dictionary<string, bool> valfresh; //indicate whether value is fresh updated.
        protected Dictionary<string, Nullable<UInt32>> curr_conf; //store the configuration string of each config name (<config_name, xml_value> pair>

        public UInt32 flag_cnt = 0;
        public double percent = 0.0;
        static string reg_define_file = "ioex_cs.Resources.node_define.xml";// /ioex-cs;component/Resources/
        public byte node_id { get; set; }
        public NodeStatus status { get; set; }

        public string errlist { get; set; } //list of errors of the current node
        private SPort port;  //serial port
        public SPort serialport{
            set{
                port = value;
            }
            get
            {
                return port;
            }
        }
        static public byte[] last_cmd;
        static SubNode()
        {
            last_cmd = null;
            
            reg_mulitply_tbl = new Dictionary<byte, uint>();
            reg_type_tbl = new Dictionary<string, RegType>();
            reg_pos2name_tbl = new Dictionary<byte, string>();
            
            //load setting from xml file
            XDocument xml_doc;

            StreamReader sr = new StreamReader(System.Reflection.Assembly.GetExecutingAssembly().GetManifestResourceStream(reg_define_file));
            xml_doc = XDocument.Load(sr);
            
            IEnumerable<XElement> regs = xml_doc.Descendants("reg");
            foreach (XElement reg in regs)
            {
                byte size = byte.Parse(reg.Element("size").Value);
                byte offset = byte.Parse(reg.Element("offset").Value);
                char rw = char.Parse(reg.Element("rw").Value);
                char update = char.Parse(reg.Element("update").Value);
                string name = reg.Element("name").Value;
                RegType rt;
                rt.pos = offset;
                rt.size = size;
                rt.rw = rw;
                rt.update = update;
                reg_type_tbl[name] = rt;
                
                if ( size == 8)
                {
                    reg_pos2name_tbl[offset] = name;
                    reg_mulitply_tbl[offset] = 1;
                }
                if (size == 16)
                {
                    
                    reg_pos2name_tbl[offset] = name;
                    reg_mulitply_tbl[offset] = 1;
                    reg_pos2name_tbl[(byte)(offset+1)] = name;
                    reg_mulitply_tbl[(byte)(offset+1)] = 256;
                }
                if (size == 32)
                {
                    reg_pos2name_tbl[offset] = name;
                    reg_mulitply_tbl[offset] = 1;
                    reg_pos2name_tbl[(byte)(offset + 1)] = name;
                    reg_mulitply_tbl[(byte)(offset + 1)] = 256 ;
                    reg_pos2name_tbl[(byte)(offset + 2)] = name;
                    reg_mulitply_tbl[(byte)(offset + 2)] = 256*256;
                    reg_pos2name_tbl[(byte)(offset + 3)] = name;
                    reg_mulitply_tbl[(byte)(offset + 3)] =256*256*2561;
                }
            }
            
            return;
        }
        internal SubNode(SPort _port, byte node_id)
        {
            port = _port;
            this.node_id = node_id;
            errlist = "";
            
            //to initial reg_table items from xml table
            curr_conf = new Dictionary<string, Nullable<UInt32>>();
            valfresh = new Dictionary<string, bool>();
           
            ICollection<string> names = reg_type_tbl.Keys;
            foreach (string name in names)
            {
                curr_conf[name] = null;
                valfresh[name] = false;
            }
            port.InFrameHandlers[node_id] = HandleInFrame;
            
            
            
            status = NodeStatus.ST_IDLE;
                
        }
        //write to group address
        public void writebyte_group_reg(byte group_addr, string[] regaddr, byte[] value)
        {
            FrameBuffer ofrm = new FrameBuffer();
            ofrm.cmd = (byte)'W';
            ofrm.addr = group_addr;
            byte[] regbuffer = new byte[16];
            byte[] valbuffer = new byte[16];
            byte[] outbuffer = new byte[32];
            int outptr = 0;
            for (int i = 0; i < regaddr.GetLength(0); i++)
            {
                regbuffer[outptr] = reg_type_tbl[regaddr[i]].pos;
                valbuffer[outptr] = (byte)value[i];
                outptr++;
            }
            ofrm.generate_write_frame(outbuffer, regbuffer, valbuffer, 0, (byte)(outptr));
            port.AddVCommand(outbuffer);
        }

        //write to address with S mode
        public void writeseq_abs_reg(byte regaddr, byte[] value, int len)
        {
            byte[] buf= new byte[200];
            buf[1] = 0xfe;
            buf[2] = 0x68;
            buf[3] = 0x00;
            buf[4] = node_id;
            buf[5] = (byte)'S';
            buf[6] = (byte)(len+1);
            buf[7] = regaddr;

            UInt32 sum = (UInt32)(0xfe) + (UInt32)(0x68) + (UInt32)(0x00) + (UInt32)node_id +  + (UInt32)buf[5] + (UInt32)buf[6];
            sum = sum + (UInt32)buf[7];

            for (int i = 0; i < len; i++)
            {
                buf[8 + i] = value[i];
                sum = sum + value[i];
            }
            sum = 256 - (sum % 256);

            buf[8 + len] = (byte)sum;
            buf[0] = (byte)(8 + len); //total frame length
            port.AddVCommand(buf);

        }
        public void writebyte_abs_reg(string[] regaddr, byte[] value)
        {
            FrameBuffer ofrm = new FrameBuffer();
            ofrm.cmd = (byte)'W';
            ofrm.addr = this.node_id;
            byte[] regbuffer = new byte[16];
            byte[] valbuffer = new byte[16];
            byte[] outbuffer = new byte[32];
            int outptr = 0;
            for (int i = 0; i < regaddr.GetLength(0); i++)
            {
                regbuffer[outptr] = reg_type_tbl[regaddr[i]].pos;
                valbuffer[outptr] = (byte)value[i];
                outptr++;
            }
            ofrm.generate_write_frame(outbuffer, regbuffer, valbuffer, 0, (byte)(outptr));
            port.AddVCommand(outbuffer);
        }

        //write to register with absolute address, for bootloader use
        public void writebyte_abs_reg(byte[] regaddr, byte[] value)
        {
            FrameBuffer ofrm = new FrameBuffer();
            ofrm.cmd = (byte)'W';
            ofrm.addr = this.node_id;
            byte[] regbuffer = new byte[16];
            byte[] valbuffer = new byte[16];
            byte[] outbuffer = new byte[32];
            int outptr = 0;
            for (int i = 0; i < regaddr.GetLength(0); i++)
            {
                 regbuffer[outptr] = regaddr[i];
                 valbuffer[outptr] = (byte)value[i];
                 outptr++;
            }
            ofrm.generate_write_frame(outbuffer, regbuffer, valbuffer, 0, (byte)(outptr));
            port.AddVCommand(outbuffer);
        }
        
        private void write_vregs(string[] names, UInt32[] values)
        {
            if (NodeAgent.IsDebug)
            {
                for (int i = 0;i< names.Length;i++)
                {
                    if(curr_conf.ContainsKey(names[i]))
                        curr_conf[names[i]] = values[i];
                }
                return;
            }

            FrameBuffer ofrm = new FrameBuffer();
            ofrm.cmd = (byte)'W';
            if (!curr_conf["addr"].HasValue)
            {
                throw new Exception("Invalid node address");
            }
            ofrm.addr = (byte)curr_conf["addr"].Value;
            byte[] regbuffer = new byte[16];
            byte[] valbuffer = new byte[16];
            byte[] outbuffer = new byte[32];
            int outptr = 0;
            for (int i = 0; i < names.GetLength(0); i++)
            {
                if (reg_type_tbl[names[i]].size == 8)
                {
                    regbuffer[outptr] = reg_type_tbl[names[i]].pos;
                    valbuffer[outptr] = (byte)values[i];
                    outptr++;
                }
                if (reg_type_tbl[names[i]].size == 16)
                {
                    valbuffer[outptr ] = (byte)(values[i] % 256);
                    valbuffer[outptr + 1] = (byte)(values[i] / 256);
                    regbuffer[outptr] = reg_type_tbl[names[i]].pos;
                    regbuffer[outptr + 1] = (byte)(regbuffer[outptr] + 1);
                    outptr = outptr + 2;
                }
                if (reg_type_tbl[names[i]].size == 32)
                {
                    valbuffer[outptr + 0] = (byte)(values[i] % 256);
                    values[i] = (UInt32)(values[i] / 256);
                    valbuffer[outptr + 1] = (byte)(values[i] % 256);
                    values[i] = (UInt32)(values[i] / 256);
                    valbuffer[outptr + 2] = (byte)(values[i] % 256);
                    values[i] = (UInt32)(values[i] / 256);
                    valbuffer[outptr + 3] = (byte)(values[i] % 256);
                    regbuffer[outptr] = reg_type_tbl[names[i]].pos;
                    regbuffer[outptr + 1] = (byte)(regbuffer[outptr] + 1);
                    regbuffer[outptr + 2] = (byte)(regbuffer[outptr] + 2);
                    regbuffer[outptr + 3] = (byte)(regbuffer[outptr] + 3);

                    outptr = outptr + 4;
                }
            }
            ofrm.generate_write_frame(outbuffer, regbuffer, valbuffer, 0, (byte)(outptr));
            port.AddVCommand(outbuffer);
        }

        public Nullable<UInt32> this[byte reg_id]    //get/set the reg_value
        {
            get { 
                return this[reg_pos2name_tbl[reg_id]]; 
            }
            set {
                this[reg_pos2name_tbl[reg_id]] = value; 
            }
        }
        public Nullable<UInt32> this[string reg_name]   //get/set the reg_value
        {
            get {
                if (NodeAgent.IsDebug)
                {
                    if (reg_name == "addr")
                    {
                        return node_id;
                    }
                    if (curr_conf[reg_name].HasValue)
                        return curr_conf[reg_name];
                    else
                        return 0;
                }
                valfresh[reg_name] = false;
                return curr_conf[reg_name]; 
            }
            set {
                if (NodeAgent.IsDebug)
                {
                    if (curr_conf.ContainsKey(reg_name) && value != null)
                        curr_conf[reg_name] = value;
                    return;
                }
                if (port.Status == PortStatus.CLOSED)
                    throw new Exception("port is closed");
                if (status == NodeStatus.ST_DISABLED)
                    return;
                if (status == NodeStatus.ST_LOST)
                    return; //todo try the action at random level
//                if (status == NodeStatus.ST_BUSY)
//                    throw new Exception("reenter when busy");

                {
                    status = NodeStatus.ST_BUSY;
                    if (value.HasValue)
                    {
                       
                        if (reg_type_tbl[reg_name].rw == 'v') //volatile value
                        {
                                write_vregs(new string[] { reg_name }, new UInt32[] { value.Value });
                        }
                        if (reg_type_tbl[reg_name].rw == 'b') //r+w
                        {
                                write_vregs(new string[] { reg_name }, new UInt32[] { value.Value });
                                curr_conf[reg_name] = null;
                                Thread.Sleep(2);
                                read_regs(new string[] { reg_name });
                                return;
                        }
                        status = NodeStatus.ST_IDLE;    //no response or action required r (read only), v mode
                        return;

                    }
                    if(valfresh[reg_name])
                    {
                        status = NodeStatus.ST_IDLE;
                        return;
                    }
                    curr_conf[reg_name] = null;
                    read_regs(new string[] { reg_name });
                }
            }
        }
        /*
         * Generate read frame and add to buffer
         */
        protected void read_regs(string[] names)
        {
            FrameBuffer ofrm = new FrameBuffer();
            ofrm.cmd = (byte)'R';
            ofrm.addr = node_id;
            byte[] regbuffer = new byte[16];
            byte[] outbuffer = new byte[16];
            int outptr = 0;
            for (int i = 0; i < names.GetLength(0); i++)
            {
                if (reg_type_tbl[names[i]].size == 8)
                {
                    regbuffer[outptr] = reg_type_tbl[names[i]].pos;
                    outptr++;
                }
                if (reg_type_tbl[names[i]].size == 16)
                {
                    regbuffer[outptr] = reg_type_tbl[names[i]].pos;
                    regbuffer[outptr + 1] = (byte)(regbuffer[outptr] + 1);
                    outptr = outptr + 2;
                }
                if (reg_type_tbl[names[i]].size == 32)
                {
                    regbuffer[outptr] = reg_type_tbl[names[i]].pos;
                    regbuffer[outptr + 1] = (byte)(regbuffer[outptr] + 1);
                    regbuffer[outptr + 2] = (byte)(regbuffer[outptr] + 2);
                    regbuffer[outptr + 3] = (byte)(regbuffer[outptr] + 3);

                    outptr = outptr + 4;
                }
            }
            ofrm.generate_read_frame(outbuffer, regbuffer, 0, (byte)(outptr));
            port.AddVCommand(outbuffer);
            last_cmd = outbuffer;
        }
        public void ResendCommand()
        {
            port.ResetInFlag();
            port.AddVCommand(last_cmd);
        }
        
        /*
         * Status will be set to IDLE after valid frame is processed.
         */
        private void HandleInFrame(FrameBuffer ifrm)
        {
            UInt32 value;
            if (ifrm.cmd == 'W') //write by bytes
            {
                for (int j = 0; j < ifrm.datalen; j=j+2 )
                {
                    try
                    {
                        /*
                         * just replace the specified 8 bits in register
                         */
                        string name = reg_pos2name_tbl[ifrm.data[j]];
                        value = 0; 
                        if (curr_conf[name].HasValue)
                        {
                            value = curr_conf[name].Value;
                        }
                        UInt32 multiplier = reg_mulitply_tbl[ifrm.data[j]];
                        valfresh[name] = true;
                        curr_conf[name] = 256 * multiplier * (UInt32)(value / (256 * multiplier)) + multiplier * ifrm.data[j + 1] + (value % multiplier);
                    }
                    catch (System.Exception e)
                    {
                       MessageBox.Show(e.Message);                 	
                    }
                }
                status = NodeStatus.ST_IDLE;
            }
            
#region obsolete_readword_code
            /*
            if (ifrm.cmd == 'X')
            {
                for (int j = 0; j < ifrm.framelen;j=j+3 )
                {
                    try
                    {
                        //queryEvent(ifrm.data[j], (UInt32)(ifrm.data[j + 1]) * 256 + (UInt32)(ifrm.data[j + 2]));
                    }
                    catch (System.Exception e)
                    {
                        MessageBox.Show(e.Message);                 	
                    }
                }
            }
            */

#endregion
        }
        /*
         * The function wait until the regname data equal to value.
         * It is only used for long time operation such as EEPROM programming
         */
        protected void WaitUntilGetValue(string regname, UInt32 value)
        {
            do 
            {
                Thread.Sleep(5);
           
                if ((this[regname].HasValue && this[regname].Value == value))
                    return;

                this[regname] = null;
            } while (true);
        }
        /*
         * Load all the reg value from the board
         */
        public void init_load_regs()  //load the initial reg value
        {
            foreach (string regname in reg_type_tbl.Keys)
            {
                RegType reg = reg_type_tbl[regname];
                if (reg.update == 's')
                {
                    this[regname] = null;
                }
            }
        }

    }
}
