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

    class bootloader
    {
        private SubNode node;
        private byte poll()
        {
            byte uc;
            do
            {
                Thread.Sleep(10);
                node["upgrade_cmd"] = null;
                uc = (byte)node["upgrade_cmd"].Value;
            } while ((uc & 0x0f) == 0);
            return uc;
        }
        public bool download(string fwfile)
        {
            int i = 0;
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
            
            byte[] page = new byte[130];
            /* Node is waiting for firmware upgrade now*/
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
                        //todo calculate crc
                        //page[count] = crc
                        //send data to node
                        node["upgrade_cmd"] = Convert.ToUInt32(BOOT_CMD.BOOT_CMD_PGM_PAGE);
                        if (poll() != 0)
                            return false;
                    }
                }
                node["upgrade_cmd"] = Convert.ToUInt32(BOOT_CMD.BOOT_CMD_UPGRADE);
                if (poll() != 0)
                    return false;
                node["upgrade_cmd"] = Convert.ToUInt32(BOOT_CMD.BOOT_CMD_RUN_APP);
                
                i = 0;

                Thread.Sleep(500);  /*update board_id*/
                while (('B' == node["board_id"].Value) && (i < 5))
                {
                    /* we are in application firmware now*/
                    node["board_id"] = null; /* 500ms is enough for node to reset and power up*/
                    Thread.Sleep(500);  /*update board_id*/
                    i++;
                }
                return (i < 5);
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
