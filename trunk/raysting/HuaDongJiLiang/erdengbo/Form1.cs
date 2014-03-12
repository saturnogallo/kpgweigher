using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;
using System.Threading;
using System.IO;
using ZedGraph;
using System.Text.RegularExpressions;
namespace Jmbo
{
    [System.Runtime.InteropServices.ComVisible(true)]
    public partial class Form1 : Form
    {
        internal TestDocument test;
        private TestProcess process;
        private Input input_dlg;
        private MsgDlg msg_dlg;
        public AutoResetEvent uilock = new AutoResetEvent(false);
        public Form1()
        {
            if (!File.Exists("C:\\blank.htm"))
            {
                File.WriteAllText("C:\\blank.htm", "<html><head>blank></head><body></body></html>");
            }
            InitializeComponent();
            zedallprobes.GraphPane.Title.Text = "各探头阻值()";
            zedallprobes.IsShowPointValues = true;
            btn_start.Enabled = true;
            saveFileDialog1.AddExtension = true;
            saveFileDialog1.CheckPathExists = true;
            saveFileDialog1.OverwritePrompt = true;
            saveFileDialog1.ValidateNames = true;
            saveFileDialog1.DefaultExt = ".rec";
            saveFileDialog1.Filter = "Record File(*.rec)|*.rec|All Files(*.*)|*.*";
            saveFileDialog1.FileName = "";

            openFileDialog1.CheckFileExists = true;
            openFileDialog1.Multiselect = false;
            openFileDialog1.ValidateNames = true;
            openFileDialog1.Filter = "Record File(*.rec)|*.rec|All Files(*.*)|*.*";
            openFileDialog1.DefaultExt = ".rec";
            openFileDialog1.FileName = "";
            
            cb_probetype.SelectedIndex = 0;
            cb_probetype.SelectedValueChanged += new EventHandler(cb_temprange_SelectedValueChanged);

            
            this.webBrowser1.IsWebBrowserContextMenuEnabled = false;
            this.webBrowser1.AllowWebBrowserDrop = false;
            //this.webBrowser1.ScrollBarsEnabled = false;
            this.webBrowser1.WebBrowserShortcutsEnabled = false;
            try
            {
                this.webBrowser1.ObjectForScripting = this;
                
            }
            catch
            {
            }
            LoadDefaultRecordPage();

            test = new TestDocument();
            process = new TestProcess(this);
            input_dlg = new Input();
            
            msg_dlg = new MsgDlg();
            NewToolStripMenuItem_Click(null, null);
            this.webBrowser1.DocumentCompleted += new WebBrowserDocumentCompletedEventHandler(webBrowser1_DocumentCompleted);
            backgroundWorker1.DoWork += new DoWorkEventHandler(backgroundWorker1_DoWork);
            backgroundWorker1.WorkerSupportsCancellation = true;
            backgroundWorker1.WorkerReportsProgress = true;
            backgroundWorker1.RunWorkerCompleted += new RunWorkerCompletedEventHandler(backgroundWorker1_RunWorkerCompleted);
        }

        void backgroundWorker1_RunWorkerCompleted(object sender, RunWorkerCompletedEventArgs e)
        {
            if (e.Error != null)
            {
                MessageBox.Show(e.Error.Message);
            }
            else if (e.Cancelled)
            {
                // Next, handle the case where the user canceled 
                // the operation.
                // Note that due to a race condition in 
                // the DoWork event handler, the Cancelled
                // flag may not have been set, even though
                // CancelAsync was called.
                // todo
                MessageBox.Show("测试意外终止:"+e.Result.ToString());
            }
            else
            {
                // Finally, handle the case where the operation 
                // succeeded.
                // todo
                test.FillInPage(this);
                MessageBox.Show("测试成功完成");
            }
            uilock.Reset();
            process.bRunning = false;
            lbl_status.Text = "";
            test.ProbeType = cb_probetype.SelectedText.ToUpper();
            btn_start.Text = "开始测量";
            cb_probetype.Enabled = true;
            btn_start.Enabled = true;

        }

        void cb_temprange_SelectedValueChanged(object sender, EventArgs e)
        {
            try
            {
                if (test.IsViewMode)
                    btn_start.Enabled = false;
                else
                    btn_start.Enabled = true;
                
            }
            catch
            {
            }
        }

 
        void backgroundWorker1_DoWork(object sender, DoWorkEventArgs e)
        {
            BackgroundWorker worker = sender as BackgroundWorker;

            try
            {
                process.StartProcess(worker);
                return;
            }catch(Exception ex)
            {
                string line = ex.Message;
                e.Cancel = true;
                e.Result = ex.Message;
                return;
            }

            throw new NotImplementedException();
        }
        public void ShowMessage(string msg)
        {
            msg_dlg.message = msg;
            msg_dlg.ShowDialog();
            uilock.Set();
            process.form_result = msg_dlg.choice;

        }
        private string TranslateString(string msg)
        {
            string ret;
            ret = Util.ConstIni.StringValue("PROMPT", msg);
            if(Regex.IsMatch(msg,"^(m|c)_\\w_"))
                ret = Util.ConstIni.StringValue("PROMPT", "m_"+msg.Remove(0,4));
            if (ret == "")
            {
                if(msg.StartsWith("c_"))
                    ret = "请手工输入数据:";
            }
            return ret;
        }
        public void UpdateStatus(string msg)
        {
            lbl_status.Text = msg;
            uilock.Set();
        }
        private void LoadDefaultRecordPage()
        {
            this.webBrowser1.DocumentText = Jmbo.Properties.Resources.jmrec;
        }
        private void OpenToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (test.IsDirty)
            {
                DialogResult dr = MessageBox.Show("打开历史记录前是否保存本次测试记录?\n(点'是'保存,点'否'继续)", "是否保存", MessageBoxButtons.YesNoCancel);
                if (DialogResult.Cancel == dr)
                    return;
                if (DialogResult.Yes == dr)
                {
                    SaveToolStripMenuItem_Click(null, null);
                }
            }
            if(openFileDialog1.ShowDialog() != DialogResult.OK)
                    return;
            if (!test.LoadHistory(openFileDialog1.FileName))
            {
                MessageBox.Show("打开历史记录文件失败", "失败");
            }
            else
            {
                this.tabPage2.Text = "原始记录-(" + openFileDialog1.FileName + ")";
                LoadDefaultRecordPage();
            }
            
        }

        void webBrowser1_DocumentCompleted(object sender, WebBrowserDocumentCompletedEventArgs e)
        {
            test.FillInPage(this);
        }

        private void SaveToolStripMenuItem_Click(object sender, EventArgs e)
        {
//          if (test.IsDirty)
            {
                saveFileDialog1.DefaultExt = ".rec";
                saveFileDialog1.Filter = "Record File(*.rec)|*.rec|All Files(*.*)|*.*";

                saveFileDialog1.FileName = DateTime.Now.ToString("yyyy-MM-dd") + ".rec";
                if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                    return;
                test.SaveToDisk(saveFileDialog1.FileName);
                this.tabPage2.Text = "原始记录-(" + saveFileDialog1.FileName + ")";
            }
        }

        private void NewToolStripMenuItem_Click(object sender, EventArgs e)
        {
            
            if (test.IsDirty)
            {
                DialogResult dr = MessageBox.Show("是否保存本次测试记录?\n(点'是'保存,点'否'继续)", "是否保存", MessageBoxButtons.YesNoCancel);
                if (DialogResult.Cancel == dr)
                    return;
                if (DialogResult.Yes == dr)
                {
                    SaveToolStripMenuItem_Click(null, null);
                }
            }
            test.CreateNewRecord();
            test.IsViewMode = false;
            this.tabPage2.Text = "原始记录-新测量";
            LoadDefaultRecordPage();
        }



        private void AboutToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Assembly assem = Assembly.GetExecutingAssembly();
            MessageBox.Show(String.Format(@"
名称 : 二等铂电阻温度计标准装置自动测量系统
作者 : 上海市计量测试技术研究院(SIMT)
版本 : {0}
时间 : 2013/09", assem.GetName().Version.ToString()), "关于");
        }
        #region page record related function

        public string getvalue(string index)
        {
            HtmlElement el = this.webBrowser1.Document.All[index];
            return el.InnerText;
        }
        public void askinput(string id)
        {
            if (test.IsViewMode)
            {
                MessageBox.Show("历史数据不可以修改");
                return;
            }
            input_dlg.Message = TranslateString(id);
            
                Decimal dat;
                if (!id.StartsWith("c_"))
                {
                    if (DialogResult.OK == input_dlg.ShowDialog())
                    {
                        process.form_result = input_dlg.result;
                        setvalue(id, process.form_result);
                        test[id] = process.form_result;
                        if(test.CopyToOtherPages(id,process.form_result))
                            FillInPage("");
                    }
                }
                else
                {
                    try
                    {
                        if (id.LastIndexOfAny("1234567890".ToCharArray()) != (id.Length - 1))
                            throw new Exception("计算结果不能直接输入");
                        if (process.bRunning)
                            throw new Exception("请在程序停止后再手工输入数据");

                        if (DialogResult.OK == input_dlg.ShowDialog())
                        {
                            process.form_result = input_dlg.result;
                            if (!Decimal.TryParse(process.form_result, out dat))
                                throw new Exception("数据格式错误");

                            setvalue(id, process.form_result);
                            test[id] = dat;
                            test.AutoCaculation(false);
                            FillInPage("");
                        }
                    }
                    catch(Exception err)
                    {
                        MessageBox.Show(err.Message, "错误");
                    }

                }
                

            if(process.bRunning)
                uilock.Set();
        }
        public void FillInPage(string value)
        {
            test.FillInPage(this);
            uilock.Set();
        }
        public void ClearAllGraph(string type)
        {
            ClearGraph(type);
            uilock.Set();
        }
        public void setvalue(string name, string value)
        {
            // You can call any method from the page you loaded...
            // I have enclosed this call in a try-catch, because it will
            // create a exception when there isn't a page loaded ...
            try
            {
                // If you call a method, you can use a object array to
                // supply parameters. My method only has 1 parameter,
                // so I give 1 object in the array.
                this.webBrowser1.Document.InvokeScript("DisplayValue", new object[] { name + "=" + value });
            }
            catch (Exception ex)
            {
            }
        }
        public void callfunc(string func, string param)
        {
            // You can call any method from the page you loaded...
            // I have enclosed this call in a try-catch, because it will
            // create a exception when there isn't a page loaded ...
            try
            {
                // If you call a method, you can use a object array to
                // supply parameters. My method only has 1 parameter,
                // so I give 1 object in the array.
                this.webBrowser1.Document.InvokeScript(func, new object[] { param });
            }
            catch (Exception ex)
            {
            }

        }
        #endregion

        
        private void button_Start_Click(object sender, EventArgs e)
        {
            if (test.IsViewMode)
            {
                btn_start.Enabled = false;
                return;
            }
            SetDefaultZedTitle();
            if (btn_start.Text == "开始测量")
            {
                string sel = cb_probetype.SelectedItem.ToString();
                if (!sel.StartsWith("Pt"))
                {
                    MessageBox.Show("请选择被检温度计类型");
                    return;
                }
                test.ClearData();
                test.SetStartInfo();

                test.ProbeType = sel.Split(new char[] { '_' })[0].ToUpper();
                string swire = sel.Split(new char[] { '_' })[1];
                if (swire == "三线")
                    TestProcess.wire = WIREMODE.MODE_3WIRE;
                else
                    TestProcess.wire = WIREMODE.MODE_4WIRE;
                btn_start.Enabled = false;
                process.bRunning = true;
                cb_probetype.Enabled = false;
                btn_start.Text = "停止测量";
                ClearGraph("");
                uilock.Set();
                backgroundWorker1.RunWorkerAsync();
                btn_start.Enabled = true;
            }
            else
            {
                btn_start.Enabled = false;
                if (DialogResult.Yes != MessageBox.Show("是否要停止当前测量", "问题", MessageBoxButtons.YesNo))
                    return;
                
                backgroundWorker1.CancelAsync();
                uilock.Set();

            }
        }
        string tm = "时间(min)";
        string rt = "温度计阻值(Ω)";
        string wd = "温度曲线(℃)";
        private void SetDefaultZedTitle()
        {
            GraphPane myPane;
            myPane = zedallprobes.GraphPane;
            myPane.Title.Text = rt;
            myPane.XAxis.Title.Text = tm; 
            myPane.YAxis.Title.Text = "(Ω)";
        }
 

        private void ClearGraph(string type)
        {
            ZedGraphControl zed = zedallprobes;
            GraphPane myPane = zed.GraphPane;
            myPane.CurveList.Clear();
            zed.Refresh();
        }
        private Color[] colormap = new Color[]{Color.Black, Color.DarkOrange,Color.Red,Color.Green,Color.DarkBlue,
            Color.DarkViolet, Color.DeepPink,Color.Brown,Color.DarkKhaki};
        private SymbolType[] symbolmap = new SymbolType[]{SymbolType.Circle,SymbolType.Diamond,SymbolType.Triangle,SymbolType.XCross,SymbolType.Square,
            SymbolType.Star,SymbolType.Triangle,SymbolType.XCross,SymbolType.VDash};
        internal void AddToGraph(int ch, double value,string type)
        {

            ZedGraphControl zed = zedallprobes;
            GraphPane myPane = zedallprobes.GraphPane;
            myPane.Legend.IsVisible = true;

            double index = DateTime.Now.Subtract(test.test_start).TotalMinutes;
            if (index < 0)
                return;
            if (myPane.CurveList.Count <= ch)
            {
                LineItem li;
                li = myPane.AddCurve(("CH "+ch.ToString()).Replace("CH 0","标准温度计"), new double[] { index }, new double[] { value }, colormap[ch],symbolmap[ch]);
                li.Symbol.Fill = new Fill(colormap[ch]);

                myPane.XAxis.Title.Text = tm;
            }
            else
            {
                 myPane.CurveList[ch].AddPoint(index, value);
            }
            myPane.AxisChange();
            if (type == "ohm")
            {
                myPane.Title.Text = rt;
                myPane.YAxis.Title.Text = "(Ω)";
            }
            if (type == "c")
            {
                myPane.Title.Text = wd;
                myPane.YAxis.Title.Text = "(℃)";
            } 
            zed.Refresh();
        }



        private void ReportToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (process.bRunning)
                return;

            saveFileDialog1.DefaultExt = ".doc";
            saveFileDialog1.Filter = "DOC File(*.doc)|*.doc|All Files(*.*)|*.*";

            saveFileDialog1.FileName = DateTime.Now.ToString("yyyy-MM-dd") + "原始记录单.doc";
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            test.FillInRawData(saveFileDialog1.FileName);
        } 




        private void 生成分度表ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (process.bRunning)
                return;
            
            test.FillInFendu(saveFileDialog1);

        }

        private void 结果通知书ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (process.bRunning)
                return;

            saveFileDialog1.DefaultExt = ".doc";
            saveFileDialog1.Filter = "DOC File(*.doc)|*.doc|All Files(*.*)|*.*";

            saveFileDialog1.FileName = DateTime.Now.ToString("yyyy-MM-dd") + "结果通知书.doc";
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            test.FillInDocument(REPORT_TYPE.INFO_REPORT, saveFileDialog1.FileName);
        }

        private void 校准证书ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (process.bRunning)
                return;

            saveFileDialog1.DefaultExt = ".doc";
            saveFileDialog1.Filter = "DOC File(*.doc)|*.doc|All Files(*.*)|*.*";


            saveFileDialog1.FileName = DateTime.Now.ToString("yyyy-MM-dd") + "校准证书.doc";
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            test.FillInDocument(REPORT_TYPE.JIAOZHUN_REPORT, saveFileDialog1.FileName);
        }

        private void 检定证书ToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (process.bRunning)
                return;

            saveFileDialog1.DefaultExt = ".doc";
            saveFileDialog1.Filter = "DOC File(*.doc)|*.doc|All Files(*.*)|*.*";


            saveFileDialog1.FileName = DateTime.Now.ToString("yyyy-MM-dd") + "检定证书.doc";
            if (saveFileDialog1.ShowDialog() != DialogResult.OK)
                return;
            test.FillInDocument(REPORT_TYPE.CALI_REPORT, saveFileDialog1.FileName);

        }
    }
}
