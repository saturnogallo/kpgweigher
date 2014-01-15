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
        RX,
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

            AdjustPanel();
        }

        void btn_left_ValidClick(object sender, EventArgs e)
        {
            config.iAux1Data = config.iAux1Data - 1;
            if (config.iAux1Data < RXDATA_MODE.LAST)
            {
                config.iAux1Data = RXDATA_MODE.MIN;
            }
            ReDraw();
        }
        void btn_right_ValidClick(object sender, EventArgs e)
        {
            config.iAux1Data = config.iAux1Data + 1;
            if (config.iAux1Data > RXDATA_MODE.MIN)
            {
                config.iAux1Data = RXDATA_MODE.LAST;
            }
            ReDraw();
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
            ReDraw();
        }

        void btn_aux2_ValidClick(object sender, EventArgs e)
        {
            /*
            config.iAux2Data = config.iAux2Data + 1;
            if (config.iAux2Data > RXDATA_MODE.MIN)
            {
                config.iAux2Data = RXDATA_MODE.LAST;
            }
            ReDraw();
             */
        }

        void btn_aux1_ValidClick(object sender, EventArgs e)
        {
            config.iAux1Data = config.iAux1Data + 1;
            if (config.iAux1Data > RXDATA_MODE.MIN)
            {
                config.iAux1Data = RXDATA_MODE.LAST;
            }
            ReDraw();
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
            PointPair.datamode = mode;
            if (mode == RXDATA_MODE.LAST)
                return Util.FormatData(Program.data.my_last(config.sChannel), 8) + " C";
            if (mode == RXDATA_MODE.RATIO)
            {
                double r = Program.data.my_avg(config.sChannel) / Program.lst_rsinfo[myRx.iStdChan-1].dTValue;
                return Util.FormatDataWithSpace(r,8);
            }
            if (mode == RXDATA_MODE.RX)
            {
                return Util.FormatData(Program.data.my_last(config.sChannel).dTValue, 8) + " Ω";
            }

            if (mode == RXDATA_MODE.RS)
            {
                return Util.FormatData(Program.lst_rsinfo[myRx.iStdChan-1].dTValue, 8) + " Ω";
            }
            if (mode == RXDATA_MODE.AVERAGE)
            {
                if (Program.data.validrows(config.sChannel) == 0)
                    return "0.000 000 0 Ω";

                return Util.FormatData(Program.data.my_avg(config.sChannel), 8) + " Ω";
            }
            if (mode == RXDATA_MODE.MAX)
                return Util.FormatData(Program.data.my_max(config.sChannel), 8) + " Ω";

            if (mode == RXDATA_MODE.MIN)
                return Util.FormatData(Program.data.my_min(config.sChannel), 8) + " Ω";

            if (mode == RXDATA_MODE.VARIANCE)
            {
                return (Program.data.my_sqrtsum(config.sChannel) * 1e6).ToString("F2") + " PPM";
            }
            return "0";
        }
        public void ReDraw()
        {
            lbl_reading.Text = GetData(config.iMainData);
            lbl_aux1.Text = GetData(config.iAux1Data);
            lbl_aux2.Text = GetData(config.iAux2Data);
            btn_aux1.Text = StringResource.str("val_" + config.iAux1Data.ToString().ToLower());
            btn_aux2.Text = StringResource.str("val_" + config.iAux2Data.ToString().ToLower());
            btn_bigdisp.Text = StringResource.str("val_" + config.iMainData.ToString().ToLower());
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
