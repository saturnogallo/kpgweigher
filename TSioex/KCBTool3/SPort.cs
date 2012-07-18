using System;
using System.Collections.Generic;
using System.Text;
using System.IO.Ports;
using System.IO;
using System.Windows.Forms;
using System.Threading;

namespace KCBTool3
{
    enum RF_STATE
    {
        RF_IDLE,
        RF_HEAD1,
        RF_HEAD2,
        RF_ADDRFROM,
        RF_ADDRTO,
        RF_CMD,
        RF_DATALEN,
        RF_DATABUF,
        RF_CKSUM
    }
    enum PortStatus
    {
        IDLE = 0,
        BUSY,
        ERROR,
        CLOSED
    }
    
    internal class FrameBuffer
    {
        public RF_STATE flag;
        public byte addr;
        public byte cmd;
        public byte datalen; //data length of the frame
        public byte[] data;
        private byte bufcnt;  //data buffer count
        public FrameBuffer()
        {
            flag = RF_STATE.RF_IDLE;
            data = new byte[32];
            bufcnt = 0;
        }
        public UInt32 generate_read_frame(byte[] buf,byte[] readregs,int offset, byte len)
        {
            
            buf[1] = 0xfe;
            buf[2] = 0x68;
            buf[3] = 0x00;
            buf[4] = addr;
            buf[5] = (byte)'R';
            buf[6] = len;
            cmd = buf[5];
            
            for (int i = 0; i < len;i++ )
            {
                data[i] = readregs[i + offset];
                buf[7 + i] = readregs[i + offset];
            }
            datalen = len;
            buf[7 + len] = get_cksum();
            buf[0] = (byte)(7 + len); //total frame length
            return buf[0];
        }
        public UInt32 generate_write_frame(byte[] buf, byte[] writeregs, byte[] writeval, int offset, byte len)
        {
            buf[1] = 0xfe;
            buf[2] = 0x68;
            buf[3] = 0x00;
            buf[4] = addr;
            buf[5] = (byte)'W';
            buf[6] = (byte)(2 * len);
            cmd = buf[5];
            
            for (int i = 0; i < len; i++)
            {
                data[2 * i] = writeregs[i+offset];
                data[2 * i + 1] = writeval[i+offset];
                buf[7 + 2 * i] = writeregs[i+offset];
                buf[7 + 2 * i + 1] = writeval[i+offset];
            }
            datalen = (byte)(2 * len);
            buf[7 + 2*len] = get_cksum();
            buf[0] = (byte)(7 + 2 * len); //total frame length
            return buf[0];
        }
        
        public byte get_cksum()
        {
            UInt32 sum = (UInt32)(0xfe)+ (UInt32)(0x68) + (UInt32)addr + (UInt32)(0x00) + (UInt32)cmd + (UInt32)datalen;
            for (int i = 0; i < datalen; i++)
            {
                sum = sum + data[i];
            }
            sum = 256 - (sum % 256);
            return (byte)sum;
        }
        public void ResetFlag()
        {
            flag = RF_STATE.RF_IDLE;
        }
        public void pushc(byte c)
        {
            if (flag == RF_STATE.RF_CKSUM)
            {
                throw new Exception("unexpected incoming char before last frame is handled.");
                
            }
            if (flag == RF_STATE.RF_DATABUF)
            {
                if (c == get_cksum())
                {
                    flag = RF_STATE.RF_CKSUM; //means we are handling command
                    return;
                }
            }
            if (flag == RF_STATE.RF_DATALEN)
            {
                try
                {
                    data[bufcnt] = c;
                }
                catch
                {
                    Program.MsgShow("so weird " + bufcnt.ToString() + ">" + data.Length);
                }
                bufcnt++;
                if (datalen <= bufcnt)
                {
                    flag = RF_STATE.RF_DATABUF;
                }
                return;
            }
            if (flag == RF_STATE.RF_CMD)
            {
                if ((c > 0) && (c < data.Length)) //valid frame
                {
                    bufcnt = 0;
                    datalen = c;
                    flag = RF_STATE.RF_DATALEN;
                }
                else
                {
                    flag = RF_STATE.RF_IDLE;
                }
                return;
            }
            if (flag == RF_STATE.RF_ADDRTO)
            {
                cmd = c;
                flag = RF_STATE.RF_CMD;
                return;
            }
            if (flag == RF_STATE.RF_ADDRFROM)
            {
                if (c == 0x00) //send to pc node
                {
                    flag = RF_STATE.RF_ADDRTO;
                }
                return;
            }
            if (flag == RF_STATE.RF_HEAD2)
            {
                addr = c;
                flag = RF_STATE.RF_ADDRFROM;
                return;
            }
            if (flag == RF_STATE.RF_HEAD1)
            {
                if (c == 0x68)
                {
                    flag = RF_STATE.RF_HEAD2;
                    return;
                }
            }
            if (flag == RF_STATE.RF_IDLE)
            {
                if (c == 0xfe)
                {
                    flag = RF_STATE.RF_HEAD1;
                    return;
                }
            }

            flag = RF_STATE.RF_IDLE;
        }
    }

    /*
     * SerialPort will have a buffer to store the command, and a timeout constant value, all the command will be set in a list and sent out one by one.
     * command will have a node id , each node should control the timeout themselves, after command is sent out SPort will 
     * notify the node , and when echo frame is received, notification will be sent out too
    */
    internal delegate void IOInFrameHandler(FrameBuffer frm);
    internal delegate void IOTimeoutHandler(byte[] last_cmd);
    
    internal class SPort
    {
        private Queue<byte[]> CmdToSend; //Command list of register read/write waiting for sendout
        public Dictionary<byte, IOInFrameHandler> InFrameHandlers;  //handler for incoming frame, key is the address of the node
        
        public PortStatus Status { get; set; }
        private FrameBuffer ifrm;
        private SerialPort _serial;
        private System.Threading.Timer _timer;
        byte[] rbuf = new byte[500];
        public void Close()
        {
            _serial.Close();
            Status = PortStatus.CLOSED;
        }
        private void _checkport(object state)
        {
            int size = 0;
            if (Status == PortStatus.CLOSED)
                return;
            lock(_serial)
            {
                if (_serial.BytesToRead > 0)
                {
                    size = _serial.Read(rbuf, 0, _serial.BytesToRead);
                }
            }
            if(size <= 0)               
                return;
                int p = 0;
                while (size-- >= 0)
                {
                    ifrm.pushc(rbuf[p++]);
                    if (ifrm.flag == RF_STATE.RF_CKSUM)
                    {
                        if (InFrameHandlers.ContainsKey(ifrm.addr))
                        {
                            InFrameHandlers[ifrm.addr](ifrm);
                        }
                        ifrm.ResetFlag();
                    }
                }
            
            
        }
        public SPort(string PortName, int BaudRate, Parity Parity, int DataBits, StopBits StopBit)
        {
            _serial = new SerialPort(PortName, BaudRate, Parity, DataBits, StopBit);
            _serial.DataReceived +=new SerialDataReceivedEventHandler(_serial_DataReceived);
            _serial.Handshake = Handshake.None;

            
            CmdToSend = new Queue<byte[]>();
            InFrameHandlers = new Dictionary<byte, IOInFrameHandler>();
            
            ifrm = new FrameBuffer();
            ifrm.ResetFlag();

            Status = PortStatus.CLOSED;
        }

        void _serial_ErrorReceived(object sender, SerialErrorReceivedEventArgs e)
        {
            throw new NotImplementedException();
        }

        /*
         * Just send out the command and no use of buffer
         */
        public bool AddVCommand(byte[] cmd) ////cmd[0] would be length of data, command is sent out with no enqueue
        {
            if (Status == PortStatus.CLOSED || Status == PortStatus.ERROR)
                return false;
            lock(CmdToSend)
            {
                if (CmdToSend.Contains(cmd)) //duplicate command
                    return false;
            }
                /*for double FEFE head
                byte l = cmd[0];
                cmd[0] = 0xfe;
                _serial.Write(cmd, 0, l+1);
                cmd[0] = l;
                */
            _serial.Write(cmd, 1, cmd[0]);
            
            return true;
        }

        public bool Open()
        {
            if (Status != PortStatus.CLOSED)
                return true;
            try
            {
                _serial.Open();
            }
            catch (System.Exception e)
            {
                Program.MsgShow(e.Message);
                Status = PortStatus.ERROR;
                return false ;
            }
            if (_serial.IsOpen)
            {
                Status = PortStatus.IDLE;
            }
            _serial.DiscardInBuffer();
            ifrm.ResetFlag();
            return _serial.IsOpen;            	
        }
        public void ResetInFlag()
        {
            _serial.DiscardInBuffer();
            ifrm.ResetFlag();
        }
        private void  _serial_DataReceived(object sender, SerialDataReceivedEventArgs e)
        {
            while (_serial.BytesToRead > 0)
            {
                ifrm.pushc((byte)_serial.ReadByte());
                if (ifrm.flag == RF_STATE.RF_CKSUM)
                {
                    if (InFrameHandlers.ContainsKey(ifrm.addr))
                    {
                        InFrameHandlers[ifrm.addr](ifrm);
                    }
                    ifrm.ResetFlag();
                }
            }
        }
    }
}
