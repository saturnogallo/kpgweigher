using System;

using System.Collections.Generic;
using System.Text;
using System.IO;
using System.Xml;
using System.ComponentModel;
using System.Data;

namespace RT9609CS
{
    class Config
    {
        private DataSet cfgset; //configuration dataset
        /*
         * Has four table with following structure, 
         * 1)   Probe   Setting:
         *      Id, ProbeConvertType, ProbeSerialNumber, StdR,RefI, 3Wire, Sqrt, CoefId
         * 2)   Coef    Setting
         *      Id, Range, Wxx,A/B/Cxxx
         * 3)   Std     Setting
         *      Id,  SetName,   1/10/100/1K/10K (ohm,a,b)
         * 4)   Filter Setting
         *      Id, Yuzhi, integrater, sample interval, average method, response time, speed
         */
        public Config(string cfgxml)
        {
            strXmlPath = cfgxml;
            cfgset = new DataSet();
            LoadConfig();
        }
        string strXmlPath; //configuration path
        public void SaveConfig()
        {
            cfgset.WriteXml(strXmlPath);
        }
        public void LoadConfig()
        {
            cfgset.ReadXml(strXmlPath);
        }
        public void SetField(string tbl, string identify, string col, string val)
        {
            if (cfgset.Tables.Contains(tbl))
            {
                DataRow[] drs = cfgset.Tables[tbl].Select("Id='" + identify + "'");
                if (drs.Length == 0)
                {
                    DataRow dr = cfgset.Tables[tbl].NewRow();
                    dr["Id"] = identify;
                    dr[col] = val;
                    cfgset.Tables[tbl].Rows.Add(dr);
                }
                else
                {
                    for (int i = 0; i < cfgset.Tables[tbl].Rows.Count; i++)
                    {
                        if (cfgset.Tables[tbl].Rows[i]["Id"].ToString() == identify)
                        {
                            cfgset.Tables[tbl].Rows[i][col] = val;
                            break;
                        }
                    }
                    
                }
                cfgset.AcceptChanges();
            }
        }
        public void CopyRecord(string tbl, string old_id, string new_id)
        {
            if (!cfgset.Tables.Contains(tbl))
                return;
            DataRow dr;

            if (old_id != "")
            {
                DataRow[] drs = cfgset.Tables[tbl].Select("Id='" + old_id + "'");
                if (drs.Length == 0)
                    return;
                dr = drs[0];
            }
            else
                dr = cfgset.Tables[tbl].Rows[0];

            dr["Id"] = new_id;
            cfgset.Tables[tbl].Rows.Add(dr);
            cfgset.AcceptChanges();
        }
        public void GetNames(string tbl, out string[] ids)
        {
            ids = null;
            if (cfgset.Tables.Contains(tbl))
            {
                ids = new string[cfgset.Tables[tbl].Rows.Count];
                for (int i = 0; i < cfgset.Tables[tbl].Rows.Count; i++)
                {
                    ids[i] = cfgset.Tables[tbl].Rows[i]["Id"].ToString();
                }
            }
        }
        public object GetField(string tbl, string identify, string col)
        {
            if (cfgset.Tables.Contains(tbl) && cfgset.Tables[tbl].Columns.Contains(col))
            {
                DataRow[] drs = cfgset.Tables[tbl].Select("Id='" + identify + "'");
                if (drs.Length > 0)
                {
                    if(col[0] == 's')   //string
                        return drs[0][col].ToString();
                    if (col[0] == 'i')   //int
                        return Int32.Parse(drs[0][col].ToString());
                    if (col[0] == 'u')  //uint
                        return UInt32.Parse(drs[0][col].ToString());
                    if (col[0] == 'b')   //bool
                        return Boolean.Parse(drs[0][col].ToString());
					if (col[0] == 'd')  //double
                        return Double.Parse(drs[0][col].ToString());
                }
            }
            return null;
        }
    }
}
