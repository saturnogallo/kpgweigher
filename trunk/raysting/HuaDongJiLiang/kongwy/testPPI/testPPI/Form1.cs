using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO.Ports;
using System.Threading;

namespace testPPI
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

       

        private void Form1_Load(object sender, EventArgs e)
        {
            //自动获取COM口名称
            foreach (string com in System.IO.Ports.SerialPort.GetPortNames())
            {
                this.comportName.Items.Add(com);
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        private void button1_Click(object sender, EventArgs e)
        {
            try
            {
                if (this.serialPort1.IsOpen)
                {
                    this.serialPort1.Close();
                }
                else
                {
                    // 设置端口参数
                    this.serialPort1.BaudRate = int.Parse(this.comboBox2.Text);
                    this.serialPort1.DataBits = int.Parse(this.comboBox3.Text);
                    this.serialPort1.StopBits = (StopBits)Enum.Parse(typeof(StopBits), this.comboBox4.Text);
                    this.serialPort1.Parity = (Parity)Enum.Parse(typeof(Parity), this.comboBox5.Text);
                    this.serialPort1.PortName = this.comportName.Text;
                    //comport.Encoding = Encoding.ASCII;

                    //打开端口
                    this.serialPort1.Open();
                }
                this.groupBox1.Enabled = !this.serialPort1.IsOpen;
                //txtsend.Enabled = btnsend.Enabled = comport.IsOpen;

                if (this.serialPort1.IsOpen)
                {
                    this.button1.Text = "&C关闭端口";
                }
                else
                {
                    this.button1.Text = "&O打开端口";
                }
                //if (this.serialPort1.IsOpen) txtsend.Focus();
            }
            catch (Exception er)
            {
                MessageBox.Show("端口打开失败！" + er.Message, "提示");
            }

        }

        private void button18_Click(object sender, EventArgs e)
        {
            this.serialPort1.Close();
            this.Close();
        }

        private void button2_Click_1(object sender, EventArgs e)
        {

        }

        private void button2_Click_2(object sender, EventArgs e)
        {
            byte[] data=null;
            if ("VB".Equals(this.comboBox1.Text))
            {
                try
                {
                    data = ConvertDataToByte.WriteVB(2, Int32.Parse(this.textBox1.Text), Int32.Parse(this.textBox2.Text));

                    if (this.serialPort1.IsOpen)
                    {
                        serialPort1.Write(data, 0, data.Length);
                    }
                    else
                    {
                        MessageBox.Show("端口尚未打开");
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
            else if ("VM".Equals(this.comboBox1.Text))
            {
                try
                {
                    data =  ConvertDataToByte.WriteVW(2,Int32.Parse(this.textBox1.Text),Int32.Parse(this.textBox2.Text));
                    if (this.serialPort1.IsOpen)
                    {
                        serialPort1.Write(data, 0, data.Length);
                    }
                    else
                    {
                        MessageBox.Show("端口尚未打开");
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
            else if ("Q".Equals(this.comboBox1.Text))
            {
                try
                {
                    data = ConvertDataToByte.WriteQ(2, Double.Parse(this.textBox1.Text), Int32.Parse(this.textBox2.Text));
                    if (this.serialPort1.IsOpen)
                    {
                        serialPort1.Write(data, 0, data.Length);
                        Thread.Sleep(100);
                        //serialPort1.Close();
                    }
                    else
                    {
                        MessageBox.Show("端口尚未打开");
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }else if("Q_Word".Equals(this.comboBox1.Text))
            {
                try
                {
                    string[] ds = this.textBox2.Text.Split(',');
                    data = ConvertDataToByte.WriteQ_w(2, Int32.Parse(this.textBox1.Text), Int32.Parse(ds[0]), Int32.Parse(ds[1]));
                    if (this.serialPort1.IsOpen)
                    {
                        serialPort1.Write(data, 0, data.Length);
                        Thread.Sleep(100);
                        //serialPort1.Close();
                    }
                    else
                    {
                        MessageBox.Show("端口尚未打开");
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
            else
            {
                
                try
                {
                    data = ConvertDataToByte.ReadI(2, Int32.Parse(this.textBox1.Text), Double.Parse(this.textBox2.Text));
                    if (this.serialPort1.IsOpen)
                    {
                        serialPort1.Write(data, 0, data.Length);
                        Thread.Sleep(100);
                        //serialPort1.Close();
                    }
                    else
                    {
                        MessageBox.Show("端口尚未打开");
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }

            if (data != null)
            {
                this.textBox4.Text = "";
                this.textBox4.Text += BitConverter.ToString(data).Replace("-", " ");
            }
        }

        private void serialPort1_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            int length = serialPort1.BytesToRead;
            byte[] data = new byte[length];
            if(length > 0)
            {
                //byte[] data = new byte[length];
                serialPort1.Read(data,0,length);
                serialPort1.DiscardInBuffer();
                //如果接收到的数据长度为1，值为E5H (229)，则为确认码，返回读写确认命令
                if(data[0] == (byte)229 && data.Length == 1)
                {
                    //收到数据
                    byte[] dataSend = new byte[6];
                    dataSend[0] = 16;
                    //站号
                    dataSend[1] = 0x02;
                    dataSend[2] = 0x00;
                    dataSend[3] = 0x5C;
                    dataSend[4] = (byte)(dataSend[3] + dataSend[1]);
                    dataSend[5] = 0x16;
                    serialPort1.DiscardOutBuffer();
                    serialPort1.DiscardInBuffer();
                    Thread.Sleep(100);
                    serialPort1.Write(dataSend, 0, dataSend.Length);
                    Thread.Sleep(100);
                }
            }
            Invoke
           (new EventHandler
             (delegate
             {
                 this.textBox3.Text += BitConverter.ToString(data);
             }
             )
            );
        }

        private void comboBox1_SelectedValueChanged(object sender, EventArgs e)
        {
            if (this.comboBox1.Text.Equals("I"))
            {
                this.label7.Text = "长度";
                this.label8.Text = "地址";
            }
            else
            {
                this.label7.Text = "地址";
                this.label8.Text = "值";
            }
        }

    }
}
