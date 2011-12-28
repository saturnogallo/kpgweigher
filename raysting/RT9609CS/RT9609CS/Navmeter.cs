using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using System.IO.Ports;
using System.Threading;
using System.Windows.Forms;
using System.Collections;
namespace RT9609CS
{
    class Navmeter
    {
        private Config cmdset;
        private SerialPort _sport;
        private StringBuilder spbuf;
        private Dictionary<string, string> vars;
        public Navmeter(SerialPort sp)
        {
            cmdset = new Config("./command.xml");
            
            vars = new Dictionary<string,string>();
            spbuf = new StringBuilder();
            _sport = sp;
            _sport.DataReceived += new SerialDataReceivedEventHandler(_sport_DataReceived);
        }
        public string this[string val]
        {
            get
            {
                if (vars.ContainsKey(val))
                    return val;
                else
                    return "";
            }
        }
        void _sport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            spbuf.Append(_sport.ReadExisting().ToCharArray());
            string line = spbuf.ToString();
            if (line.IndexOfAny(new char[] { '\r', ';' }) > 0)
            {
                try{
                    reading = Double.Parse(line.Trim(new char[] { '\r', ';' }));
                }catch{
                }
            }
        }
        public void Send(string cmd_id)
        {
            _sport.DiscardInBuffer();
            _sport.DiscardOutBuffer();
            spbuf.Remove(0, spbuf.Length);

            object o = cmdset.GetField("Navmeter", cmd_id, "sSTR");
            string ret;
            if (o is string)
                ret = o as string;
            else
                return;
            o = cmdset.GetField("Navmeter", cmd_id, "uPAUSE");
            UInt32 delay;
            if (o is UInt32)
                delay = (UInt32)o;
            else
                return;
            o = cmdset.GetField("Navmeter", cmd_id, "sVAR");
            string var;
            if (o is string)
                var = o as string;
            else
                return;
            vars[var] = cmd_id;
            ret = "Un" + ret.Replace("U", "UU").Replace("\\n", "\n").Replace("\\r","\r");
            byte[] b = new byte[1];
            foreach (char c in ret.ToCharArray())
            {
                if (c == '^')
                {
                    Thread.Sleep(10);
                }
                else
                {
                    b[0] = (Byte)c;
                    _sport.Write(b, 0, 1);
                    Thread.Sleep(1);
                }
            }
            Thread.Sleep(100);
        }
        private double reading;
        const double INVALID_DATA = -9999;
        public double Read()
        {
            reading = INVALID_DATA - 1;
            
            while (true)
            {
                int i = 0;
                while (i < 100) //10 seconds at most
                {
                    if (reading > INVALID_DATA)
                        break;
                    Thread.Sleep(100);
                    
                    if ((i % 30) == 0)
                        Send("READ");
                    i++;
                }
                if (i >= 100)
                    MessageBox.Show("请检查纳伏计连接");
                else
                    return reading;
            }
        }
    }
    class Scanner
    {
        private SerialPort _sport;
        public Scanner(SerialPort sp)
        {
            _sport = sp;
            _sport.DataReceived += new SerialDataReceivedEventHandler(_sport_DataReceived);
        }

        public void ToChannel(int ch)
        {
            _sport.Write(ch.ToString() + "A\r\n");
            Thread.Sleep(100);
        }
        void _sport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            _sport.DiscardInBuffer();
            
        }
    }
    class Switch
    {
        public bool bRx { get; set; }
        public bool bRs { get; set; }
        public bool bOv1 { get; set; }
        public bool bOv2 { get; set; }
        private Queue<Byte> spbuf;
        private Config cmdset;
        private SerialPort _sport;
        private Dictionary<string, bool> swis; //true means  on(closed)
        private byte pos(string id, bool on)
        {
            if (id == "KI1") { if (!on) return 0x21; else return 0x28; }
            if (id == "KI2") { if (!on) return 0x32; else return 0x3b; }
            if (id == "KI3") { if (!on) return 0x3a; else return 0x33; }
            if (id == "KI4") { if (!on) return 0x02; else return 0x0b; }
            if (id == "KI6") { if (!on) return 0x25; else return 0x2c; }
            if (id == "KI5") { if (!on) return 0x24; else return 0x2d; }
            if (id == "KI7") { if (!on) return 0x2e; else return 0x27; }


            if (id == "H1") { if (!on) return 0x20; else return 0x29; }
            if (id == "H2") { if (!on) return 0x42; else return 0x43; }
            if (id == "H3") { if (!on) return 0x16; else return 0x1f; }
            if (id == "H4") { if (!on) return 0x40; else return 0x41; }
            if (id == "H5") { if (!on) return 0x00; else return 0x09; }
            

            if (id == "CH1") { if (!on) return 0x35; else return 0x3c; }

            if (id == "EXT") { if (!on) return 0x2b; else return 0x22; }
            if (id == "HX1") { if (!on) return 0x08; else return 0x01; }
            if (id == "KTT") { if (!on) return 0x23; else return 0x2a; }
            if (id == "SQT") { if (!on) return 0x2f; else return 0x26; }

            return 0xff;
        }
        public Switch(SerialPort sp)
        {
            spbuf = new Queue<Byte>(4);
            cmdset = new Config("./command.xml");
            _sport = sp;
            _sport.DataReceived += new SerialDataReceivedEventHandler(_sport_DataReceived);
        }
        public void Send(string cmdid)
        {
            if (cmdid == "RESET")
                swis.Clear();
            object o = cmdset.GetField("Switch", cmdid, "sSTR");
            string ret;
            if (o is string)
                ret = o as string;
            else
                return;
            string[] cmds = ret.Split(',');
            foreach (string scmd in cmds)
            {
                if (scmd[0] == 'O')
                {
                    string cmd = scmd.Substring(1, scmd.Length - 1);
                    if (swis.ContainsKey(cmd) && (swis[cmd] == true))
                        continue;
                    SingleSwitch(true, cmd);
                    swis[cmd] = true;
                }
                if (scmd[0] == 'F')
                {
                    string cmd = scmd.Substring(1, scmd.Length - 1);
                    if (swis.ContainsKey(cmd) && (swis[cmd] == false))
                        continue;
                    SingleSwitch(false, cmd);
                    swis[cmd] = false;
                }
            }
        }
        bool hit = false;
        private void SingleSwitch(bool on,string id)
        {
            byte[] b = new byte[1];
            _sport.DiscardInBuffer();
            _sport.DiscardOutBuffer();
            hit = false;
            while (true)
            {
                int i = 0;
                while (i < 100) //10 seconds at most
                {
                    if (hit)
                        break;
                    Thread.Sleep(100);
                    if ((i % 30) == 0)
                    {
                    _sport.Write("UsUU");
                    b[0] = pos(id, on);
                    _sport.Write(b, 0, 1);
                    _sport.Write("V");
                    }
                    i++;
                }
                if (i >= 100)
                    MessageBox.Show("请检查主控板连接");
                else
                    return;
            }
        }
        void _sport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            if (_sport.BytesToRead > 0)
            {
                Byte b = (Byte)_sport.ReadByte();
                if (spbuf.Count >= 4)
                    spbuf.Dequeue();
                spbuf.Enqueue(b);
                if ((spbuf.Count == 4) && (b == 'V') && (spbuf.Peek() == 'U'))
                {
                    spbuf.Dequeue();
                    Byte P2 = spbuf.Dequeue();
                    Byte P3 = spbuf.Dequeue();
                    bRx = (P2 & 0x01) != 0;
                    bRs = (P2 & 0x02) != 0;
                    bOv1 = (P3 & 0x04) != 0;
                    bOv2 = (P3 & 0x08) != 0;
                    spbuf.Dequeue();
                    hit = true;
                }
            }
        }
    }
}
