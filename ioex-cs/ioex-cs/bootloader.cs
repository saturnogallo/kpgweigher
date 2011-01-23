using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
using System.IO;
namespace ioex_cs
{
    enum BOOT_CMD
    {
        BOOT_CMD_IDLE =0,
        BOOT_CMD_CHECKCRC = 1,
        BOOT_CMD_PGM_PAGE = 2,
        BOOT_CMD_UPGRADE = 3,
        BOOT_CMD_RUN_APP = 4

    }

    //board address map
    //typedef struct {
    // u8 upgrade_cmd;  "addr"                /* command sent from master to control firmware upgrade in node board */    
    // u8 status;   "board_id"                /* The second byte. This byte is initialized to 'B' when code is in bootloader */
                                              /* PC polls this byte to know whether MEGA16 is executing bootloader or application firmware */
                                              /* 'B' --- bootloader, Valid board id --- application firmware */
    // u16 page_addr; "baudrate"                      /* specify which page to be written */
    // u8 page_buffer[DATA_BUFFER_SIZE]; ""   /* data sent from master board. to be programmed to flash, last 2 bytes are CRC bytes */
    //} BOOT_COMM_INTERFACE;                  /* boot communication interface with master board */
    
    class bootloader
    {
        private SubNode node;
        //todo calculate crc
        private UInt16 crc(byte[] buffer, int size)
        {
            UInt16 c = 0;
            UInt16 b = 0;
            int index = 0;
            byte i;
            while (--size >= 0)
            {
                b = (UInt16)((UInt16)(buffer[index++]) << 8);
                c = (UInt16)(c ^ b);
                i = 8;
                do
                {
                    if (c >= 0x8000)
                        c = (UInt16)((UInt16)(c << 1) ^ 0x1021);
                    else
                        c = (UInt16)(c << 1);
                } while (--i != 0);
            }
            return c;
        }
        private byte poll(string reg, byte quitval)
        {
            byte uc = 2;
            byte timeout;
            do
            {
                timeout = 0;
                node[reg] = null;
                while((node.status == NodeStatus.ST_BUSY) && (timeout++ < 6))
                    Thread.Sleep(50);

                if (node.status == NodeStatus.ST_BUSY)
                {
                    node.status = NodeStatus.ST_IDLE;
                    node[reg] = null;
                }
                if(node[reg].HasValue)
                    uc = (byte)node[reg].Value;

            } while ((uc == quitval));
            return uc;
        }
        public bool download(string fwfile)
        {

            int i = 0;
            byte[] page = new byte[130];
//            page[0] = 0x11;
//            node.writeseq_abs_reg(4, page, 1);

            node["addr"] = null;
            Thread.Sleep(100);
            node["board_id"] = null;
            Thread.Sleep(100);
            while (('B' != node["board_id"].Value) && (i<5))
            {
                /* we are in application firmware now*/
                node["flag_reset"] = 0xA0; /*force node to stay in bootloader after WDT reset*/
                Thread.Sleep(500);  /*update board_id*/
                node["board_id"] = null; /* 500ms is enough for node to reset and power up*/
                i++;
            }
            if (i >= 5)
                return false;            
            
            
            /* Node is waiting for firmware upgrade now*/
            UInt32 page_addr = 0;
            try
            {
                using(FileStream fs = new FileStream(fwfile, FileMode.Open))
                {
                    int count;
                    while(fs.CanRead)
                    {
                        count = fs.Read(page, 0, 128);
                        if (count == 0)
                            break;
                        while (count < 128)
                        {
                            page[count++] = 0x1A; //filled up with eof char
                        }
                        //get crc
                        UInt16 crc_ret = crc(page, count);
                        page[count] = (byte)(crc_ret);
                        page[count + 1] = (byte)(crc_ret>>8);

                        //write page_address
                        node.writebyte_abs_reg(new byte[] { (byte)2, (byte)3 }, new byte[] { (byte)page_addr, (byte)(page_addr >> 8) });

                        node.writeseq_abs_reg(4, page, 1);
                        //todo send data to node
                        node.writeseq_abs_reg(4, page, count+2);

                        node.writebyte_abs_reg(new byte[] { (byte)0 }, new byte[] { (byte)2, (byte)0 });//BOOT_CMD_PGM_PAGE =2
                        
                        if (poll("addr",2) != 0)
                            return false;
                        page_addr += (UInt32)count;
                    }
                }
                node.writebyte_abs_reg(new byte[] { (byte)0 }, new byte[] { (byte)3, (byte)0 }); //BOOT_CMD_UPGRADE = 3
                
                if (poll("addr",2) != 0)
                    return false;
                node.writebyte_abs_reg(new byte[] { (byte)0 }, new byte[] { (byte)4, (byte)0 }); //BOOT_CMD_RUN_APP = 3
                
                i = 0;

                Thread.Sleep(500);  /*update board_id*/
                return (poll("board_id", (byte)('B')) == (byte)'B');

            }

            catch (System.Exception e)
            {
                return false;
            }
            

        }
        public bootloader(SubNode n)
        {
            node = n;
        }
    }
}
