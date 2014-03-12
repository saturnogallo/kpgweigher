using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;
using System.Threading;
using System.ComponentModel;
using System.Windows.Forms;
using System.IO;

namespace Jmbo
{
    internal class TestProcess
    {
        public bool bRunning = false;

        //setting
        public TempRange range; //range for temperature measurement
        public TestDocument testdoc;
        //hardware device
        public static SwitchS200 trigger;
        //public static SwitchSER trigger1;
        //public static SwitchSER trigger2;
        //public static SwitchSER trigger3;
        //public static SwitchSER trigger4;
        public static WD1529 wd1529;
        public static BathCTRJ bath300A;
        public static BathCTRJ bathN35A;

        private IniHandler ovencfg;
        private Form1 formbase;
        //testdoc related configuration
        Decimal this[string id]
        {
            get
            {
                try
                {
                    string ret = testdoc[id].ToString();
                    if (ret != "")
                        return Decimal.Parse(ret);
                }
                catch
                {

                }
                return 0;

            }
            set
            {
                try
                {
                    testdoc[id] = value;
                    FormAction("fill_page", "");
                }
                catch
                {
                }
            }
        }
        public TestProcess(Form1 form)
        {
            formbase = form;

            testdoc = form.test;
            ovencfg = new IniHandler(Path.Combine(Util.basedir, "oven.ini"), "");

            trigger = new SwitchS200("S7214");
            //trigger1 = new SwitchSER("TRIGGER1");
            //trigger2 = new SwitchSER("TRIGGER2");
            //trigger3 = new SwitchSER("TRIGGER3");
            //trigger4 = new SwitchSER("TRIGGER4");
            wd1529 = new WD1529("WD1529");
            bath300A = new BathCTRJ("CTRJ-300A");
            bathN35A = new BathCTRJ("CTRJ-35A");
            if (!bath300A.bInUse && !Util.ISDEBUG)
                MessageBox.Show("连接CTRJ-300A 恒温槽失败,请检查串口设置后重试", "连接失败");
            if (!bathN35A.bInUse && !Util.ISDEBUG)
                MessageBox.Show("连接CTRJ-N35A 恒温槽失败,请检查串口设置后重试", "连接失败");

            if (!wd1529.bInUse || !trigger.bInUse)//!trigger1.bInUse || !trigger2.bInUse || !trigger3.bInUse || !trigger4.bInUse)
            {
                if (!wd1529.bInUse && !Util.ISDEBUG)
                    MessageBox.Show("连接FLUKE 1529失败,请检查串口设置后重试", "连接失败");
                //if ((!trigger1.bInUse || !trigger2.bInUse || !trigger3.bInUse || !trigger4.bInUse) && !Util.ISDEBUG)
                //    MessageBox.Show("连接开关检测模块失败,请检查串口设置后重试", "连接失败");

                if (!trigger.bInUse && !Util.ISDEBUG)
                    MessageBox.Show("连接S7-200 开关检测模块失败,请检查串口设置(9600/8/EVEN/1STOP)后重试, S7-200 地址为2，连接I0.0-I0.3)", "连接失败");
                if (!Util.ISDEBUG)
                    Process.GetCurrentProcess().Kill();
            }
        }
        private string lastoven = "";
        private void SetTemperature(Decimal setpoint)
        {
            if (setpoint < 50) //sojo
            {
                if (lastoven != "N35A")
                {
                    FormAction("show_status", String.Format("请将所有探头放入CTRJ-N35A恒温槽"));
                    lastoven = "N35A";
                }
                bathN35A.SetTemperature(setpoint.ToString());
            }
            else
            {
                if (lastoven != "300A")
                {
                    FormAction("show_message", String.Format("请将所有探头放入CTRJ-300A恒温槽"));
                    lastoven = "300A";
                }
                bath300A.SetTemperature(setpoint.ToString());
            }
        }
        private string GetCurrentIO()
        {
            string ret;
            for (int i = 0; i < 3; i++)
            {
                 ret = trigger.ReadInput();
                 if (ret.Length == 4)
                     return ret;
                 Thread.Sleep(300);
            }
            throw new Exception("读取S7-200状态失败");
        }
        public void StartProcess(BackgroundWorker formworker)
        {
            worker = formworker;

            FormAction("show_status", "测试进行中...");

            List<TEMPRDG> trdgs = new List<TEMPRDG>();

            for(int ipoint = 1; ipoint <= TestDocument.MAX_PT_1PAGE;ipoint++) //check every ipoint
            {
                string pkey = "m_1_" + ipoint.ToString(); //all pages should have same setpoint
                Decimal setpoint;
                if(!testdoc.IsValueAvailable(new string[]{pkey + "wdd"}))
                    continue;
                if (!Decimal.TryParse(testdoc.StrValue(pkey + "wdd"), out setpoint))
                    continue;

                SetTemperature(setpoint);
                FormAction("clear_graph","");
                CheckCancel(2);
                trdgs.Clear();
#region wait temp stable
                double tempnow;
                double period = Double.Parse(Util.ConstIni.StringValue("CONDITION", "PERIOD"));
                double pplimit = Double.Parse(Util.ConstIni.StringValue("CONDITION", "PPLIMIT"));

                while (true)
                {
                    DateTime st = DateTime.Now;
                    while (!wd1529.HasReading(1))
                    {
                        FormAction("show_status", "读取标准温度计温度....");
                        CheckCancel(1);
                        if(DateTime.Now.Subtract(st).TotalSeconds > 10)
                        {
                            MessageBox.Show("读取标准温度计读数失败，请检查FLUKE 1529 连接和设置.(PRINT ON, 波特率9600).");
                            st = DateTime.Now;
                        }
                    }
                    
                        TEMPRDG rdg = wd1529.GetTemperature(1);
                        tempnow = Convert.ToDouble(rdg.reading);
                        //convert to temperature
                        trdgs.Add(new TEMPRDG(rdg.reading, DateTime.Now));
                        FormGraphAction(0, tempnow, "c");
                        FormAction("show_status", String.Format("等待温度稳定....{0} ℃", tempnow.ToString("F03")));

                          while ((trdgs.Count > 0) && trdgs[0].time < DateTime.Now.AddMinutes(-period)) 
                            trdgs.RemoveAt(0);
                        
                        
                        if ((trdgs.Count > 10) && 
                            (trdgs[trdgs.Count - 1].time.Subtract(trdgs[0].time).TotalMinutes > (period-0.1))&&
                            ((Util.my_max(trdgs, 0) - Util.my_min(trdgs, 0)) < pplimit) && 
                            IsDiffSmall(trdgs[trdgs.Count - 1].reading, setpoint, 2))
                        {
                            break;
                        }
                    
                    CheckCancel(1);
                }
                #endregion
                FormAction("clear_graph","");
#region start to collect const temp data
                Decimal rnow = 0;
                for (int round = 1; round <= 4; round++)
                {
                    while (!wd1529.HasReading(1))
                    {
                        CheckCancel(1);
                    }
                    Decimal stdnow = wd1529.GetTemperature(1).reading;
                    FormGraphAction(0, Convert.ToDouble(stdnow), "c");
                    for (int sch = 1; sch < TestDocument.MAX_BC; sch++)
                    {
                        if (!testdoc.IsValueAvailable(new string[] { "m_" + sch.ToString() + "_fdz"}))
                            continue;
                        this["c_" + sch.ToString() + "_" + ipoint.ToString() + "wd_" + round.ToString()] = stdnow;
                        int retry = 0;
                        Input askdlg = new Input();
                askdlg.Message = String.Format("请输入控温仪{0} 示值数据...", sch);

                        while (true)
                        {
                            if (askdlg.ShowDialog() == DialogResult.OK)
                            {
                                if (Decimal.TryParse(askdlg.result, out rnow))
                                    break;
                            }

                            if(retry++ > 3)
                                throw new Exception("测试被中止.");
                        }
                        this["c_" + sch.ToString() + "_" + ipoint.ToString() + "bc_" + round.ToString()] = rnow;
                        FormGraphAction(sch, Convert.ToDouble(rnow),"ohm");
                    }
                }
#endregion
                testdoc.AutoCaculation();
            }
            FormAction("clear_graph", "");
            //start qie huan data test
            for (int ipoint = 1; ipoint <= TestDocument.MAX_PT_1PAGE; ipoint++) //check every ipoint
            {
                string pkey = "m_1_" + ipoint.ToString(); //all pages should have same setpoint
                Decimal setpoint;
                if (!testdoc.IsValueAvailable(new string[] { pkey + "qhwd" }))
                    continue;
                if (!Decimal.TryParse(testdoc.StrValue(pkey + "qhwd"), out setpoint))
                    continue;

                SetTemperature(setpoint);
                #region wait temp stable
                double tempnow;
                double period = Double.Parse(Util.ConstIni.StringValue("CONDITION", "PERIOD"));
                double pplimit = Double.Parse(Util.ConstIni.StringValue("CONDITION", "PPLIMIT"));
                while (true)
                {
                    while (!wd1529.HasReading(1))
                    {
                        CheckCancel(1);
                    }

                    TEMPRDG rdg = wd1529.GetTemperature(1);
                    tempnow = Convert.ToDouble(rdg.reading);
                    //convert to temperature
                    trdgs.Add(new TEMPRDG(rdg.reading, DateTime.Now));
                    FormGraphAction(0, tempnow,"c");
                    FormAction("show_status", String.Format("准备测量切换温度，等待温度稳定....{0} ℃", tempnow.ToString("F03")));

                    while ((trdgs.Count > 0) && trdgs[0].time < DateTime.Now.AddMinutes(-period))
                        trdgs.RemoveAt(0);
                    if ((trdgs.Count > 10) && (trdgs[trdgs.Count - 1].time.Subtract(trdgs[0].time).TotalMinutes > (period-0.1)) && 
                        (Util.my_max(trdgs, 0) - Util.my_min(trdgs, 0)) < pplimit && 
                        IsDiffSmall(trdgs[trdgs.Count - 1].reading, setpoint, 2)) //sojo
                    {
                        break;
                    }
                    CheckCancel(1);
                }
                #endregion

                string lastio = GetCurrentIO();
                FormAction("clear_graph", "");
                #region collect up switch temperature
                
                decimal uprange =  Decimal.Parse(Util.ConstIni.StringValue("CONDITION", "UPRANGE"));
                SetTemperature(setpoint + uprange);
                FormAction("show_status", String.Format("上切换值测量中...."));
                while (true)
                {
                    if (!wd1529.HasReading(1))
                    {
                        CheckCancel();
                        continue;
                    }
                    TEMPRDG rdg = wd1529.GetTemperature(1);
                    FormGraphAction(0, Convert.ToDouble(rdg.reading),"c");
                    
                    string newio = GetCurrentIO();
                    bool alldone = true;
                    for (int ibc = 1; ibc <= TestDocument.MAX_BC; ibc++)
                    {
                        if (!testdoc.IsValueAvailable(new string[] { "m_" + ibc.ToString() + "_fdz" }))
                            continue;
                        if (newio[ibc - 1] == lastio[ibc - 1])
                        {
                            alldone = false;
                            continue;
                        }
                        string ckey;
                        ckey= "c_" + ibc.ToString() + "_" + ipoint.ToString() + "sqh_1";
                        if (!testdoc.IsValueAvailable(new string[] { ckey }))
                        {
                            this[ckey] = rdg.reading;
                            Thread.Sleep(200);
                            while(!wd1529.HasReading(1))
                                Thread.Sleep(200);
                            rdg = wd1529.GetTemperature(1);
                            ckey = "c_" + ibc.ToString() + "_" + ipoint.ToString() + "sqh_2";
                            if (!testdoc.IsValueAvailable(new string[] { ckey }))
                                this[ckey] = rdg.reading;
                        }
                    }
                    if (alldone || IsDiffSmall(rdg.reading, setpoint + uprange+2, 2))
                        break;
                }
                testdoc.AutoCaculation();
                #endregion

                #region collect down switch temperature
                lastio = GetCurrentIO();
                decimal dnrange = Decimal.Parse(Util.ConstIni.StringValue("CONDITION", "DNRANGE"));
                SetTemperature(setpoint + dnrange);
                FormAction("show_status", String.Format("下切换值测量中...."));
                while (true)
                {
                    if (!wd1529.HasReading(1))
                    {
                        CheckCancel();
                        continue;
                    }
                    TEMPRDG rdg = wd1529.GetTemperature(1);
                    FormGraphAction(0, Convert.ToDouble(rdg.reading),"c");

                    string newio = GetCurrentIO();
                    bool alldone = true;
                    for (int ibc = 1; ibc <= TestDocument.MAX_BC; ibc++)
                    {
                        if (!testdoc.IsValueAvailable(new string[] { "m_" + ibc.ToString() + "_fdz" }))
                            continue;
                        if (newio[ibc - 1] == lastio[ibc - 1])
                        {
                            alldone = false;
                            continue;
                        }
                        string ckey;
                        ckey = "c_" + ibc.ToString() + "_" + ipoint.ToString() + "xqh_1";
                        if (!testdoc.IsValueAvailable(new string[] { ckey }))
                        {
                            this[ckey] = rdg.reading;
                            Thread.Sleep(200);
                            while (!wd1529.HasReading(1))
                                Thread.Sleep(200);
                            rdg = wd1529.GetTemperature(1); ckey = "c_" + ibc.ToString() + "_" + ipoint.ToString() + "xqh_2";
                            if (!testdoc.IsValueAvailable(new string[] { ckey }))
                                this[ckey] = rdg.reading;
                        }
                    }
                    if (alldone || IsDiffSmall(rdg.reading, setpoint + dnrange - 2, 2))
                        break;
                }
                testdoc.AutoCaculation();
                #endregion
            }
            Thread.Sleep(1000);
        }
     
        public void CheckThreadState(Thread thread)
        {
            string abc;
            while ((thread.ThreadState == System.Threading.ThreadState.Running) ||
                (thread.ThreadState == System.Threading.ThreadState.WaitSleepJoin))
            {
                abc = thread.ThreadState.ToString();
                Thread.Sleep(1000);
            }
            CheckCancel();
        }

 
        private bool NeedExtraRInput(string message, ref decimal value)
        {

            string abc = FormAction("ask_input", message);
            try
            {
                if (abc != "")
                {
                    value = Decimal.Parse(abc);
                    return true;
                }
            }
            catch
            {
            }
            return false;
        }



        public string form_result;
        private static Mutex talklock = new Mutex();
        private void FormGraphAction(int ch, double value,string type)
        {
            formbase.Invoke(new Action<int,double,string>(formbase.AddToGraph), new object[] { ch, value,  type });
        }
        private string FormAction(string type, string args)
        {
            talklock.WaitOne();
            string ret = "";

            if (type == "clear_graph")
            {

                formbase.Invoke(new Action<string>(formbase.ClearAllGraph), new object[] { args });

            }

            if (type == "fill_page")
            {

                formbase.Invoke(new Action<string>(formbase.FillInPage), new object[] { args });

            }
            if (type == "show_status")
            {

                formbase.Invoke(new Action<string>(formbase.UpdateStatus), new object[] { args });

            }
            if (type == "ask_input")
            {
                formbase.uilock.Reset();
                formbase.Invoke(new Action<string>(formbase.askinput), new object[] { args });
                while (!formbase.uilock.WaitOne(10, false))
                {
                    CheckCancel();
                }
                ret = this.form_result;
            }
            if (type == "show_message")
            {
                formbase.uilock.Reset();
                formbase.Invoke(new Action<string>(formbase.ShowMessage), new object[] { args });
                while (!formbase.uilock.WaitOne(10, false))
                {
                    CheckCancel();
                }
            }
            talklock.ReleaseMutex();
            return ret;
        }

        private bool WaitMessage(string prefix, DateTime end)
        {
            TimeSpan ts = end.Subtract(DateTime.Now);
            if (ts.TotalSeconds <= 0)
                return false;
            FormAction("show_status", String.Format("{0} - {1}:{2}", prefix, ts.Minutes.ToString("D2"), ts.Seconds.ToString("D2")));
            
            CheckCancel();
            return true;
        }
  
        private bool IsDiffSmall(Decimal current, Decimal target, double tolerance)
        {
            Decimal tol = Convert.ToDecimal(tolerance);
            return (current < (target + tol) && current > (target - tol));
        }
     
        private BackgroundWorker worker;
        private void CheckCancel(int seconds) //wait seconds
        {
            while (true)
            {
                CheckCancel();
                Thread.Sleep(1000);
                if (--seconds > 0)
                    FormAction("show_status", String.Format("等待{0}秒...", seconds));
                else
                    return;
            }
        }
        
        private void CheckCancel()
        {
            if (worker.CancellationPending)
                throw new Exception("");
        }
    }
}
