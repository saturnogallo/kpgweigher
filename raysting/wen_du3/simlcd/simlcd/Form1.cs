using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
namespace simlcd
{
    
    public partial class Form1 : Form
    {
        private Timer ct;
        public Form1()
        {
            InitializeComponent();
            ct = new Timer();
            ct.Interval = 500;
            
            ct.Tick += new EventHandler(ct_Tick);
            ct.Enabled = true;
            ct.Start();
            serialPort1.BaudRate = 57600;
            serialPort1.DataBits = 8;
            serialPort1.StopBits = System.IO.Ports.StopBits.One;
            serialPort1.PortName = "COM2";
            serialPort1.Parity = System.IO.Ports.Parity.None;
            serialPort1.Handshake = System.IO.Ports.Handshake.None;
            serialPort1.DataReceived += new System.IO.Ports.SerialDataReceivedEventHandler(serialPort1_DataReceived);
            serialPort1.Open();
            
        }

        void ct_Tick(object sender, EventArgs e)
        {
            if (doupdate)
            {
                doupdate = false;
                UpdateDisplay();
                
            }
        }
        private bool doupdate = false;
        public new void Dispose()
        {
            ct.Stop();
            serialPort1.Close();
            base.Dispose();

        }
        private Byte incnt = 0;
        void serialPort1_DataReceived(object sender, System.IO.Ports.SerialDataReceivedEventArgs e)
        {
            while(serialPort1.BytesToRead > 0)
            {
                pushchar((Byte)serialPort1.ReadByte());
                if (incnt++ > 10)
                {
                    sendchar((char)CMDI_CLEARBUF);
                    incnt = 0;
                }
            }
        }

        private void buttona_Click(object sender, EventArgs e)
        {
            string n = (sender as Button).Name.Replace("button", "");
            if (n.Length > 1)
            {
                n.Replace("DDD", "d");
                n.Replace("CCC", "c");
                n.Replace("DOT", ".");
            }
            if (n.Length == 1)
            {
                if (n == "a" && buttona.BackColor == Color.Green)
                {
                    sendchar('A');
                    return;
                }
                if (n == "b" && buttonb.BackColor == Color.Green)
                {
                    sendchar('B');
                    return;
                }
                if (n == "c" && buttonCCC.BackColor == Color.Green)
                {
                    sendchar('C');
                    return;
                }
                if (n == "d" && buttonDDD.BackColor == Color.Green)
                {
                    sendchar('D');
                    return;
                }
                sendchar(n.ToCharArray()[0]);
            }

        }
        private Byte[] outbuf = new Byte[1];
        private void sendchar(char b)
        {
            outbuf[0] = (Byte)b;
            serialPort1.Write(outbuf, 0, 1);

        }
        private Byte in_leds;
        const Byte CMDI_CLEARBUF = 0x0F;
        const Byte CMDO_DISPLAYBTN = 0x02;
        const Byte CMDO_LCD_DATA = 0x04;
        const Byte CMDO_LCD_COMM = 0x06;
        const Byte CMDO_LCD_INIT = 0x08;
        const Byte CMDO_LCD_CLS = 0x0A;
        const Byte CMDO_LCD_AUTO = 0x0C;
        const Byte CMDO_LCD_REVERSE = 0x0E;
        const Byte CMDO_LCD_CLSBLK = 0x10;
        const Byte CMDO_LCD_RECT = 0x12;

        const Byte KEY_INVALID = 0xff;

        const Byte KEY_BTN1 = (Byte)'a';
        const Byte KEY_BTN2 = (Byte)'b';
        const Byte KEY_BTN3 = (Byte)'c';
        const Byte KEY_BTN4 = (Byte)'d';

        const Byte CLR_BTN1 = (Byte)'A';
        const Byte CLR_BTN2 = (Byte)'B';
        const Byte CLR_BTN3 = (Byte)'C';
        const Byte CLR_BTN4 = (Byte)'D';
        private Byte push_cnt = 0;
        private Byte head = 0xff;
        private void pushchar(Byte c)
        {

            if (head == 0xff) //searching for command header
            {
                head = c;
                if (head == CMDO_DISPLAYBTN ||
                    head == CMDO_LCD_DATA ||
                    head == CMDO_LCD_COMM ||
                    head == CMDO_LCD_AUTO ||
                    head == CMDO_LCD_REVERSE ||
                    head == CMDO_LCD_CLSBLK ||
                    head == CMDO_LCD_RECT)
                {
                    push_cnt = 0;
                    return;
                }
                if (head == CMDO_LCD_INIT)
                {
                    LCD_Init();
                    head = 0xff;
                    return;
                }
                if (head == CMDO_LCD_CLS)
                {
                    LCD_Init();
                    head = 0xff;
                    return;
                }

                head = 0xff;
                return;
            }
            else
            {
                if (head == CMDO_DISPLAYBTN)
                {
                    in_leds = c;
                    leds();
                    head = 0xff;
                    return;
                }
                if (head == CMDO_LCD_DATA)
                {
                    LCD_Data(c);
                    head = 0xff;
                    return;
                }
                if (head == CMDO_LCD_COMM)
                {
                    LCD_Comm(c);
                    head = 0xff;
                    return;
                }
                if (head == CMDO_LCD_AUTO)
                {
                    LCD_AutoWrite(c);
                    head = 0xff;
                    return;
                }
                if (head == CMDO_LCD_REVERSE || head == CMDO_LCD_CLSBLK || head == CMDO_LCD_RECT)
                {
                    push_cnt++;
                    if (push_xywh(push_cnt, c) == 1)
                    {
                        push_cnt = 0;
                        head = 0xff;
                    }
                    return;
                }
                head = 0xff;
                return;
            }
	


        }
        private void LCD_Init()
        {
            for (int i = 0; i < bits.Length; i++)
                bits[i] = 0;
        }
        private UInt16 addr;
        private Byte[] bits = new Byte[LCD_HEIGHT * LCD_WIDTH];
        private Byte d1;
        private Byte d2;
        private Byte d3;
        private void LCD_Data(Byte c)
        {
            d1 = d2;
            d2 = d3;
            d3 = c;
        }

        private void LCD_Comm(Byte c)
        {
            if (c == 0x24) //address is set
            {
                addr = (UInt16)(d3 * 256 + d2);
                addr = (UInt16)(addr -LCD_GRAPH_HOME_ADDR);
                return;
            }
            if (c == 0xb2) //update the display
                doupdate = true;
        }
        private StringBuilder lcd = new StringBuilder();
        public void UpdateDisplay()
        {
            lcd.Remove(0,lcd.Length);
            for(int j = 0;j < LCD_HEIGHT; j++)
            {
                for (int k = 0; k < LCD_WIDTH; k++)
                {

                    for (int i = 7; i >=0; i--)
                    {
                        
                        if ((bits[j*LCD_WIDTH + k] & (Byte)(1 << i)) == 0)
                            lcd.Append("＿");
                        else
                            lcd.Append("■");
                    }
                }
                lcd.Append("\r\n");
            }
            richTextBox1.Text = lcd.ToString();
        }
        private void LCD_AutoWrite(Byte c)
        {
            bits[addr] = c;
            addr++;

        }
        private void LCD_WriteInt(UInt16 data,Byte comm)
        {
            LCD_Data((Byte)(data % 256));
            LCD_Data((Byte)(data >>8));
            LCD_Comm(comm);
        }
        private const UInt16 LCD_GRAPH_HOME_ADDR = 240;
        private const UInt16 LCD_WIDTH = 32;
        private const UInt16 LCD_HEIGHT = 64;

        private void LCD_ClsBlock(Byte x, Byte y, Byte x2, Byte y2)
        {
            uint laddr;
            Byte i, j;
            Byte c;

            laddr = (uint)(LCD_GRAPH_HOME_ADDR + LCD_WIDTH * (UInt16)y + (UInt16)(x >> 3));

            for (j = y; j < y2; j++)
            {
                for (i = 0; i < (Byte)((x2-x) >> 3); i++)
                {
                    LCD_WriteInt((UInt16)(laddr + i), 0x24);
                    c = bits[laddr - LCD_GRAPH_HOME_ADDR + i];
                    c = 0;

                    LCD_AutoWrite(c);
                }
                laddr += LCD_WIDTH;
            }
            LCD_Comm(0xb2);
        }
        private void LCD_Putpixel(Byte x, Byte y)
        {
            UInt16 sa = (UInt16)(y * LCD_WIDTH + (x>>3));
            bits[sa] = (Byte)(bits[sa] | (Byte)(1<<(x%8)));
        }
        private void LCD_ReverseRect(Byte x, Byte y, Byte w, Byte h)
        {
            uint laddr;
            Byte i, j;
            Byte c;


            laddr = (uint)(LCD_GRAPH_HOME_ADDR + LCD_WIDTH * (UInt16)y + (UInt16)(x >> 3));

            for (j = 0; j < h; j++)
            {
                for (i = 0; i < w; i++)
                {
                    LCD_WriteInt((UInt16)(laddr + i), 0x24);
                    c = bits[laddr - LCD_GRAPH_HOME_ADDR + i];
                    c = (Byte)~c;

                    LCD_AutoWrite(c);
                }
                laddr += LCD_WIDTH;
            }
            LCD_Comm(0xb2);
        }

        private void LCD_LineV(Byte x, Byte y1, Byte y2)
        {
            Byte i;
            //LCD_CE = 0;
            for (i = y1; i < y2; i++)
            {
                LCD_Putpixel(x, i);
            }

        }

        private void LCD_LineXX(Byte x1, Byte x2, Byte y)
        {
            Byte i;
            for (i = x1; i < x2; i++)
            {
                LCD_Putpixel(i, y);
            }
        }
        private void LCD_Rectange(Byte x1, Byte y1, Byte x2, Byte y2)
        {
            LCD_LineXX(x1, x2, y1);
            LCD_LineXX(x1, x2, y2);
            LCD_LineV(x1, y1, y2);
            LCD_LineV(x2, y1, y2);
            doupdate = true;
        }
        private Byte lx, ly, lw, lh;
        private Byte push_xywh(Byte pos, Byte val)
        {
            if (pos == 1)
            {
                lx = val;
                return 0;
            }
            if (pos == 2)
            {
                ly = val;
                return 0;
            }
            if (pos == 3)
            {
                lw = val;
                return 0;
            }
            if (pos == 4)
            {
                lh = val;
                
                
                if (head == CMDO_LCD_REVERSE)
                    LCD_ReverseRect(lx, ly, lw, lh);
                if (head == CMDO_LCD_CLSBLK)
                    LCD_ClsBlock(lx, ly, lw, lh);
                if (head == CMDO_LCD_RECT)
                    LCD_Rectange(lx, ly, lw, lh);
                
                return 1;
            }
            return 1;
        }

        private void leds()
        {
            if (in_leds != KEY_INVALID)
            {
                if (in_leds == KEY_BTN1)
                    buttona.BackColor = Color.Green;
                if (in_leds == KEY_BTN2)
                    buttonb.BackColor = Color.Green;
                if (in_leds == KEY_BTN3)
                    buttonCCC.BackColor = Color.Green;
                if (in_leds == KEY_BTN4)
                    buttonDDD.BackColor = Color.Green;

                if (in_leds == CLR_BTN1)
                    buttona.BackColor = Color.Gray;
                if (in_leds == CLR_BTN2)
                    buttonb.BackColor = Color.Gray;
                if (in_leds == CLR_BTN3)
                    buttonCCC.BackColor = Color.Gray;
                if (in_leds == CLR_BTN4)
                    buttonDDD.BackColor = Color.Gray;

                in_leds = KEY_INVALID;
            }

        }

        private void Form1_Load(object sender, EventArgs e)
        {

        }

        private void richTextBox1_TextChanged(object sender, EventArgs e)
        {

        }
    }
}
