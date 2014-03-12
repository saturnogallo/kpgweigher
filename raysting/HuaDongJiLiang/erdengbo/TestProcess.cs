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
        public TestDocument testdoc;
        //hardware device
        public static Scanner scanner;
        public static ThermBridge bridge;
        public static WIREMODE wire = WIREMODE.MODE_4WIRE;
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
            
            Scanner sc148;
            Scanner sc4012;
            Scanner2560 sc2560 = new Scanner2560("FLUKE2560");
            ThermBridge dq700 = new DQF700("F700");
            ThermBridge dq1560;
            if (dq700.bInUse)
            {
                bridge = dq700;
                sc148 = new ScannerSB148("SB148");
                sc4012 = new ScannerST4012A("ST4012A");
                if (sc148.bInUse || sc4012.bInUse)
                {
                    if (sc148.bInUse && sc4012.bInUse)
                    {

//                        if (DialogResult.Yes == MessageBox.Show("同时发现扫描器SB148 和 ST4012A.\n 按‘是’选择SB148\n按‘否’选择ST4012A", "选择扫描器", MessageBoxButtons.YesNo))
//                            scanner = sc148;
//                        else
                            scanner = sc4012;
                        return;
                    }
                    if (sc148.bInUse)
                        scanner = sc148;
                    else
                        scanner = sc4012;
                    return;
                }
                    MessageBox.Show("连接SB158或ST4012A扫描开关失败,请检查连接后重试", "连接失败");
            }
            else
            {
                dq1560 = new FL1560("FLUKE1560", sc2560);
                if (dq1560.bInUse)
                {
                    bridge = dq1560;
                    scanner = sc2560;
                    return;
                }
                MessageBox.Show("连接F700测温电桥或FLUKE1560失败,请检查连接后重试", "连接失败");
            }
            if (!Util.ISDEBUG)
            {
                Process.GetCurrentProcess().Kill();
            }
            
        }
        private Decimal GetStdMultiply(ABCParam param)
        {
            if (bridge is DQF700)
            {
                return 100;
                /*
                if (Math.Abs(param.rtp - 25) < 1)
                    return 25;
                if (Math.Abs(param.rtp - 100) < 1)
                    return 100;
                else
                    throw new Exception("未知探头类型");
                 */
            }else if (bridge is FL1560)
            {
                    return 1;
            }
            else
                    throw new Exception("不支持的电桥");

        }
        private Decimal GetMultiply()
        {

                if (bridge is DQF700)
                {
                    return 100;
                    /*
                    if (testdoc.ProbeType == "PT100")
                        return 100;
                    else if (testdoc.ProbeType == "PT25")
                        return 25;
                    else
                        throw new Exception("未知探头类型");
                     */
                }
                else if (bridge is FL1560)
                {
                    return 1;
                }
                else
                    throw new Exception("不支持的电桥");


        }

        public void StartProcess(BackgroundWorker formworker)
        {
            /*
            ABCParam ppp = new ABCParam();
            ppp.LoadFromIni("PROBE_TEST2");
            double uu = Wendu.RValueToTValue(40.083 / ppp.rtp, ppp, ConvertMethod.ITS90);
            return;
             */
            worker = formworker;
            
            FormAction("show_status", "测试进行中..."); // FormAction("show_message", "请到原始记录页,测量并填入500度时的实际温度和阻值");

            Dictionary<string, string> checkedpts = new Dictionary<string, string>();
            for (int ipage = 0; ipage < 6; ipage++)
            {
                string pgid = "abcdef"[ipage].ToString();
                List<TEMPRDG> trdgs = new List<TEMPRDG>();
                ABCParam param = new ABCParam();
                if (!testdoc.IsValueAvailable(new string[] { "m_" + pgid + "_bzqbh" }))
                    continue;
                 if(!param.LoadFromIni("PROBE_" + testdoc.StrValue("m_" + pgid + "_bzqbh")))
                    throw new Exception(String.Format("第{0}页无效的标准器编号",(ipage+1).ToString()));

            for(int ipoint = 1; ipoint <= TestDocument.MAX_PT_1PAGE;ipoint++) //check every ipoint
            {
                string sdwd = "m_" + pgid+ "_" + ipoint.ToString() + "jddwd";
                if(!testdoc.IsValueAvailable(new string[]{sdwd}))
                    continue;
                if(checkedpts.ContainsKey(testdoc.StrValue(sdwd))) //tested before
                    continue;
                checkedpts.Add(testdoc.StrValue(sdwd), "m_" + pgid + "_bzqbh");
                Decimal setpoint;
                if (!Decimal.TryParse(testdoc.StrValue(sdwd), out setpoint))
                        continue;
                    string choice = FormAction("show_message", String.Format("请将所有探头放入恒温槽,并将温度设置到 {0} ℃。\n点击'跳过'直接重测下一点", setpoint.ToString()));
                    if (choice == "skip")
                        continue;
                    FormAction("clear_graph", "");
                
                    CheckCancel(2);
                    trdgs.Clear();
                    //wait temp stable
                    scanner.ScanTo(0, WIREMODE.MODE_4WIRE); //force 1st channel (standard) to be 4 wire
                    double tempnow;

                    double period = Double.Parse(Util.ConstIni.StringValue("CONDITION", "PERIOD"));
                    double pplimit = Double.Parse(Util.ConstIni.StringValue("CONDITION", "PPLIMIT"));
                    int scandelay = Int32.Parse(Util.ConstIni.StringValue("CONDITION", "SCANDELAY"));
                    int goodcnt = Int32.Parse(Util.ConstIni.StringValue("CONDITION", "GOODCNT"));

                    bridge.ClearBuffer();
                    while (true)
                    {
                        tempnow = Convert.ToDouble(ReadF700(1)* GetStdMultiply(param)) / param.rtp;
                        tempnow = Wendu.RValueToTValue(tempnow, param, ConvertMethod.ITS90);
                        //convert to temperature
                        trdgs.Add(new TEMPRDG(Convert.ToDecimal(tempnow), DateTime.Now));
                        FormGraphAction(0, tempnow,"c");
                        FormAction("show_status", String.Format("等待温度稳定....{0} ℃", tempnow.ToString("F03")));

                        while ((trdgs.Count > 0) && trdgs[0].time < DateTime.Now.AddMinutes(-period)) 
                            trdgs.RemoveAt(0);

                        if ((trdgs.Count > 10) && 
                            (trdgs[trdgs.Count - 1].time.Subtract(trdgs[0].time).TotalMinutes > (period-0.1)) &&
                            IsDiffSmall(trdgs[trdgs.Count - 1].reading, setpoint, 2) &&
                            ((Util.my_max(trdgs, 0) - Util.my_min(trdgs, 0)) < pplimit))
                        {
                            break;
                        }
                        CheckCancel(1);
                    }
                    FormAction("clear_graph", "");
                    //start to collect data
                    Decimal rnow;
                    for (int round = 1; round <= 4; round++)
                    {
                        int sindex = 0;
                        for (int sch = 0; sch < 24; sch++)
                        {
                            if (sch > 0)
                            {
                                if (!testdoc.IsValueAvailable(new string[] { "m_" + sch.ToString() + "_dj" }))
                                    continue;

                                string cdpt = "m_"+"abcdef"[(sch - 1) / 4].ToString() ;
                                if (testdoc.IsValueAvailable(new string[] { cdpt + "_1jddwd" }) && (testdoc.StrValue(cdpt + "_1jddwd") == testdoc.StrValue(sdwd)))
                                    sindex++;
                                else if (testdoc.IsValueAvailable(new string[] { cdpt + "_2jddwd" }) && (testdoc.StrValue(cdpt + "_2jddwd") == testdoc.StrValue(sdwd)))
                                    sindex++;
                                else if (testdoc.IsValueAvailable(new string[] { cdpt + "_3jddwd" }) && (testdoc.StrValue(cdpt + "_3jddwd") == testdoc.StrValue(sdwd)))
                                    sindex++;
                                else
                                    continue;
                            }
                            if (sindex == 0)
                            {
                                FormAction("show_status", String.Format("正在采集标准温度计数据..."));
                                scanner.ScanTo(sindex, WIREMODE.MODE_4WIRE);
                            }
                            else
                            {
                                FormAction("show_status", String.Format("正在采集被检温度计 {0} 数据...", sindex));
                                scanner.ScanTo(sindex, wire);
                            }
                            CheckCancel(scandelay);
                            bridge.ClearBuffer();
                            rnow = ReadF700(goodcnt);
                            if (sindex == 0)
                            {
                                this["c_" +pgid+ "_"+ ipoint.ToString() + "sjwd_" + round.ToString()] = Convert.ToDecimal(Wendu.RValueToTValue(Convert.ToDouble(rnow*GetStdMultiply(param))/param.rtp, param, ConvertMethod.ITS90));
                                FormGraphAction(sindex, Convert.ToDouble(rnow * GetStdMultiply(param)), "ohm");
                            }
                            else
                            {
                                if(wire == WIREMODE.MODE_3WIRE)
                                {
                                    this["c_" + sch.ToString() + "_" + ipoint.ToString() + "bcw3_" + round.ToString()] = rnow*GetMultiply();
                                }
                                else{
                                    this["c_" + sch.ToString() + "_" + ipoint.ToString() + "bc_" + round.ToString()] = rnow * GetMultiply();
                                }
                                FormGraphAction(sindex, Convert.ToDouble(rnow * GetMultiply()), "ohm");
                            }
                            CheckCancel(2);
                        }
                        if (wire == WIREMODE.MODE_3WIRE) //another roudn for 2 wire measure
                        {
                            int nsindex = 0;
                            for (int sch = 0; sch < 24; sch++)
                            {
                                if (sch > 0)
                                {
                                    if (!testdoc.IsValueAvailable(new string[] { "m_" + sch.ToString() + "_dj" }))
                                        continue;

                                    string cdpt = "m_" + "abcdef"[(sch - 1) / 4].ToString();
                                    if (testdoc.IsValueAvailable(new string[] { cdpt + "_1jddwd" }) && (testdoc.StrValue(cdpt + "_1jddwd") == testdoc.StrValue(sdwd)))
                                        nsindex++;
                                    else if (testdoc.IsValueAvailable(new string[] { cdpt + "_2jddwd" }) && (testdoc.StrValue(cdpt + "_2jddwd") == testdoc.StrValue(sdwd)))
                                        nsindex++;
                                    else if (testdoc.IsValueAvailable(new string[] { cdpt + "_3jddwd" }) && (testdoc.StrValue(cdpt + "_3jddwd") == testdoc.StrValue(sdwd)))
                                        nsindex++;
                                    else
                                        continue;
                                }
                                if (nsindex == 0)
                                {
                                    continue;
                                }
                                else
                                {
                                    FormAction("show_status", String.Format("正在采集被检温度计 {0} 数据...", sindex));
                                    scanner.ScanTo(nsindex, WIREMODE.MODE_2WIRE);
                                }
                                CheckCancel(scandelay);
                                bridge.ClearBuffer();
                                rnow = ReadF700(goodcnt);
                                this["c_" + sch.ToString() + "_" + ipoint.ToString() + "bcw2_" + round.ToString()] = rnow * GetMultiply();
                                FormGraphAction(sindex, Convert.ToDouble(rnow * GetMultiply()), "ohm");
                                CheckCancel(2);
                                testdoc.AutoCaculation(true);
                            }
                        }
                    }
                    testdoc.AutoCaculation(false);
                }
            }
            scanner.ScanTo(0,WIREMODE.MODE_4WIRE);
            Thread.Sleep(1000);
        }
        private Decimal ReadF700(int gdcnt)
        {
            while (true)
            {
                try
                {
                    Decimal rdg = 0;
                    int goodcnt = gdcnt; //continous good check
                    while (true)
                    {
                        CheckCancel();
                        Thread.Sleep(1000);
                        int retry = 3;

                        while (retry-- > 0)
                        {
                            try
                            {
                                rdg = bridge.Resistance;
                                break;
                            }
                            catch
                            {
                                CheckCancel(1);
                            }
                        }
                        if (retry == 0)
                            throw new Exception("读取温度数据失败");
                        if (!bridge.bStable)
                        {
                            FormAction("show_status", "等待电桥稳定中...");
                            CheckCancel(1);
                            goodcnt = gdcnt;
                            continue;
                        }
                        if (--goodcnt > 0)
                        {
                            CheckCancel(1);
                            continue;
                        }
                        break;
                    }
                    return rdg;
                }
                catch(Exception ex)
                {
                    if (ex.Message != "") //not in cancel case
                    {
                        FormAction("show_message", String.Format("读取温度数据失败,请检查连接和电源。\n按'确定'后重试。"));
                        Thread.Sleep(3000);
                    }
                    else
                    {
                        throw ex;
                    }
                        
                }
            }
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
                ret = this.form_result;
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
