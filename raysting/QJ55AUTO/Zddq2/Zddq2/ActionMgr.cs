using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Windows;
using System.Windows.Forms;
using System.Threading;
namespace Zddq2
{
    class DataMgr
    {
        //management of readings
        public int iMeasCnt { get; set; }
        public RsInfo rs { get; set; }
        public RxInfo rx { get; set; }

        private List<double> valfilter;
        public DataMgr()
        {
            valfilter = new List<double>();
        }
        public void Reset()
        {
            valfilter.Clear();
            iMeasCnt = 0;
        }
        //change the new reading base on history
        public bool AddNewValue(ref double newreading)
        {
            int drop = 2;
            if (valfilter.Count == 0) //1st reading
            {
                SysConfig syscfg = RunWnd.syscfg;
                DeviceMgr.Log(String.Format(@"==== 新测量参数========
    测量次数 = {0}
    标准电阻ID = {1}
    标准电阻阻值 = {2}
    标准电阻温度系数. Alpha = {3}
    标准电阻温度系数. Beta = {4}
    温度 = {5}
    被测电阻ID = {6}
    单次采样次数 = {7}
    换向时间(秒) = {8}
    测量间隔(秒) = {9}
    电流值(A) = {10}
    滤波器 = {11}
    新测量开始...", syscfg.iMeasTimes.ToString(), rs.sSerial, rs.dValue.ToString("F8"), rs.dAlpha.ToString("F3"),
                                                                                       rs.dBeta.ToString("F3"),
                                                                                       syscfg.dTemp.ToString("F3"), rx.sSerial, syscfg.iSampleTimes, syscfg.iKTT,
                                                                                       syscfg.iMeasDelay, rx.var.iSrc, syscfg.iFilter
                                                            ));
            }
            valfilter.Add(newreading);
            if (valfilter.Count < 4)
                return false;
            
            if ((RunWnd.syscfg.iFilter + drop) >= valfilter.Count)
            newreading = valfilter.Skip(2).Average();
            else
                newreading = valfilter.Skip(valfilter.Count - RunWnd.syscfg.iFilter).Average();
            iMeasCnt++;
            return true;
        }
    }
    //manager all the next step action and data retrieving
    public class ActionMgr
    {
        public RsInfo rs { get; set; }
        public RxInfo rx { get; set; }

        public const int RNG_P001 = 0;
        public const int RNG_P01 = 1;
        public const int RNG_P1 = 2;
        public const int RNG_1 = 3;
        public const int RNG_10 = 4;
        public const int RNG_100 = 5;
        public const int RNG_1K = 6;
        public const int RNG_10K = 7;
        public const int RNG_100K = 8;
        public const int RNG_INVALID = 999;

        private Dictionary<int, int> history_newk; //new K
        public ActionMgr()
        {
            datafilter = new List<double>();
            history_newk = new Dictionary<int, int>();
        }

        #region delay related property
        public int waiting
        {
            get
            {
                int ret = 0;
                if (delay <= 0)
                    return 0;
                ret = (delay - Convert.ToInt32(DateTime.Now.Subtract(tm_delay_start).TotalSeconds));
                if (ret <= 0)
                {
                    ret = 0;
                    delay = 0; //stop the timer
                }
                return ret;
            }
        }
        public int delay
        {
            get
            {
                return _delay;
            }
            set
            {
                if (value != 0)
                {
                    tm_delay_start = DateTime.Now;
                    _delay = value;
                }
            }
        }
        private DateTime tm_delay_start = DateTime.Now;
        private int _delay = 0;
        #endregion

        #region auto range related property
        public static string nav_range = "";
        public int range_dir //0 means not in adjusting 1 means adjusting up, -1 means adjusting down.
        {
            get
            {
                return _range_dir;
            }
            set
            {
                if (value > 0)
                    _range_dir = 1;
                else if (value < 0)
                    _range_dir = -1;
                else
                    _range_dir = 0;
                if ((_range_dir == 1) && IsTopRange()) //already the top range
                {
                    _range_dir = 0;
                    return;
                }
                if ((_range_dir == -1) && IsBottomRange()) //already the top range
                {
                    _range_dir = 0;
                    return;
                }
                bFirstReadSent = false;
                datafilter.Clear();
                
            }
        }
        private int _range_dir = 0;
        private const string NAV_TYPE_2182 = "2182";
        private bool IsTopRange()
        {
            if (RunWnd.syscfg.sNavmeter == NAV_TYPE_2182 && nav_range == "navto30v")
                return true;
            if (nav_range == "navto1v") //PZ158, PZ2182
                return true;
            return false;
        }
        private bool IsBottomRange()
        {
            if ((RunWnd.syscfg.sNavmeter == NAV_TYPE_2182 || RunWnd.syscfg.sNavmeter == "PZ2182") && nav_range == "navto10mv")
                return true;
            if (nav_range == "navto120mv") //PZ158
                return true;
            return false;
        }
        private bool RangeAdjustDown(double value)
        {
            string newnav = "";
            if (RunWnd.syscfg.sNavmeter == NAV_TYPE_2182)
                newnav = "navto30v";
            else
                newnav = "navto1v";
            if (value < 1) //while < 1V
                newnav = "navto1v";
            if (value < 0.100) //while < 100mV)
                newnav = "navto120mv";
            if ((value < 0.010) && (RunWnd.syscfg.sNavmeter == NAV_TYPE_2182 || RunWnd.syscfg.sNavmeter == "PZ2182")) //while < 10mV
                newnav = "navto10mv";

            if (nav_range != newnav)
            {
                
                nav_range = newnav;
                DeviceMgr.Action(nav_range, 0);
                return true;
            }
            return false;
        }
        private void RangeAdjust()
        {
            
            if (DeviceMgr.IsInAction())
                return;
            if (!bFirstReadSent)
            {
                DeviceMgr.Action("navread", 0);
                bFirstReadSent = true;
                return;
            }
            if(RunWnd.syscfg.sNavmeter != NAV_TYPE_2182 && nav_range != "navto1v")
                datafilter.Add(DeviceMgr.reading/1000);
            else
                datafilter.Add(DeviceMgr.reading);
            if (datafilter.Count < 4)
            {
                DeviceMgr.Action("navread", 0);
                return;
            }
            int total = datafilter.Count(x => Math.Abs(x) < 999);
            datafilter.Clear();
            if (range_dir == 1)
            {
                if (total > 0 || IsTopRange())
                {
                    range_dir = 0;
                    return;
                }
                if (RunWnd.syscfg.sNavmeter == NAV_TYPE_2182)
                {
                    if (nav_range == "navto1v")
                        nav_range = "navto30v";
                    else
                        nav_range = "navto1v";
                }
                else
                {
                    if (nav_range == "navto10mv")
                        nav_range = "navto120mv";
                    else
                        nav_range = "navto1v";

                }
                delay = 3;
                DeviceMgr.Action(nav_range, 0);
                return;
            }
            else
            {
                throw new NotImplementedException("unexpected down range");
            }
        }
        #endregion

        private bool bFirstReadSent = false;
        #region reading related section
        private double _dStableReading = -999;
        public double dStableReading{
            get
            {
                if (_dStableReading > -999)
                {
                    double val = _dStableReading;
                    _dStableReading = -9999;
                    return val;
                }
                return -9999;
            }
            set
            {
                _dStableReading = value;
            }
        }
        private ACTION_STATE laststm;
        private List<double> datafilter;

        private DateTime tm_meas_begin = DateTime.Now;

        private bool bInReading
        {
            get
            {
                return _bInReading;
            }
            set
            {
                _bInReading = value;
                if (_bInReading)
                {
                    tm_meas_begin = DateTime.Now;
                    badReadingCount = 0;
                    datafilter.Clear();
                    bFirstReadSent = false;
                }
            }
        }
        private bool _bInReading = false;

        private int badReadingCount;

        #endregion

        public void Reset()
        {
            datafilter.Clear();
            history_newk.Clear();
            delay = 0;
            range_dir = 0;
        }
        public bool IsBusy
        {
            get
            {
                if (range_dir != 0)
                {
                    RangeAdjust();
                    return true;
                }
                if (waiting > 0)
                    return true;
                if (bInReading)
                {
                    CollectReading();
                    return true;
                }
                return false;
            }
        }
        private int delayKI;
        private int delayKTT;
        public double CalibrateCurr(RxInfo rx)
        {
            double vp, vn;
            DeviceMgr.Action("turnk", rx.var.iK);
            Thread.Sleep(1000);
            while (DeviceMgr.IsInAction())
                ;
            DeviceMgr.Action("MEAS_RS|KTTP|" + DeviceMgr.ComposeAction(rx), 0);
            Reset();
            this.rx = rx;
            this.rs = Program.lst_rsinfo[0];
            Thread.Sleep(20000);
            bInReading = true;
            Thread.Sleep(500);
            while (IsBusy)
            {
                Thread.Sleep(1000);
            }
            vp = DeviceMgr.reading;

            DeviceMgr.Action("MEAS_RS|KTTN|" + DeviceMgr.ComposeAction(rx), 0);
            Thread.Sleep(20000); 
            bInReading = true;
            Thread.Sleep(500);
            while (IsBusy)
            {
                DeviceMgr.IsInAction();
                Thread.Sleep(1000);
            }
            vn = DeviceMgr.reading;
            double val = (Math.Abs((vp - vn) / 2.0)) / rx.var.rRs;
            val = val * RX_VAR.INIT_LOOP / rx.var.iK;
            RunWnd.syscfg.SetCurrent(rx.iIx, rx.bSqrt,val);
            return val;
            
        }
        public ACTION_STATE Do(ACTION_REQUEST action, ACTION_STATE stm)
        {
            laststm = stm;
            if (action == ACTION_REQUEST.INIT_NAV_RANGE)
            {
                return InitNavRange(stm);
            }
            if (action == ACTION_REQUEST.INITKI_FOR_SEARCH)
            {
                delay = 2;
                return InitKI(stm);
            }

            if (action == ACTION_REQUEST.SET_FOR_RS_KTTP)
            {
                DeviceMgr.Action("MEAS_RS|KTTP|" + DeviceMgr.ComposeAction(rx), 0);
                range_dir = 1;
                delay = delayKI;
                bInReading = true;
                return ACTION_STATE.INIT_FOR_RS_KTTN;
            }
            if (action == ACTION_REQUEST.SET_FOR_RS_KTTN)
            {
                DeviceMgr.Action("MEAS_RS|KTTN|" + DeviceMgr.ComposeAction(rx), 0);
                range_dir = 1;
                delay = delayKTT;
                bInReading = true;

                return ACTION_STATE.COMPLETE_ISRC;
            }
            /*
            if (action == ACTION_REQUEST.SET_FOR_RX_KTTP)
            {
                DeviceMgr.Action("MEAS_RX|KTTP|" + DeviceMgr.ComposeAction(rx), 0);
                range_dir = 1;
                delay = delayKI;
                bInReading = true;
                return ACTION_STATE.INIT_FOR_RX_KTTN;
            }
            if (action == ACTION_REQUEST.SET_FOR_RX_KTTN)
            {
                DeviceMgr.Action("MEAS_RX|KTTN|" + DeviceMgr.ComposeAction(rx), 0);
                range_dir = 1;
                delay = delayKTT;
                bInReading = true;
                return ACTION_STATE.INIT_FOR_TRACKP;
            }
             */
            if (action == ACTION_REQUEST.SET_FOR_TRACKP)
            {
                DeviceMgr.Action("MEAS_DELTA|KTTP|" + DeviceMgr.ComposeAction(rx), 0);
                delay = RunWnd.syscfg.iKTT; //adjust delay
                range_dir = 1;
                bInReading = true;
                return ACTION_STATE.INIT_FOR_TRACKN;
            }
            if (action == ACTION_REQUEST.SET_FOR_TRACKN)
            {
                if (RangeAdjustDown(Math.Abs(rx.var.vCrossP)))
                {
                    range_dir = 1;
                    delay = 2;
                    bInReading = false;
                    return ACTION_STATE.INIT_FOR_TRACKP;
                }

                DeviceMgr.Action("MEAS_DELTA|KTTN|" + DeviceMgr.ComposeAction(rx), 0);
                delay = RunWnd.syscfg.iKTT; //adjust delay
                range_dir = 1;
                bInReading = true;
                return ACTION_STATE.UPDATE_K_TRACK;
            }
            if (action == ACTION_REQUEST.SET_NEXT_K)
            {
                /*
                if (RangeAdjustDown(Math.Max(Math.Abs(rx.var.vCrossP), Math.Abs(rx.var.vCrossN))))
                {
                    range_dir = 1;
                    delay = 2;
                    bInReading = false;
                    return ACTION_STATE.INIT_FOR_TRACKP;
                }
                */
                if ((rx.iRRange == RNG_P1) || (rx.iRRange == RNG_P01) || (rx.iRRange == RNG_P001))  //P1 ohm, P01 ohm, P001 ohm
                {
                    rx.var.calc_track_nextk(rx.iRRange);
                    return ACTION_STATE.SHOW_NEW_VALUE;
                }
                int newN = rx.var.calc_track_nextk(rx.iRRange) - rx.var.iK;
                if (newN == 0)
                {
                    return ACTION_STATE.SHOW_NEW_VALUE;
                }
                if ((!history_newk.ContainsKey(rx.var.iK)) || (history_newk[rx.var.iK] != newN)) //a different decision
                {
                    history_newk[rx.var.iK] = newN;
                    rx.var.iK += newN;
                    DeviceMgr.Action("turnk", rx.var.iK);
                    return ACTION_STATE.INIT_FOR_TRACKP;
                }
                if (history_newk.ContainsKey(rx.var.iK + newN)) //it has an old decision
                {
                    int oldnewN = history_newk[rx.var.iK + newN];
                    if ((oldnewN + newN) == 0)   //just jump up and down
                    {
                        return ACTION_STATE.SHOW_NEW_VALUE;
                    }
                    rx.var.iK += newN;
                    DeviceMgr.Action("turnk", rx.var.iK);
                    return ACTION_STATE.INIT_FOR_TRACKP;
                }
                return ACTION_STATE.SHOW_NEW_VALUE;
            }
            return ACTION_STATE.IDLE;
        }
        private void SetDelays()
        {
            if (rx.iRRange == RNG_100K)
            {
                delayKI = 12; delayKTT = 12;
            }
            else if (rx.iRRange == RNG_10K)
            {
                delayKI = 12; delayKTT = 12;
            }
            else if (rx.iRRange == RNG_1K)
            {
                delayKI = 12; delayKTT = 12;
            }
            else if (rx.iRRange == RNG_100)
            {
                delayKI = 12; delayKTT = 12;
            }
            else if (rx.iRRange == RNG_10)
            {
                delayKI = 12; delayKTT = 12;
            }
            else if (rx.iRRange == RNG_1)
            {
                delayKI = 12; delayKTT = 12;
            }
            else if (rx.iRRange == RNG_P1)
            {
                delayKI = 12; delayKTT = 12;
            }
            else if (rx.iRRange == RNG_P01)
            {
                delayKI = 12; delayKTT = 12;
            }
            else if (rx.iRRange == RNG_P001)
            {
                delayKI = 12; delayKTT = 12;
            }
        }
        public static void SetIxRange(int selectedRx, double rname)
        {
            if (rname > 10240) //100k, 0.01mA
            {
                Program.lst_rxinfo[selectedRx].iIx = -1;
                Program.lst_rxinfo[selectedRx].iRRange = ActionMgr.RNG_100K;
            }
            else if (rname > 1024) //10k, 0.1mA
            {
                Program.lst_rxinfo[selectedRx].iIx = 0;
                Program.lst_rxinfo[selectedRx].iRRange = ActionMgr.RNG_10K;
            }
            else if (rname > 102.4) //1k, 1mA
            {
                Program.lst_rxinfo[selectedRx].iIx = 0;
                Program.lst_rxinfo[selectedRx].iRRange = ActionMgr.RNG_1K;
            }
            else if (rname > 10.24) //100, 10mA
            {
                Program.lst_rxinfo[selectedRx].iIx = 1;
                Program.lst_rxinfo[selectedRx].iRRange = ActionMgr.RNG_100;
            }
            else if (rname > 1.024) //10, 10mA
            {
                Program.lst_rxinfo[selectedRx].iIx = 1;
                Program.lst_rxinfo[selectedRx].iRRange = ActionMgr.RNG_10;
            }
            else if (rname > 0.1024) //1, 100mA
            {
                Program.lst_rxinfo[selectedRx].iIx = 2;
                Program.lst_rxinfo[selectedRx].iRRange = ActionMgr.RNG_1;
            }
            else if (rname > 0.05) //0.1, 0.3A
            {
                Program.lst_rxinfo[selectedRx].iIx = 3;
                Program.lst_rxinfo[selectedRx].iRRange = ActionMgr.RNG_P1;
            }
            else if (rname > 0.005) //0.01, 1A
            {
                Program.lst_rxinfo[selectedRx].iIx = 4;
                Program.lst_rxinfo[selectedRx].iRRange = ActionMgr.RNG_P01;
            }
            else  //0.001, 5A
            {
                Program.lst_rxinfo[selectedRx].iIx = 5;
                Program.lst_rxinfo[selectedRx].iRRange = ActionMgr.RNG_P001;
            }
        }
        public static int ConvertToRange(string param)
        {
            if (param == "100000")
                return ActionMgr.RNG_100K;
            if (param == "10000")
                return ActionMgr.RNG_10K;
            if (param == "1000")
                return ActionMgr.RNG_1K;
            if (param == "100")
                return ActionMgr.RNG_100;
            if (param == "10")
                return ActionMgr.RNG_10;
            if (param == "1")
                return ActionMgr.RNG_1;
            if (param == "0.1")
                return ActionMgr.RNG_P1;
            if (param == "0.01")
                return ActionMgr.RNG_P01;
            if (param == "0.001")
                return ActionMgr.RNG_P001;
            return ActionMgr.RNG_INVALID;
        }
        private ACTION_STATE InitNavRange(ACTION_STATE stm)
        {
            nav_range = "navto1v";
            DeviceMgr.Action(nav_range, 0);
            if (stm == ACTION_STATE.INIT_FOR_START)
                return ACTION_STATE.INIT_FOR_SEARCH;
            else
                return stm;
        }
        private ACTION_STATE InitKI(ACTION_STATE stm)
        {
            RxInfo currRx = rx;
            SetDelays();
            currRx.var.iSrc = RunWnd.syscfg.GetCurrent(currRx.iIx,currRx.bSqrt); //0.1mA
            currRx.var.rRx = currRx.dRxInput;
            currRx.var.iK = RX_VAR.INIT_LOOP;
            currRx.var.iK = currRx.var.calc_capture_nextk();

            if (currRx.iRRange == RNG_P1) //P1 ohm
                currRx.var.iK = 100; //10:1
            if (currRx.iRRange == RNG_P01) //P01 ohm
                currRx.var.iK = 10;  //100:1
            if (currRx.iRRange == RNG_P001) //P001 ohm
                currRx.var.iK = 1;   //1000:1
            
            DeviceMgr.Action("turnk", Convert.ToUInt32(currRx.var.iK));
            if ((currRx.iRRange == RNG_P1) || (currRx.iRRange == RNG_P01) || (currRx.iRRange == RNG_P001))
                return ACTION_STATE.INIT_FOR_RS_KTTP;
            else
                return ACTION_STATE.INIT_FOR_TRACKP;
        }

        private double GetSqrt3(double delta)
        {
            double avg = datafilter.Average();
            double sqr = 0;
            foreach (double v in datafilter)
            {
                sqr = sqr + v * (v - avg);
            }
            sqr = Math.Sqrt(sqr / (datafilter.Count - 1));
            if (Math.Abs(sqr / avg) < 5e-9) //0.05ppm
                sqr = avg * 5e-9;
            else if (Math.Abs(sqr) < 1e-9) //10nV
                sqr = 1e-9;
            return sqr * delta;

        }
        private void CollectReading()
        {
            if (DeviceMgr.IsInAction())
                return;
            if (!bFirstReadSent)
            {
                DeviceMgr.Action("navread", 0);
                bFirstReadSent = true;
                return;
            }

            if (DeviceMgr.reading < -999)
                badReadingCount++;
            else
            {
                badReadingCount = 0;
                if (RunWnd.syscfg.sNavmeter != NAV_TYPE_2182 && nav_range != "navto1v")
                    datafilter.Add(DeviceMgr.reading / 1000);
                else
                    datafilter.Add(DeviceMgr.reading);
            }
            if (badReadingCount > 5) //to much invalid readings
            {
                dStableReading = -9999;
                bInReading = false;
                return;
            }
            if (datafilter.Count < RunWnd.syscfg.iSampleTimes)
            {
                DeviceMgr.Action("navread", 0);
                return;
            }
            int cnt0 = 0;  //count for no change
            int cnt1 = 0;  //count for going up
            int cnt_1 = 0; //count for going down
            for (int i = 1; i < datafilter.Count; i++)
            {
                if (Math.Abs(datafilter[i] - datafilter[i - 1]) < 1e-12)
                    cnt0++;
                else if (datafilter[i] > datafilter[i - 1])
                        cnt1++;
                else
                        cnt_1++;
            }
            if ((cnt1 == datafilter.Count - 1) || (cnt_1 == datafilter.Count - 1)) //all up or all down
            {
//no jump check                datafilter.Clear();
//                return;
            }


            {
                double sqr;
                if (rx.iRRange == RNG_P001) //0.001
                    sqr = GetSqrt3(10);
                else if (rx.iRRange == RNG_P01) //0.01
                    sqr = GetSqrt3(5);
                else
                    sqr = GetSqrt3(3);
                double avg = datafilter.Average();
                int removed = datafilter.RemoveAll(x => Math.Abs(x - avg) > sqr);
                
                string type = RunWnd.syscfg.sFilterType;

                if (type != "filtertype4" && removed > 0)
                {
                    DeviceMgr.reading = -9999; //clear the current reading
                    return; //not enough data now
                }
                
                 if (type == "filtertype1") //中位数平均值
                {
                    datafilter.Sort();
                    dStableReading = datafilter.Skip(datafilter.Count * 3 / 5).Take(datafilter.Count / 4).Average();
                    bInReading = false;
                    return;
                }
                if (type == "filtertype2")  //最后值平均
                {
                    int len = 10;
                    dStableReading = datafilter.Skip(datafilter.Count - len).Take(len).Average();
                    bInReading = false;
                }
                if (type == "filtertype3") //滑动滤波平均
                {
                    int len = datafilter.Count - 5;
                    if(len > 10)
                        len = 10;
                    if (len <= 0)
                        len = 2;
                    for (int i = (datafilter.Count - 1) ; i >= len; i--)
                    {
                        datafilter[i] = datafilter.Skip(i-len).Take(len).Average();
                    }
                    datafilter.RemoveRange(0, len);
                    datafilter.Sort();
                    dStableReading = datafilter.Skip(datafilter.Count / 3).Take(datafilter.Count / 3).Average();
                    bInReading = false;
                }
                if (type == "filtertype4") //看分布平均
                {
                    int[] bins = new int[]{0,0,0,0,0};
                    double max = datafilter.Max();
                    double min = datafilter.Min();
                    double step = (max-min)/(bins.Length - 2);
                    int i;
                    foreach (double v in datafilter)
                    {
                        for (i = 0; i < bins.Length-1; i++)
                        {
                            if (v < (min + i * step))
                            {
                                bins[i]++;
                                break;
                            }
                        }
                        if (i >= bins.Length)
                            bins[bins.Length - 1]++;
                    }
                    
                    double bottom = -99999;
                    double top = -9999;
                    for (i = 0; i < bins.Length - 1; i++)
                    {
                        bottom = top;
                        top = min + i * step;
                        if (bins[i] == bins.Max())
                            break;
                    }
                    if (i == bins.Length - 1)
                    {
                        bottom = top;
                        top = 10e9;
                    }
                    dStableReading = datafilter.Where(x => (x <= top && x >= bottom)).Average();
                    bInReading = false;
                }

                return;
            }
            throw new NotImplementedException("unexpected action state in data processing");    
        }
        
    }
}

