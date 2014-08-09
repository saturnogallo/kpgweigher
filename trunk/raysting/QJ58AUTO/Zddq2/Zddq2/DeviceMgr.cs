using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.Diagnostics;
using System.IO;
using System.IO.Ports;
using System.Text.RegularExpressions;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Data;
using Raysting.Common;
namespace Zddq2
{
    public delegate void StrEventHandler(object sender, string e);
    public enum RUN_STATE
    {
        DISABLED,
        IDLE,
        INITIALIZING,
        SEARCHING,
        TRACKING,
        READING,
        STOPPING,
        ERROR
    }

    public class FullScreenClass
    {
        public const int SPI_SETWORKAREA = 47;
        public const int SPI_GETWORKAREA = 48;
        public const int SW_HIDE = 0x00;
        public const int SW_SHOW = 0x0001;
        public const int SPIF_UPDATEINIFILE = 0x01;
        [DllImport("coredll.dll", EntryPoint = "FindWindow")]
        private static extern IntPtr FindWindow(string lpWindowName, string lpClassName);
        [DllImport("coredll.dll", EntryPoint = "ShowWindow")]
        private static extern bool ShowWindow(IntPtr hwnd, int nCmdShow);
        [DllImport("coredll.dll", EntryPoint = "SystemParametersInfo")]
        private static extern int SystemParametersInfo(int uAction, int uParam, ref Rectangle lpvParam, int fuWinIni);

        /// <summary>
        /// 设置全屏或取消全屏
        /// </summary>
        /// <param name="fullscreen">true:全屏 false:恢复</param>
        /// <param name="rectOld">设置的时候，此参数返回原始尺寸，恢复时用此参数设置恢复</param>
        /// <returns>设置结果</returns>
        public static bool SetFullScreen(bool fullscreen, ref Rectangle rectOld)
        {
            IntPtr Hwnd = FindWindow("HHTaskBar", null);
            if (Hwnd == IntPtr.Zero) return false;
            if (fullscreen)
            {
                ShowWindow(Hwnd, SW_HIDE);
                /*
                Rectangle rectFull = Screen.PrimaryScreen.Bounds;
                SystemParametersInfo(SPI_GETWORKAREA, 0, ref rectOld, SPIF_UPDATEINIFILE);//get
                SystemParametersInfo(SPI_SETWORKAREA, 0, ref rectFull, SPIF_UPDATEINIFILE);//set
                 */
            }
            else
            {
                ShowWindow(Hwnd, SW_SHOW);
                /*
                SystemParametersInfo(SPI_SETWORKAREA, 0, ref rectOld, SPIF_UPDATEINIFILE);
                 */
            }
            return true;
        }

    }
    public class TaskMachine
    {
        
        //public RUN_STATE state = RUN_STATE.IDLE;
        public RxInfo currRx;
        //private List<RxInfo> rx_Lists no use now , can use 1..24 for compare
        public bool bRunning = false;
        public bool bPaused = false;
        public TaskMachine()
        {
            datafilter = new List<double>();
            valfilter = new List<double>();
        }
        private int curr_ch = 0;
        public int stm = 0;
        private int delay = 0;
        public StrEventHandler StatusChanged;
        private const int sec_cnt = 4;
        public int waiting
        {
            get
            {
                return (delay+1) / sec_cnt;
            }
        }
        //compose string of curr, sqr std, constv

        private string ComposeAction()
        {
            string ret = "";
            switch(currRx.iIx)
            {
                case -1: ret = ret + "CURR_P0001"; break; //0.1mA
                case 0: ret = ret + "CURR_P001"; break; //1mA
                case 1: ret = ret + "CURR_P01"; break; //10mA
                case 2: ret = ret + "CURR_P1"; break; //0.1A
                case 3: ret = ret + "CURR_P3"; break; //0.3A
                case 4: ret = ret + "CURR_1"; break; //1A
                case 5: ret = ret + "CURR_5"; break; //5A
                
            }
            ret = ret + "|";
            if(currRx.bSqrt)
                ret = ret + "X2|";
            else
                ret = ret + "X1|";
                
                if (currRx.iIx == 3) //0.3A
                    ret = ret + "STD_P1|VMODE_3V";
                else if (currRx.iIx == 4) //1A
                    ret = ret + "STD_P01|VMODE_3V";
                else if (currRx.iIx == 5) //5A
                    ret = ret + "STD_P001|VMODE_3V";
                else
                    ret = ret + "STD_1|VMODE_3V";

            return ret;
        }
        private bool bInReading = false;
        private double dStableReading = -9999;
        private List<double> datafilter;
        private int dataCount;
        private bool broughk; //variance  < 1/1k will be ok
        private void StartCollectReading(bool roughk)
        {
            broughk = roughk;
            bInReading = true;
            dataCount = 0;
            datafilter.Clear();
        }
        private List<double> valfilter;
        private bool ValidNewValue(ref double val)
        {
            valfilter.Add(val);
            if(valfilter.Count < RunWnd.syscfg.iFilter)
                return false;
            double avg = valfilter.Average();
            double sqr = 0;
            foreach (double v in valfilter)
            {
                sqr = sqr + v * (v - avg);
            }
            double sqr3 = 2*Math.Sqrt(sqr / (valfilter.Count - 1));
            if (sqr3 < 1e-7)
                sqr3 = 1e-7;
            int i = 0;
            double ret = 0;
            foreach(double v in valfilter)
            {
                if ((val > (avg - sqr3)) && (val < (avg + sqr3)))
                {
                    ret = ret + val;
                    i = i+1;
                    continue;
                }
            }
            if (i <= 1)
                return false;
            val = ret / i;
            return true;
        }

        private bool FoundWildData()
        {
            double avg = datafilter.Average();
            double sqr = 0;
            foreach (double v in datafilter)
            {
                 sqr = sqr + v * (v - avg);
            }
            double sqr3 = 2*Math.Sqrt(sqr / (datafilter.Count - 1));
            if(sqr3 < 1e-8)
                sqr3 = 1e-8;
            int i = 0;
            bool ret = false;
            while(i < datafilter.Count)
            {
                if ((datafilter[i] < (avg - sqr3)) || (datafilter[i] > (avg + sqr3)))
                {
                    datafilter.RemoveAt(i);
                    dataCount--;
                    ret = true;
                    continue;
                }
                i++;
            }
            return ret;
        }
        private bool IsReadingStable()
        {
            if (DeviceMgr.IsInAction())
                return false;
            if (dataCount <= 0)
            {
                DeviceMgr.Action("navread", 0);
                dataCount = 1;
                delay = sec_cnt / 4;
                return false;
            }
            else
            {
                datafilter.Add(DeviceMgr.reading);
                if (dataCount < RunWnd.syscfg.iSampleTimes)
                {
                    DeviceMgr.Action("navread", 0);
                    delay = sec_cnt / 4;
                    dataCount++;
                    return false;
                }
                else
                {

                    if (broughk)
                    {
                        dStableReading = datafilter[datafilter.Count - 1];
                        datafilter.Sort();
                        if (Math.Abs(datafilter[datafilter.Count - 1] - datafilter[0]) < Math.Abs(datafilter.Average()) * 0.001)
                            return true;

                        dataCount = 0;
                        datafilter.Clear();
                        return false;
                    }
                    if (FoundWildData())
                        return false;

                    int cnt0 = 0;  //count for no change
                    int cnt1 = 0;  //count for going up
                    int cnt_1 = 0; //count for going down
                    for (int i = 1; i < datafilter.Count; i++)
                    {
                        if (Math.Abs(datafilter[i] - datafilter[i - 1]) < 1e-12)
                        {
                            cnt0++;
                        }
                        else
                        {
                            if (datafilter[i] > datafilter[i - 1])
                                cnt1++;
                            else
                                cnt_1++;
                        }
                    }
                    if ((cnt0 > datafilter.Count * 0.6) || 
                        ((Math.Abs(cnt1 - cnt_1) < datafilter.Count * 0.6) && (cnt_1>= 1) && (cnt1>= 1)) ||
                        ((cnt0 > datafilter.Count * 0.3) && (cnt_1 >= 1) && (cnt1 >= 1)))
                    {
                        //60% no change or cnt1 and cnt_1 are almost equal
                    }
                    else
                    {
                        dataCount = 0; //do collection again
                        datafilter.Clear();
                        return false;
                    }
                    
                    //full data acquired
                    int win_start = 0;
                    int win_end = datafilter.Count - 1;
                    int avg_cnt = datafilter.Count / 4;
                    if (avg_cnt < 3)
                        avg_cnt = 3;
                    if (RunWnd.syscfg.sFilterType == "filtertype1") //中位数平均值
                    {
                        datafilter.Sort();
                        win_start = (datafilter.Count - avg_cnt) / 2;
                        win_end = (datafilter.Count + avg_cnt) / 2;
                        dStableReading = 0;
                        for (int i = win_start; i <= win_end; i++)
                        {
                            dStableReading += datafilter[i];
                        }
                        dStableReading = dStableReading / (win_end - win_start + 1);
                        bInReading = false;
                    }
                    if (RunWnd.syscfg.sFilterType == "filtertype2") //average of final data
                    {
                        win_start = (datafilter.Count - avg_cnt);
                        win_end = datafilter.Count - 1;
                    }
                    if (RunWnd.syscfg.sFilterType == "filtertype3") //
                    {
                        for (int i = 1; i < (datafilter.Count-1); i++)
                        {
                            datafilter[i] = (2 * datafilter[i] + datafilter[i - 1] + datafilter[i + 1]) / 4;
                        }
                        win_start = 0;
                        win_end = datafilter.Count - 1;
                    }
                    dStableReading = 0;
                    for (int i = win_start; i <= win_end; i++)
                    {
                        dStableReading += datafilter[i];
                    }
                    dStableReading = dStableReading / (win_end - win_start + 1);
                    datafilter.Clear();
                    dataCount = 0;
                    bInReading = false;
                    return true;
                }
            }
        }
        public int iMeasCnt;
        public void Start()
        {
            if (!bRunning)
            {
                stm = 0;
                iMeasCnt = 1;
                InitStep();
                bRunning = true;
            }
        }
        public void PauseResume()
        {
            if (bRunning)
            {
                bPaused = !bPaused;
            }
        }
        public void InitStep()
        {
            curr_ch = 0;
            currRx = Program.lst_rxinfo[curr_ch];
            valfilter.Clear();
            datafilter.Clear();
            currRx.var.rRs = Program.lst_rsinfo[currRx.iStdChan - 1].dTValue;
        }
        bool is1V = true;
        public void Step()
        {

            if (DeviceMgr.IsInAction())
                return;
            if(delay > 0)
            {
                delay--;
                return;
            }
            if (currRx.i_State == RUN_STATE.STOPPING)
            {
                DeviceMgr.Reset();
                bPaused = false;
                bInReading = false;
                currRx.i_State = RUN_STATE.IDLE;
                bRunning = false;
            }
            if (bPaused)
            {
                return;
            }
            if (bInReading)
            {
                if (!IsReadingStable())
                    return;
            }
            if (!bRunning)
                return;
            
            if (stm == 0)//search the next valid channel
            {
                curr_ch = 0;
                currRx = Program.lst_rxinfo[curr_ch];
                
                currRx.var.rRs = Program.lst_rsinfo[currRx.iStdChan - 1].dTValue;
                stm = 5;
            }


            if (stm == 5) //set nav range to 1v
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");
                DeviceMgr.Action("navto1v", 0);
                is1V = true;
                stm = 10;
                return;
            }
            if (stm == 10)//prepare the capture with 1:1 mode
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");

                if(currRx.iVMode == 0)
                {
                    if (currRx.iIx == 3) //0.3A
                    {
                        currRx.var.iK = 100;
                    }
                    if (currRx.iIx == 4) //1A
                    {
                        currRx.var.iK = 10;
                    }
                    if (currRx.iIx == 5) //5A
                    {
                        currRx.var.iK = 1;
                    }
                    delay = 0;
                    stm = 20;
                    return;
                }
                else if (currRx.iVMode == 1)
                {
                    DeviceMgr.Action("turnk", Convert.ToUInt32(RX_VAR.INIT_LOOP)); //1:1 mode
                    currRx.var.iK = RX_VAR.INIT_LOOP;
                    delay = 0;
                    stm = 20;
                    return;
                }else if((currRx.iRRange == 6) && (!currRx.bSqrt)) //1K ohm, no x2 current
                {
                    DeviceMgr.Action("turnk", Convert.ToUInt32(RX_VAR.INIT_LOOP)); //1:1 mode
                    currRx.var.iK = RX_VAR.INIT_LOOP;
                    delay = 0;
                    stm = 20;
                    return;
                }
                else if (currRx.iVMode == 2 || currRx.iVMode == 3) //10k high mode
                {
                    DeviceMgr.Action("turnk", Convert.ToUInt32(RX_VAR.INIT_LOOP)); //1:1 mode
                    currRx.var.iK = RX_VAR.INIT_LOOP;
                    currRx.var.rRx = currRx.var.rRs;
                    if(currRx.iRRange == 6)
                        currRx.var.iSrc = RunWnd.syscfg.d1Kcurr; //10mA
                    else if (currRx.iRRange == 7)
                        currRx.var.iSrc = RunWnd.syscfg.d10Kcurr; //1mA
                    else
                        currRx.var.iSrc = RunWnd.syscfg.d100Kcurr; //0.1mA
                    if (currRx.bSqrt)
                        currRx.var.iSrc = currRx.var.iSrc * 2;
                    stm = 45; //prepare tracking
                    return;
                }

                currRx.i_State = RUN_STATE.STOPPING;
                stm = 0;
                return;
            }
            if (stm == 20) //capture once //set to KTTP mode and delay 1s
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");

                DeviceMgr.Action("MEAS_RS|KTTP|" + ComposeAction(),0);
                delay = 5*sec_cnt; //1s delay
                stm = 25;
                return;
            }
            if (stm == 25) //get vrs reading
            {
                currRx.i_State = RUN_STATE.READING;
                StatusChanged(this, "search");

                StartCollectReading(true);
                stm = 27;
                return;
            }
            if (stm == 27) 
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");
                currRx.var.vRs = dStableReading;
                DeviceMgr.Action("MEAS_RS|KTTN|" + ComposeAction(), 0);
                delay = 5 * sec_cnt; //1s delay
                stm = 28;
                return;
            }
            if (stm == 28) //get vrs reading
            {
                currRx.i_State = RUN_STATE.READING;
                StatusChanged(this, "reading");

                StartCollectReading(true);
                stm = 30;
                return;
            }
            if (stm == 30)
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");

                if(currRx.var.vRs < 0)
                    currRx.var.vRs = (currRx.var.vRs - dStableReading) / 2;
                else
                    currRx.var.vRs = (currRx.var.vRs - dStableReading) / 2;
                if (Math.Abs(currRx.var.vRs) > 2)
                {
                    currRx.var.rRx = -9999;
                    currRx.var.errMsg = StringResource.str("big_rs_volt");
                    stm = 1000;
                    return;
                }
                if (currRx.var.rRs < 1e-6)
                    currRx.var.rRs = 1;
                currRx.var.iSrc = Math.Abs(currRx.var.vRs / currRx.var.rRs);
                if (currRx.var.iSrc < 1e-4) //too small current
                    currRx.var.iSrc = 1e-3; //1mA
                
                if (currRx.iVMode == 0)
                {
                    currRx.var.rRx = currRx.var.rRs;
                    stm = 45;
                        return;
                }
                DeviceMgr.Action("MEAS_RX|KTTP|" + ComposeAction(), 0);
                delay = 5*sec_cnt; //1s delay
                stm = 35;
                return;
            }

            if (stm == 35) //get -rx reading
            {
                currRx.i_State = RUN_STATE.READING;
                StatusChanged(this, "reading");

                StartCollectReading(true);
                stm = 37;
                return;
            }

            if (stm == 37) 
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");
                currRx.var.vRx = Math.Abs(dStableReading);
                DeviceMgr.Action("MEAS_RX|KTTN|" + ComposeAction(), 0);
                delay = 5 * sec_cnt; //1s delay
                stm = 38;
                return;
            }
            if (stm == 38) ////get -vrx reading
            {
                currRx.i_State = RUN_STATE.READING;
                StatusChanged(this, "reading");
                StartCollectReading(true);
                stm = 40;
                return;
            }

            if (stm == 40) //update k
            {
                currRx.i_State = RUN_STATE.SEARCHING;
                StatusChanged(this, "search");

                currRx.var.vRx = (currRx.var.vRx+Math.Abs(dStableReading))/2;

                if (Math.Abs(currRx.var.vRx) > 2)
                {
                    currRx.var.rRx = -9999;
                    currRx.var.errMsg = StringResource.str("big_rx_volt");
                    stm = 1000;
                    return;
                }
                currRx.var.rRx = currRx.var.vRx / currRx.var.iSrc;
                if (currRx.iVMode == 1) //10V mode
                {
                    currRx.var.iK = currRx.var.iK + currRx.var.calc_capture_nextk();
                    DeviceMgr.Action("turnk", Convert.ToUInt32(currRx.var.iK));
                }
                stm = 45;
                return;
            }
            if (stm == 45) //prepare track
            {
                currRx.i_State = RUN_STATE.TRACKING;
                StatusChanged(this, "tracking");

                DeviceMgr.Action("MEAS_DELTA|KTTP|" + ComposeAction(), 0);
                delay = 2*sec_cnt; //1s
                stm = 50;
                return;
            }
            if (stm == 50) //switch nav to 120mV
            {
                currRx.i_State = RUN_STATE.TRACKING;
                StatusChanged(this, "tracking");
                if (currRx.iRRange == 8) //100K
                {
                    stm = 60;
                }
                else
                {
                    DeviceMgr.Action("navto120mv", 0);
                    is1V = false;
                    delay = 2 * sec_cnt;
                    stm = 60;
                    return;
                }
            }
            if (stm == 60)  //track once
            {
                currRx.i_State = RUN_STATE.TRACKING;
                StatusChanged(this, "tracking");

                DeviceMgr.Action("MEAS_DELTA|KTTP|" + ComposeAction(), 0);

                if (RunWnd.syscfg.iKTT > 0)
                {
                    delay = RunWnd.syscfg.iKTT * sec_cnt; //0.5s
                    stm = 65;
                }
                else
                {
                    stm = 75;
                }
                return;
            }
            if (stm == 65) //get vcrossp reading
            {
                currRx.i_State = RUN_STATE.READING;
                StatusChanged(this, "tracking");

                StartCollectReading(false);
                stm = 70;
                return;
            }
            if (stm == 70) //get vcrossp and turn kttn
            {
                currRx.i_State = RUN_STATE.TRACKING;
                StatusChanged(this, "tracking");

                if (!is1V)
                    currRx.var.vCrossP = dStableReading / 1000;  //convert to V
                else
                    currRx.var.vCrossP = dStableReading;
                if ((!is1V && Math.Abs(currRx.var.vCrossP) > 0.076) || Math.Abs(currRx.var.vCrossP) > 2) //76mV for maximum
                {
                    currRx.var.rRx = -9999;
                    currRx.var.errMsg = StringResource.str("big_vcrossp_volt");
                    stm = 1000;
                    return;
                }
                DeviceMgr.Action("MEAS_DELTA|KTTN|" + ComposeAction(), 0);
                delay = RunWnd.syscfg.iKTT * sec_cnt; 
                stm = 75;
                return;
            }
            if (stm == 75) //get vcrossn reading
            {
                currRx.i_State = RUN_STATE.READING;
                StatusChanged(this, "tracking");

                StartCollectReading(false);
                stm = 80;
                return;
            }
            if (stm == 80)
            {
                currRx.i_State = RUN_STATE.TRACKING;
                StatusChanged(this, "tracking");
                if(!is1V)
                    currRx.var.vCrossN = dStableReading / 1000;
                else
                    currRx.var.vCrossN = dStableReading;
                if ((!is1V && Math.Abs(currRx.var.vCrossN) > 0.076) || Math.Abs(currRx.var.vCrossN) > 2) //76mV for maximum
                {
                    currRx.var.rRx = -9999;
                    currRx.var.errMsg = StringResource.str("big_vcrossn_volt");
                    stm = 1000;
                    return;
                }
                if (RunWnd.syscfg.iKTT <= 0)
                {
                    currRx.var.vCrossP = currRx.var.vCrossN;
                    currRx.var.vCrossN = -currRx.var.vCrossN;
                }
                int newN;
                if (currRx.iVMode == 0) //fix loop mode
                {
                    newN = currRx.var.calc_track_nextk();
                    if (ValidNewValue(ref currRx.var.rRx))
                        StatusChanged(this, "newvalue");
                    else
                        iMeasCnt--;
                    delay = RunWnd.syscfg.iMeasDelay * sec_cnt;
                    if (iMeasCnt == RunWnd.syscfg.iMeasTimes)
                    {
                        stm = 90;
                    }
                    else
                    {
                        stm = 60;
                    }
                    iMeasCnt++;
                    return;
                }
                if (currRx.iVMode == 1 || currRx.iVMode == 2 || currRx.iVMode == 3) //Hi mode
                {
                    newN = currRx.var.calc_track_nextk();
                    if (Math.Abs(newN) > 64) //too big changes
                    {
                        stm = 5; //redo capture
                        return;
                    }
                    if (Math.Abs(newN) > 0)
                    {
                        if(newN > 0)
                            currRx.var.iK = currRx.var.iK + (newN+1)/2;
                        else
                            currRx.var.iK = currRx.var.iK - (newN+1)/2;
                        delay = 1 * sec_cnt;
                    }
                    stm = 60;
                    if (newN != 0)
                        DeviceMgr.Action("turnk", Convert.ToUInt32(currRx.var.iK));
                if (newN == 0)
                    {
                        if (ValidNewValue(ref currRx.var.rRx))
                            StatusChanged(this, "newvalue");
                        else
                            iMeasCnt--;
                        delay = RunWnd.syscfg.iMeasDelay * sec_cnt;
                        if (iMeasCnt == RunWnd.syscfg.iMeasTimes)
                        {
                            stm = 90;
                        }
                        iMeasCnt++;
                    }
                    return;
                }
            }
            if (stm == 90) //update the display and try the next channel
            {
                currRx.i_State = RUN_STATE.IDLE;
                bRunning = false;
                StatusChanged(this, "complete");
                stm = 1100; //reset to status
//                stm = 0; return;
                
            }
            if (stm == 1000)
            {
                currRx.i_State = RUN_STATE.ERROR;
                StatusChanged(this, "stopping");
                stm = 1100;
                return;
            }
            if (stm == 1100)
            {
                DeviceMgr.Action("navto1v",0);
                is1V = true;
                while (DeviceMgr.IsInAction())
                    ;
                currRx.var.iK = RX_VAR.INIT_LOOP;
                DeviceMgr.Reset();
                while (DeviceMgr.IsInAction())
                    ;
                stm = 0;
                return;
            }
        }
        
        public void Stop()
        {
            if (bRunning)
            {
                currRx.i_State = RUN_STATE.STOPPING;
            }
        }
    }

    internal class ActMessage
    {
        public string action;
        public byte addr;
        public UInt32 value;
    }
    /*
     1,2,4,8,16,32,64,128,256,512,1024,2048,4096  --- 12
B -- 4
ohm 1, -1,-2,-3 --- 4
A 0.1,0.3,10mA ,1A,5A-- 4 , ALL OFF IS 1MA
curr2, ktt, rs/rx, dvm  --  4
3V,10,30V -- 3
00/09(ON),J6 -- 1024
01/08(ON), J4 --- 256
02/0B(ON), J0 -- 1 ohm
03/0A -N/A
04(ON)/0D, J10 -- BACKUP 3 -- 0.3A
05(ON)/0C, J11 -- BACKUP 4  --1A
06/0F(ON), J8 -- BACKUP 1 -- overlap - KTT,
07/0E(ON), J9 -- BACKUP 2 -- 5A
10/19(ON), 	J7 --- 2048
11/18(ON) ---- 8
12/1B(ON), J5 ---512
13/1A(ON) ---- 32
14/1D(ON) ---- x2 (big) -- 100mA
15/1C - N/A
16/1F(ON) ---- N/A
1E/17(ON)  -- 16
20/29(ON) --- N/A
21/28(ON),  J2 --- 10^-2 ohm
22/2B(ON) ----- 1
23/2A(ON) ----  CURR x 2 (small) --0.1mA 
24/2D(ON),  J14 --- GAOZU
25/2C(ON), J15 --- 4096
26/2F(ON),	J12 -- RS/RX
27/2E(ON),  J13 -- DVM ON/OFF
30/39(ON) --- 2
31/38(ON)  -- 4
32/3B(ON),  J3 --10^-3 ohm
33/3A(ON), J1, --- 10^-1 ohm
3D(ON)/34,   J19            --- 1 A / 100mA
3C/35(ON), J17            -- 0.3A / 10mA
3F/36(ON),   J18	      --- 1mA (9606)
3E/37(ON), J16  -- 0.1mA - (9606)
40/41(ON)  -- 128
42/43(ON)  -- 64
*/
    internal class DeviceMgr
    {
        static public AutoResetEvent WaitEvent; //flag for wait response, when flag is on,node agent can process the command else node agent cannot do that
        static public AutoResetEvent OverEvent; //flag for process done event.
        static public object agent_access = new object(); //lock for agent access
        static public ActMessage actmsg;
        static private Thread msg_loop;
        static public SerialPort port;  //all navmeter and relay will share the same port
        static public SerialPort cmdport; //all command from PC will use the port
        static public void Dummy()
        {
            
        }
        static public double reading;
        static public bool done;
        static private StringBuilder inbuffer;
        static private StringBuilder cmdbuffer;
        static DeviceMgr()
        {
            bool bDebugGPIB = true;
            Thread.Sleep(3000);
            WaitEvent = new AutoResetEvent(false);
            OverEvent = new AutoResetEvent(false);
            inbuffer = new StringBuilder();
            cmdbuffer = new StringBuilder();
            port = new SerialPort();
            
            #region init port
            port.BaudRate = 9600;
            if (!bDebugGPIB)
                port.PortName = "COM10";
            else
                port.PortName = "COM11";
            port.Parity = Parity.None;
            port.DataBits = 8;
            port.StopBits = StopBits.One;
            port.NewLine = "\r";
            int i;
            for (i = 0; i < 10; i++)
            {
                if (bDebugGPIB)
	                break;
                port.Open();
                if (port.IsOpen)
                    break;
                Thread.Sleep(3000);
            }
            if(i >= 10)
                throw new Exception("Failed to open port A");
            #endregion 
          /*   
            
            #region init cmdport
            cmdport = new SerialPort();
            cmdport.BaudRate = 9600;
            if(!bDebugGPIB)
                cmdport.PortName = "COM11";
            else
                cmdport.PortName = "COM10";
            cmdport.Parity = Parity.None;
            cmdport.DataBits = 8;
            cmdport.StopBits = StopBits.One;
            cmdport.NewLine = "\r";
            cmdport.Open();
            #endregion
            if (!cmdport.IsOpen)
            {
                port.Close();
                throw new Exception("Failed to open port B");
            }
            cmdport.DiscardInBuffer();
            cmdport.DataReceived += new SerialDataReceivedEventHandler(cmdport_DataReceived);
            */ 
            agent_access = false;
            actmsg = new ActMessage();
            msg_loop = new Thread(new ThreadStart(MessageHandler));
            msg_loop.IsBackground = false;
            if(bDebugGPIB)
                return;
            msg_loop.Start();
        }

        static public string GetLogFileName()
        {
            string file = @"C:\\"+DateTime.Now.ToString("yyyy_MM_dd")+".txt";
            if (!File.Exists(file))
            {
                try
                {
                    FileStream fsLog = new FileStream(file, FileMode.Create, FileAccess.Write, FileShare.Read);
                    StreamWriter sw = new StreamWriter(fsLog);
                    sw.Close();
                    fsLog.Close();
                }
                catch (System.Exception ex)
                {
                }
            }
            return file;
        }
        static public void Log(string line)
        {
            string fn = GetLogFileName();
            try
            {
                FileStream fsLog = new FileStream(fn, FileMode.Append, FileAccess.Write, FileShare.Read);
                using (StreamWriter sw = new StreamWriter(fsLog))
                {
                    sw.WriteLine(DateTime.Now.ToString("yyyy-MM-dd HH:mm:ss.fff") + "\t" + line);
                }
                fsLog.Close();
            }
            catch (System.Exception ex)
            {
            }
        }
        static public void ReportHeader(int total)
        {
            string reply;
            cmdport.Write(cmdport.NewLine);
            cmdport.Write(cmdport.NewLine);
            cmdport.Write(cmdport.NewLine);
            reply = String.Format("D{0} Measurements:", total);
            cmdport.Write(reply);
            cmdport.Write(cmdport.NewLine);
            //cmdport.WriteLine(reply);
            reply = String.Format("R{0}", Program.lst_rxinfo[0].dRxInput.ToString("E13"));
            cmdport.WriteLine(reply);
            reply = String.Format("S{0}", Program.lst_rsinfo[0].dTValue.ToString("E13"));
            cmdport.WriteLine(reply);

        }
        static public void ReportData(int index, double value)
        {

            string reply;
            reply = String.Format("#{0}",index.ToString());
            cmdport.WriteLine(reply);
            reply = String.Format("&{0}",value.ToString("E13"));
            cmdport.WriteLine(reply);
            
        }
        static void cmdport_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            while (cmdport.BytesToRead > 0)
            {
                char data = Convert.ToChar(cmdport.ReadChar());
                if ((data == '\r' || data == '\n') && (cmdbuffer.Length > 0))
                {
                    string cmd = cmdbuffer.ToString();
                    Program.mainwnd.Invoke(new Action<string>(Program.mainwnd.pc_cmd), new object[] { cmd });
                    cmdbuffer.Remove(0, cmdbuffer.Length);
                    
                }
                else
                {
                    if(data != '\r' && data != '\n')
                    cmdbuffer.Append(data);
                }
            }
        }
        static public void ScanPort()
        {

            
            while (port.BytesToRead > 0)
            {
                char data = Convert.ToChar(port.ReadChar());
                if (((data >= '0') && (data <= '9')) ||
                (data == '.') || (data == '-') || (data == '+') || (data == 'e') || (data == 'E'))
                {
                    inbuffer.Append(Convert.ToChar(data));
                }
                else
                {
                    if (inbuffer.Length >= 4)
                    {
                        try
                        {
                            reading = Double.Parse(inbuffer.ToString());
                        }
                        catch
                        {
                            reading = -9999;
                        }
                    }
                    if(inbuffer.Length > 0)
                    inbuffer.Remove(0, inbuffer.Length);
                }
            }
        }
        static public bool IsInAction()
        {
            if(actmsg.action == "" || actmsg.action == null)
                return false;
            if (!OverEvent.WaitOne(30,false))
                return true;
            OverEvent.Reset();
            if ((actmsg.action != "retry") && (actmsg.action != "fail"))
            {
                actmsg.action = "";
                return false;
            }
            if (actmsg.action == "retry")
            {
                throw new Exception(StringResource.str("tryagain"));
            }
            if (actmsg.action == "fail")
            {
                throw new Exception("Critical Action failed.");
            }
            return false;
        }
        static public void Action(string cmd, UInt32 param)
        {
            
            actmsg.action = cmd;
            actmsg.addr = 0;
            actmsg.value = param;
            WaitEvent.Set();
//            OverEvent.WaitOne();
        }
        //k1 = RS/RX, DVM
        //ktt = ON/OFF
        //curr = CURR_P001|CURR_P01/P1/P3/1/5 ; 1mA/10mA,0.1A,0.3A,1A,5A
        static private string oldswi = ""; //old measurement switch satus
        static private string oldktt = "";
        static private string oldcurr = "";
        static private string oldsqr = "";
        static private UInt32 oldk = 0;
        static private string oldconstv = "";
        static private string oldstd = "";
        static public string[] constvtbl = new string[] { "", "VMODE_3V", "VMODE_10V", "VMODE_30V","VMODE_10VHIGH","VMODE_30VHIGH" };
        static public string[] stdtbl = new string[] { "",  "STD_P001", "STD_P01", "STD_P1","STD_1" };
        static public string[] meastbl = new string[] { "", "MEAS_RS","MEAS_RX","MEAS_DELTA" };
        static public string[] currtbl = new string[] { "", "CURR_OFF", "CURR_P0001","CURR_P001", "CURR_P01", "CURR_P1", "CURR_P3", "CURR_1", "CURR_5"};
        static private byte[] UsHead = new byte[] { Convert.ToByte('U'), Convert.ToByte('s'), Convert.ToByte('U'), Convert.ToByte('U') };
        static private byte[] UsTail = new byte[] { Convert.ToByte('V')};
        static private void DelayWrite(byte[] buf, int start, int len)
        {
            
            for (int i = 0; i < len; i++)
            {
                port.Write(buf,i,1);
                Thread.Sleep(30 * len);
            }
        }
        static public void KState(UInt32 k)
        {
            if (oldk == k)
                return;
            DelayWrite(UsHead, 0, 4);
            if( k % 2 == 0) //1
                DelayWrite(new byte[] { 0x2B }, 0, 1);
            else
                DelayWrite(new byte[] { 0x22 }, 0, 1);
            k = k / 2;
            if (k % 2 == 0) //2
                DelayWrite(new byte[] { 0x39 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x30 }, 0, 1);
            k = k / 2;
            if (k % 2 == 0) //4
                DelayWrite(new byte[] { 0x38 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x31 }, 0, 1);
            k = k / 2;      //8
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x18 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x11 }, 0, 1);
            k = k / 2;      //16
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x17 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x1E }, 0, 1);
            k = k / 2;      //32
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x1A }, 0, 1);
            else
                DelayWrite(new byte[] { 0x13 }, 0, 1);
            k = k / 2;      //64
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x43 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x42 }, 0, 1);
            k = k / 2;      //128
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x41 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x40 }, 0, 1);
            k = k / 2;      //256
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x08 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x01 }, 0, 1);
            k = k / 2;      //512
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x1B }, 0, 1);
            else
                DelayWrite(new byte[] { 0x12 }, 0, 1);
            k = k / 2;      //1024
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x09 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x00 }, 0, 1);
            k = k / 2;      //2048
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x19 }, 0, 1);
            else
                DelayWrite(new byte[] { 0x10 }, 0, 1);
            k = k / 2;      //4096
            if (k % 2 == 0)
                DelayWrite(new byte[] { 0x2C }, 0, 1);
            else
                DelayWrite(new byte[] { 0x25 }, 0, 1);
            
            DelayWrite(UsTail, 0, 1);
        }
        static public void Reset()
        {
            KState(Convert.ToUInt32(65536)); //clear all
            RelayState("MEAS_RS", "KTTP", "CURR_OFF", "X1", "STD_1", "VMODE_3V");
        }
        static public void RelayState(string swi, string ktt, string curr, string sqr, string std, string constv)
        {
            if (((swi == oldswi) || (swi == "")) && 
                ((ktt == oldktt) || (ktt == "")) &&
                ((curr == oldcurr) || (curr == "")) &&
                ((sqr == oldsqr) || (sqr == "")) &&
                ((std == oldstd) || (std == "")) &&
                ((constv == oldconstv) || (constv == "")))
                return;
                
            DelayWrite(UsHead, 0, 4);
            if (swi.StartsWith("MEAS_"))
            {
                if (swi == "MEAS_RS")
                {
                    DelayWrite(new byte[] { 0x26, 0x27 }, 0, 2);
                    std = "STD_OFF";
                }
                if (swi == "MEAS_RX")
                {
                    DelayWrite(new byte[] { 0x2F, 0x27 }, 0, 2);
                }
                if (swi == "MEAS_DELTA")
                {
                    DelayWrite(new byte[] { 0x2F, 0x2E }, 0, 2);
                    //std = "STD_OFF";
                }
                oldswi = swi;
            }
            if (ktt.StartsWith("KTT"))
            {
                if(ktt == "KTTN")
                    DelayWrite(new byte[] { 0x06 }, 0, 1);
                if (ktt == "KTTP")
                    DelayWrite(new byte[] { 0x0F }, 0, 1);
                oldktt = ktt;
            }
            if (sqr.StartsWith("X"))
            {
                if ((curr == "CURR_P3") || (curr == "CURR_1") || (curr == "CURR_5"))
                {
                    DelayWrite(new byte[] { 0x24 }, 0, 1);
                    if (sqr == "X2")
                        DelayWrite(new byte[] { 0x2A }, 0, 1); //todo?
                    if (sqr == "X1")
                        DelayWrite(new byte[] { 0x23 }, 0, 1); //todo?
                }
                else
                {
                    DelayWrite(new byte[] { 0x2D }, 0, 1);
                    if (sqr == "X2")
                        DelayWrite(new byte[] { 0x1D }, 0, 1);
                    if (sqr == "X1")
                        DelayWrite(new byte[] { 0x14 }, 0, 1);
                }
                oldsqr = sqr;
            }
            if (curr.StartsWith("CURR_"))
            {
                byte[] cb = new byte[] { 0x3E, 0x3F, 0x3C, 0x34, 0x0D, 0x0C, 0x0E }; //1mA
                if (curr == "CURR_P0001") cb[0] = 0x37; //0.1mA
                if (curr == "CURR_P001") cb[1] = 0x36; //1mA
                if (curr == "CURR_P01") cb[2] = 0x35; //10mA
                if (curr == "CURR_P1") cb[3] = 0x3d; //0.1A
                if (curr == "CURR_P3") cb[4] = 0x04; //0.3A
                if (curr == "CURR_1") cb[5] = 0x05; //1A
                if (curr == "CURR_5") cb[6] = 0x07; //5A
                DelayWrite(cb, 0, 7);
                //if (oldcurr == "CURR_P001") cb[0] = 0xff;//turn off all is 1mA
                //                    if (oldcurr == "CURR_P01") cb[0] = 0x3E;
                //                  if (oldcurr == "CURR_P1") cb[0] = 0x3F;
                //                if (oldcurr == "CURR_P3") cb[0] = 0x3C;
                //              if (oldcurr == "CURR_1") cb[0] = 0x3D;
                //            if (oldcurr == "CURR_5") cb[0] = 0x0C;

                //          DelayWrite(cb, 0, 1);
                //}
                //                oldcurr = curr;
            }
            if (std.StartsWith("STD_"))
            {
                if (std == "STD_OFF")
                    DelayWrite(new byte[] { 0x02, 0x33, 0x21, 0x32 }, 0, 4);
                if (std == "STD_1")
                    DelayWrite(new byte[] { 0x0B, 0x33, 0x21, 0x32 }, 0, 4);
                if (std == "STD_P1")
                    DelayWrite(new byte[] { 0x3A, 0x02, 0x21, 0x32 }, 0, 4);
                if (std == "STD_P01")
                    DelayWrite(new byte[] { 0x28, 0x02, 0x33, 0x32 }, 0, 4);
                if (std == "STD_P001")
                    DelayWrite(new byte[] { 0x3B, 0x02, 0x33, 0x21,  }, 0, 4);
            }
            DelayWrite(UsTail, 0, 1);
        }
        static public bool Loaded = false;
        static public void MessageHandler()
        {
            Reset();
            Loaded = true;
            while (true)
            {
                if (!WaitEvent.WaitOne(10, false))
                {
                    ScanPort();
                    continue;
                }
                WaitEvent.Reset();
                try
                {
                    #region navmeter action
                    ActMessage msg = DeviceMgr.actmsg;
                    if (msg.action == "navto1v" || msg.action == "navto120mv" || msg.action == "navto10mv")
                    {
                        if (msg.action == "navto1v")
                        {
//                            port.Write(NAV_INIT); //init
                            port.Write(StringResource.str("NAV_1V_"+RunWnd.syscfg.sNavmeter)); //1v
//                            DelayWrite(NAV_AFLTON); //slowmode and aflton
                        }
                        if (msg.action == "navto120mv")
                        {
                            port.Write(StringResource.str("NAV_120MV_" + RunWnd.syscfg.sNavmeter)); //120mv
                        }
                        if (msg.action == "navto10mv")
                        {
                            port.Write(StringResource.str("NAV_10MV_" + RunWnd.syscfg.sNavmeter)); //10mv
                        }

                        Thread.Sleep(3000);
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    if (msg.action == "navread")
                    {
                        reading = -9999;
                        port.DiscardInBuffer();
                        port.Write(StringResource.str("NAV_READ_" + RunWnd.syscfg.sNavmeter)); //1v
                        Thread.Sleep(5);
                        int timeout = 400;
                        while ((timeout-- > 0) && (reading < -9000))
                        {
                            ScanPort();
                            Thread.Sleep(5);
                        }
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    if (msg.action == "turnk")
                    {
                        KState(msg.value);
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    if (msg.action.StartsWith("MEAS")) 
                    {
                        string[] cmd = msg.action.Split(new char[]{'|'});
                        if (cmd.Length == 6)
                        {
                            RelayState(cmd[0],cmd[1], cmd[2], cmd[3], cmd[4], cmd[5]);
                        }
                        actmsg.action = "done";
                        OverEvent.Set();
                        continue;
                    }
                    #endregion
                    throw new Exception("Invalide command " + actmsg.action);
                }
                catch (Exception ex)
                {
                    Debug.WriteLine(ex.Message);
                    actmsg.action = "fail";
                    OverEvent.Set();
                }
            }
        }
    }

    internal class DataMgr
    {
        public List<PointPair> PointList;
        public DataMgr()
        {
     
        }
        public void ClearAll()
        {
            PointList.Clear();
        }
        public void AddValue(string chan, int index, double val, double ratio, double temp)
        {
            PointList.Add(new PointPair(chan, index, val, ratio, temp, false, DateTime.Now));
        }
        
        public IEnumerable<PointPair> my_list(string chan)
        {
            return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded));
        }
        #region data summary
        public int validrows(string chan)
        {
            return PointList.Count(x => (x.chan == chan || chan == "") && (!x.excluded));
        }
        public double my_last(string chan){
            try
            {
                return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Last().value;
            }
            catch
            {
                return 0;
            }
        }
        public double my_min(string chan)
        {
            try
            {
                return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Min().value;
            }
            catch
            {
                return 0;
            }
        }
        public double my_max(string chan)
        {
            try
            {
                return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Max().value;
            }
            catch
            {
                return 0;
            }
        }
        public double my_sqrtsum(string chan)
        {
            try
            {
                if (validrows(chan) < 2)
                    return 0;
                double avg = my_avg(chan);
                double sqr = 0;
                foreach (double v in PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Select(x => x.value))
                {
                    sqr = sqr + v * (v - avg);
                }
                return Math.Sqrt(sqr/ (validrows(chan)-1))/avg;
            }
            catch
            {
                return 0;
            }
        }
        public double my_sum(string chan)
        {
            try
            {
                return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Sum(x => x.value);
            }
            catch
            {
                return 0;
            }
        }
        public double my_avg(string chan)
        {
            try
            {
                return PointList.Where(x => (x.chan == chan || chan == "") && (!x.excluded)).Sum(x => x.value) / validrows(chan);
            }
            catch
            {
                return 0;
            }
        }
        #endregion


    }
    public class PointPair : IComparable
    {
        public int CompareTo(object r)
        {
            if (r is PointPair)
            {
                PointPair pp = r as PointPair;
                return this.value.CompareTo(pp.value);
            }
            else
            {
                return this.ToString().CompareTo(r.ToString());
            }
        }
        public int CompareTo(PointPair r)
        {
            return this.value.CompareTo(r.value);
        }
        public PointPair(string ch,int id, double val, double rt, double tp, bool ex, DateTime st)
        {
            chan = ch;
            index = id;
            rvalue = val;
            ratio = rt;
            temp = tp;
            stamp = st;
            excluded = ex;
        }
        public DateTime stamp; //"HH:mm:ss.fff"
        public int index;
        public string chan;
        public double value{
            get
            {
                if (datamode == RXDATA_MODE.RATIO)
                    return ratio;
                else if (datamode == RXDATA_MODE.RX)
                    return rvalue;
                else
                    return temp;
            }
        }
        public double rvalue;
        public double ratio;
        public double temp;
        public bool excluded;

        public static RXDATA_MODE datamode;
        
    }
}
