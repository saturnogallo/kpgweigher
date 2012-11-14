using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Zddq2
{
    public enum RXDISP_MODE
    {
        
        GRID,
        GRAPH,
        MIXED
    }
    public enum RXDATA_MODE
    {
        LAST,
        RATIO,
        RS,
        AVERAGE,
        VARIANCE,
        MAX,
        MIN,
    }
    public partial class RxDisplay : UserControl
    {
        static private int winid = 1;
        UIConfig config;
        public RxDisplay()
        {
            config = new UIConfig(winid);
            winid++;
            InitializeComponent();

            btn_aux1.bgColor = Color.LightGreen;
            btn_aux1.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_aux1.ValidClick += new EventHandler(btn_aux1_ValidClick);


            btn_left.bgColor = Color.LightGreen;
            btn_left.Text = "<";
            btn_left.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            btn_left.ValidClick += new EventHandler(btn_left_ValidClick);

            btn_right.bgColor = Color.LightGreen;
            btn_right.Text = ">";
            btn_right.SetStyle(Color.LightGreen, MyButtonType.round2RectButton);
            btn_right.ValidClick += new EventHandler(btn_right_ValidClick);


            btn_aux2.bgColor = Color.LightGreen;
            btn_aux2.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_aux2.ValidClick +=new EventHandler(btn_aux2_ValidClick);

            btn_bigdisp.bgColor = Color.LightGreen;
            btn_bigdisp.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_bigdisp.ValidClick += new EventHandler(btn_bigdisp_ValidClick);

            btn_dispmode.bgColor = Color.LightGreen;
            btn_dispmode.SetStyle(Color.Beige, MyButtonType.round2RectButton);
            btn_dispmode.ValidClick += new EventHandler(btn_dispmode_ValidClick);

            dt = new DataTable("values");
            dt.Columns.Add("id");
            dt.Columns.Add("data");
            dt.Columns.Add("ratio");
            dt.Columns.Add("time");
            dt.Columns.Add("excluded");
            SetGridStyle();
            dataGrid2.DataSource = dt;
            dataGrid2.Click += new EventHandler(dataGrid2_Click);
            AdjustPanel();
            graphPane2.XAxisTicCount = 10;
            graphPane2.YAxisTicCount = 4;
            graphPane2.YAxisMax = 5;
            graphPane2.YAxisMin = -5;
            graphPane2.YAxisTitle = "PPM";
            graphPane2.XAxisTicCount = 10;
            /*
            for (int j = 0; j < 25;j++ )
                AddValue(j+1, 100152.34-j);
            ThrowData();
             */
            graphPane2.Invalidate();
        }

        void btn_left_ValidClick(object sender, EventArgs e)
        {
            config.iAux1Data = config.iAux1Data - 1;
            if (config.iAux1Data < RXDATA_MODE.LAST)
            {
                config.iAux1Data = RXDATA_MODE.MIN;
            }
            ReDraw(false);
        }
        void btn_right_ValidClick(object sender, EventArgs e)
        {
            config.iAux1Data = config.iAux1Data + 1;
            if (config.iAux1Data > RXDATA_MODE.MIN)
            {
                config.iAux1Data = RXDATA_MODE.LAST;
            }
            ReDraw(false);
        }

        void dataGrid2_Click(object sender, EventArgs e)
        {
            if (dataGrid2.CurrentCell.ColumnIndex == 4)
            {
                if (dataGrid2.CurrentCell.RowIndex >= 0)
                {
                    PointPair pp = graphPane2.PointList[dataGrid2.CurrentCell.RowIndex];
                    
                    if(pp.excluded)
                        dt.Rows[dataGrid2.CurrentCell.RowIndex]["excluded"] = "";
                    else
                        dt.Rows[dataGrid2.CurrentCell.RowIndex]["excluded"] = "【√】";
                    pp.excluded = !pp.excluded;
                    
                    btn_aux1.Focus();
                    graphPane2.PointList[dataGrid2.CurrentCell.RowIndex] = pp;
                    ReDraw(true);
                }
            }
        }
        private void SetGridStyle()
        {
            DataGridTableStyle tblStyle = new DataGridTableStyle();
            tblStyle.MappingName = "values";
            DataGridColumnStyle col1 = new DataGridTextBoxColumn();
            col1.MappingName = "id";
            col1.HeaderText = StringResource.str("col_id");
            col1.Width = 50;
            col1.NullText = string.Empty;

            DataGridColumnStyle col2 = new DataGridTextBoxColumn();
            col2.MappingName = "data";
            col2.HeaderText = StringResource.str("col_data");
            col2.Width = 150;
            col2.NullText = string.Empty;

            DataGridColumnStyle col3 = new DataGridTextBoxColumn();
            col3.MappingName = "ratio";
            col3.HeaderText = StringResource.str("col_ratio");
            col3.Width = 150;
            col3.NullText = string.Empty;

            DataGridColumnStyle col4 = new DataGridTextBoxColumn();
            col4.MappingName = "time";
            col4.HeaderText = StringResource.str("col_time");
            col4.Width = 150;
            col4.NullText = string.Empty;

            DataGridColumnStyle col5 = new DataGridTextBoxColumn();
            col5.MappingName = "excluded";
            col5.HeaderText = StringResource.str("col_excluded");
            col5.Width = 150;
            col5.NullText = string.Empty;

            tblStyle.GridColumnStyles.Add(col1);
            tblStyle.GridColumnStyles.Add(col2);
            tblStyle.GridColumnStyles.Add(col3);
            tblStyle.GridColumnStyles.Add(col4);
            tblStyle.GridColumnStyles.Add(col5);
            //dataGridView1.TableStyles.Add(tblStyple)
        }
        public void LogData(int index, double value)
        {
            DeviceMgr.Log(String.Format("Reading {0} = {1}", index.ToString(),Util.FormatData(value, 8)));
        }
        public void LogComplete()
        {
            DeviceMgr.Log("...... Summary Begin ......");
            DeviceMgr.Log(String.Format("Rx(mean):{0}\nMax:{1}\nMin:{2}\nRx/Rs:{3}\nStdev:{4}", GetData(RXDATA_MODE.AVERAGE),
                GetData(RXDATA_MODE.MAX),
                GetData(RXDATA_MODE.MIN),
                GetData(RXDATA_MODE.RATIO),
                GetData(RXDATA_MODE.VARIANCE)));
            DeviceMgr.Log("...... Summary End ...... ");
        }

        //throw top 30% data and bottom 30% data;
        public void ThrowData()
        {
            List<PointPair> tempList = new List<PointPair>(this.graphPane2.PointList);
            tempList.Sort();
            int skip = (int)Math.Floor(tempList.Count * 0.3);
            if (skip > 0)
            {
                while (skip-- > 0)
                {
                    dt.Rows.RemoveAt(0);
                    dt.Rows.RemoveAt(dt.Rows.Count - 1);
                    graphPane2.PointList.RemoveAt(0);
                    graphPane2.PointList.RemoveAt(this.graphPane2.PointList.Count - 1);
                }
                skip = (int)Math.Floor(tempList.Count * 0.3);
                for (int n = 0; n < dt.Rows.Count; n++)
                {
                    dt.Rows[n]["id"] = Convert.ToInt32(dt.Rows[n]["id"].ToString()) - skip;
                    graphPane2.PointList[n].index = graphPane2.PointList[n].index - skip;
                }
                RefreshData(dt.Rows.Count);
            }
        }
        private DataTable dt;
        void btn_dispmode_ValidClick(object sender, EventArgs e)
        {
            config.iDataMode = config.iDataMode + 1;
            if (config.iDataMode > RXDISP_MODE.MIXED)
            {
                config.iDataMode = RXDISP_MODE.GRID;
            }
            ReDraw(true);
        }
        
        void btn_bigdisp_ValidClick(object sender, EventArgs e)
        {/*
            config.iMainData = config.iMainData + 1;
            if (config.iMainData > RXDATA_MODE.MIN)
            {
                config.iMainData = RXDATA_MODE.LAST;
            }
          */
            if (config.iMainData == RXDATA_MODE.AVERAGE)
                config.iMainData = RXDATA_MODE.RATIO;
            else
                config.iMainData = RXDATA_MODE.AVERAGE;
            ReDraw(true);
        }

        void btn_aux2_ValidClick(object sender, EventArgs e)
        {
            /*
            config.iAux2Data = config.iAux2Data + 1;
            if (config.iAux2Data > RXDATA_MODE.MIN)
            {
                config.iAux2Data = RXDATA_MODE.LAST;
            }
            ReDraw(true);
             */
        }

        void btn_aux1_ValidClick(object sender, EventArgs e)
        {
            config.iAux1Data = config.iAux1Data + 1;
            if (config.iAux1Data > RXDATA_MODE.MIN)
            {
                config.iAux1Data = RXDATA_MODE.LAST;
            }
            ReDraw(true);
        }
        
        public RxInfo myRx
        {
            get;
            set;
        }

        public string Text
        {
            get;
            set;
        }
        public void ClearAll()
        {
            dt.Clear();
            graphPane2.PointList.Clear();
            ReDraw(false);
            graphPane2.Refresh();
        }
        private double my_last
        {
            get
            {
                if (valid_rows < 1)
                    return 0;
                double last = 0;
                for (int i = 0; i < graphPane2.PointList.Count;i++ )
                {
                    if (!graphPane2.PointList[i].excluded)
                        last = graphPane2.PointList[i].value;
                }
                return last;
            }
        }
        private double my_min{
            get
            {
                if (valid_rows < 1)
                    return 0;
                double min = 1e+20;
                foreach (PointPair v in graphPane2.PointList)
                {
                    if (!v.excluded && (v.value < min))
                        min = v.value;
                }
                return min;
            }
        }
        private double my_max{
            get
            {
                if (valid_rows < 1)
                    return 0;
                double max = -9999;
                foreach (PointPair v in graphPane2.PointList)
                {
                    if (!v.excluded && (v.value>max))
                        max = v.value;
                }
                return max;
            }
        }
        private double my_sum{
            get
            {
                if (valid_rows < 1)
                    return 0;
                double sum = 0;
                foreach (PointPair v in graphPane2.PointList)
                {
                    if (!v.excluded)
                        sum = sum + v.value;
                }
                return sum;
            }
        }
        private double my_avg
        {
            get
            {
                if (my_sum < 1e-10)
                    return 0;
                return my_sum / valid_rows;
            }
        }
        private int valid_rows
        {
            get
            {
                int i = 0;
                foreach (PointPair v in graphPane2.PointList)
                {
                    if(!v.excluded)
                        i = i + 1;
                }
                return i;
            }
        }
        private double my_sqrtsum
        {
            get
            {
                if(my_sum < 1e-10)
                    return 0;
                
                int rows = valid_rows;
                if (rows < 2)
                    return 0;
                double sqr = 0;
                double avg = my_sum / rows;
                foreach (PointPair v in graphPane2.PointList)
                {
                    if(!v.excluded)
                        sqr = sqr + v.value * (v.value - avg); 
                }
                return Math.Sqrt(sqr/ (rows-1))/avg;
            }
        }
        public void AddValue(int index, double val)
        {
            graphPane2.PointList.Add(new PointPair(index, val, false, DateTime.Now));
            graphPane2.average = my_avg;

            DataRow dr = dt.NewRow();
            dr["id"] = index;
            dr["data"] = Util.FormatDataWithSpace(val , 8);
            dr["ratio"] = Util.FormatDataWithSpace(val / Program.lst_rsinfo[0].dTValue, 8);
                
            dr["time"] = DateTime.Now.ToString("HH:mm:ss.fff");
            dr["excluded"] = "";
            dt.Rows.Add(dr);

            RefreshData(index);            
        }
        private void RefreshData(int index)
        {
            if (graphPane2.Visible)
            {
                graphPane2.XAxisMax = valid_rows + 1;
                if (index > 1)
                {
                    graphPane2.YAxisMax = Math.Floor(5*my_sqrtsum * 1e6 + 0.5);
                    if (graphPane2.YAxisMax < 3)
                        graphPane2.YAxisMax = 3;
                }
                else
                {
                    graphPane2.YAxisMax = 1;
                }
                graphPane2.YAxisMin = -graphPane2.YAxisMax;
                graphPane2.Refresh();
            }
            ReDraw(false);
        }

        private Graphics graphic;
        private System.Drawing.Bitmap backBuffer;

        private Color colorTop = Color.LightGreen;
        private Rectangle areaTop;
        private Color colorBottom = Color.Gray;
        private Rectangle areaBottom;
        private Color bgColor = Color.Black;
        private void DrawRoundRect(Graphics g, Brush br, Rectangle area)
        {
            int rd = 8;
            rd = area.Width / 30;
            if (rd < 8)
                rd = 8;
            g.FillEllipse(br, area.X, area.Y, rd * 2, rd * 2);
            g.FillEllipse(br, area.X, area.Y + area.Height - rd * 2, rd * 2, rd * 2);
            g.FillEllipse(br, area.X + area.Width - rd * 2, area.Y, rd * 2, rd * 2);
            g.FillEllipse(br, area.X + area.Width - rd * 2, area.Y + area.Height - rd * 2, rd * 2, rd * 2);
            g.FillRectangle(br, area.X+1, area.Y + rd, area.Width-1 , area.Height - rd * 2);
            g.FillRectangle(br, area.X + rd, area.Y+1, area.Width - rd * 2, area.Height-1 );
        }
        private string GetData(RXDATA_MODE mode)
        {
            if (mode == RXDATA_MODE.LAST)
                return Util.FormatData(my_last, 8) + " Ω";
            if (mode == RXDATA_MODE.RATIO)
            {
                double r = my_avg / Program.lst_rsinfo[0].dTValue;
                return Util.FormatDataWithSpace(r,8);
            }
            if (mode == RXDATA_MODE.RS)
            {
                return Util.FormatData(Program.lst_rsinfo[0].dTValue, 8) + " Ω";
            }
            if (mode == RXDATA_MODE.AVERAGE)
            {
                if (valid_rows == 0)
                    return "0.000 000 0 Ω";
                
                return Util.FormatData(my_avg, 8) + " Ω";
            }
            if (mode == RXDATA_MODE.MAX)
                return Util.FormatData(my_max,8) + " Ω";

            if (mode == RXDATA_MODE.MIN)
                return Util.FormatData(my_min, 8) + " Ω";

            if (mode == RXDATA_MODE.VARIANCE)
            {
                return (my_sqrtsum * 1e6).ToString("F2") + " PPM";
            }
            return "0";
        }
        public void ReDraw(bool newmode)
        {
            if (newmode)
            {
                /*
                lbl_aux1.Top = this.ClientRectangle.Top + 50;
                lbl_aux1.Left = this.ClientRectangle.Left + 10;
                btn_aux1.Top = lbl_aux1.Top;
                btn_aux1.Left = lbl_aux1.Left + 80;

                lbl_aux2.Top = this.ClientRectangle.Top + 50;
                lbl_aux2.Left = this.ClientRectangle.Left + 110;
                btn_aux2.Top = lbl_aux2.Top;
                btn_aux2.Left = lbl_aux2.Left + 80;

                lbl_reading.Top = this.ClientRectangle.Top + 10;
                lbl_reading.Left = this.lbl_reading.Left + 20;
                btn_bigdisp.Top = lbl_reading.Top;
                btn_bigdisp.Left = lbl_reading.Left + 200;
                */
                if (config.iDataMode == RXDISP_MODE.GRAPH)
                {
                    dataGrid2.Visible = false;
                    graphPane2.Top = dataGrid2.Top;
                    graphPane2.Height = 340;
                    graphPane2.Visible = true;
                    RefreshData(dt.Rows.Count);
                    graphPane2.Refresh();
                }
                if (config.iDataMode == RXDISP_MODE.GRID)
                {
                    graphPane2.Visible = false;
                    dataGrid2.Height = 340;
                    dataGrid2.Visible = true;
                }
                if (config.iDataMode == RXDISP_MODE.MIXED)
                {
                    dataGrid2.Height = 170;
                    dataGrid2.Visible = true;
                    graphPane2.Top = dataGrid2.Top +dataGrid2.Height + 5;
                    graphPane2.Height = 170;
                    graphPane2.Visible = true;
                    RefreshData(dt.Rows.Count);
                    graphPane2.Refresh();
                }
                //arrange dialogs
            }
            lbl_reading.Text = GetData(config.iMainData);
            lbl_aux1.Text = GetData(config.iAux1Data);
            lbl_aux2.Text = GetData(config.iAux2Data);
            btn_aux1.Text = StringResource.str("val_" + config.iAux1Data.ToString().ToLower());
            btn_aux2.Text = StringResource.str("val_" + config.iAux2Data.ToString().ToLower());
            btn_bigdisp.Text = StringResource.str("val_" + config.iMainData.ToString().ToLower());
            btn_dispmode.Text = StringResource.str("disp_" + config.iDataMode.ToString().ToLower());
        }
        protected override void OnPaint(PaintEventArgs e)
        {
            SolidBrush myTopPen = new SolidBrush(colorTop);
            graphic.FillRectangle(myTopPen, this.ClientRectangle);

//            SolidBrush myBotPen = new SolidBrush(colorBottom);
//            DrawRoundRect(graphic, myBotPen, areaBottom);
//            DrawRoundRect(graphic, myTopPen, areaTop);
//            DrawText(graphic);
            myTopPen.Dispose();
//            myBotPen.Dispose();
            e.Graphics.DrawImage(backBuffer, 0, 0);
            
            base.OnPaint(e);
        }
        public void AdjustPanel()
        {
            int bgScale = 3;
            int bdScale = 3;
            areaTop = new Rectangle(bdScale, bdScale, this.ClientSize.Width - 2 * bdScale, this.ClientSize.Height - 2 * bdScale);
            areaBottom = new Rectangle(bgScale, bgScale, this.ClientSize.Width - 2 * bgScale, this.ClientSize.Height - 2 * bgScale);
            backBuffer = new Bitmap(this.Width, this.Height);
            graphic = Graphics.FromImage(backBuffer);
            this.Invalidate();
        }
        protected override void OnPaintBackground(PaintEventArgs e)
        {
            Graphics graphics = e.Graphics;
            graphics.FillRectangle(new SolidBrush(bgColor), ClientRectangle);
            //base.OnPaintBackground(e);
        }
        private void DrawText(Graphics graphic)
        {
            if (this.Text != null)
            {
                // 绘制文本 
                using (StringFormat f = new StringFormat())
                {
                    // 水平居中对齐 
                    f.Alignment = System.Drawing.StringAlignment.Center;
                    // 垂直居中对齐 
                    f.LineAlignment = System.Drawing.StringAlignment.Center;
                    // 设置为单行文本 
                    f.FormatFlags = System.Drawing.StringFormatFlags.NoWrap;
                    // 绘制文本 
                    using (SolidBrush b = new SolidBrush(this.ForeColor))
                    {
                        graphic.DrawString(
                            this.Text,
                            this.Font,
                            b,
                            new System.Drawing.RectangleF(
                                areaTop.X,
                                areaTop.Y,
                                areaTop.Width,
                                areaTop.Height),
                            f);
                    }
                }
            }

        }
        private void rectButton1_Load(object sender, EventArgs e)
        {

        }

        private void btn_dispmode_Load(object sender, EventArgs e)
        {

        }

        private void RxDisplay_Load(object sender, EventArgs e)
        {

        }
    }
}
