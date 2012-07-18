using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.IO;
namespace TSioex
{
    public partial class ProdNum : Form
    {
        public static bool bReadOnly = true;
        public static string baseDir = "\\NANDFlash\\TSioex";
        //public static string baseDir = "\\Storage Card\\TSioex";
        public ProdNum()
        {
            InitializeComponent();
            imglist = new List<string>();
            idlist = new List<string>();
            img_pglast.Click += new EventHandler(lbl_pglast_MouseLeftButtonUp);
            img_pgnext.Click += new EventHandler(lbl_pgnext_MouseLeftButtonUp);
            this.lbl_title.Text = StringResource.str("selectprodnum");
            btn_ret_run.Click += new EventHandler(btn_ret_run_Click_1);
            schemeItem1.ValidClick += new EventHandler(ImageRect_MouseLeftButtonUp);
            schemeItem2.ValidClick += new EventHandler(ImageRect_MouseLeftButtonUp);
            schemeItem3.ValidClick += new EventHandler(ImageRect_MouseLeftButtonUp);
            schemeItem4.ValidClick += new EventHandler(ImageRect_MouseLeftButtonUp);
            schemeItem5.ValidClick += new EventHandler(ImageRect_MouseLeftButtonUp);
            schemeItem6.ValidClick += new EventHandler(ImageRect_MouseLeftButtonUp);
            schemeItem7.ValidClick += new EventHandler(ImageRect_MouseLeftButtonUp);
            schemeItem8.ValidClick += new EventHandler(ImageRect_MouseLeftButtonUp);
            schemeItem9.ValidClick += new EventHandler(ImageRect_MouseLeftButtonUp);
            schemeItem1.DeleteClick += new EventHandler(Delbtn_Click);
            schemeItem2.DeleteClick += new EventHandler(Delbtn_Click);
            schemeItem3.DeleteClick += new EventHandler(Delbtn_Click);
            schemeItem4.DeleteClick += new EventHandler(Delbtn_Click);
            schemeItem5.DeleteClick += new EventHandler(Delbtn_Click);
            schemeItem6.DeleteClick += new EventHandler(Delbtn_Click);
            schemeItem7.DeleteClick += new EventHandler(Delbtn_Click);
            schemeItem8.DeleteClick += new EventHandler(Delbtn_Click);
            schemeItem9.DeleteClick += new EventHandler(Delbtn_Click);
        }
        private static int PageSize = 9;
        public ProdClickHandler phandler;
        private int totalNum = 0;
        private int pageIndex = 1;
        private List<string> imglist;
        private List<string> idlist;
        private UIPacker curr_packer
        {
            get
            {
                return Program.packers[0];
            }
        }
        public void Init(ProdClickHandler h, bool CanDelete)
        {
            this.phandler = h;
            PackerConfig pcfg = new PackerConfig();
            totalNum = 0;
            idlist.Clear();
            imglist.Clear();
            foreach (string id in curr_packer.pkg_confs.Keys)
            {
                totalNum++;
                pcfg.FromElement(curr_packer.pkg_confs[id]);
                FileInfo fi;
                if (File.Exists(ProdNum.baseDir + "\\prodpic\\" + StringResource.language + "\\" + pcfg.product_desc + ".jpg"))
                {
                    fi = new FileInfo(ProdNum.baseDir + "\\prodpic\\" + StringResource.language + "\\" + pcfg.product_desc + ".jpg");
                }
                else
                {
                    fi = new FileInfo(ProdNum.baseDir + "\\prodpic\\default.jpg");
                }
                idlist.Add(id);
                imglist.Add(fi.Name.Remove(fi.Name.Length - 4, 4).Replace(' ', '_'));

                bReadOnly = !CanDelete;
            }
            UpdateDisplay();
            this.Show();
        }


        private void btn_ret_run_Click_1(object sender, EventArgs e)
        {
            this.Hide();
        }

        private void Delbtn_Click(object sender, EventArgs e)
        {
            string id = (sender as SchemeItem2).Id;

            string lastcfg = curr_packer.pkg_confs.cfg_name;
            if (id == curr_packer.pkg_confs.cfg_name)
            {
                Program.MsgShow(StringResource.str("cfg_inuse"));
                return;
            }
            curr_packer.pkg_confs.RemoveConfig(id);
            curr_packer.pkg_confs.LoadConfig(lastcfg);
            curr_packer.pkg_confs.SaveConfigToFile();
            Init(phandler, true);
        }

        private void ImageRect_MouseLeftButtonUp(object sender, EventArgs e)
        {

                if (phandler is ProdClickHandler)
                {
                    phandler((sender as SchemeItem2).Id);
                }
                this.Hide();
        }

        private void lbl_pglast_MouseLeftButtonUp(object sender, EventArgs e)
        {
            if (pageIndex > 1)
                pageIndex--;
            UpdateDisplay();
        }

        private void lbl_pgnext_MouseLeftButtonUp(object sender, EventArgs e)
        {
            if (pageIndex * PageSize < totalNum)
                pageIndex++;
            UpdateDisplay();
        }
        private void UpdateOne(SchemeItem2 n,int startpos)
        {
            if (startpos >= totalNum)
            {
                n.Visible = false;
                return;
            }
            n.Visible = true ;
            
                PackerConfig pcfg = new PackerConfig();
                pcfg.FromElement(curr_packer.pkg_confs[idlist[startpos]]);

                n.Product = imglist[startpos];
                n.Text = String.Format("{0}:{1}\r\n{2}:{3}\r\n{4}:{5}\r\n{6}:{7}\r\n{8}:{9}",
                    StringResource.str("product_no"), pcfg.product_no,
                    StringResource.str("product_desc"), pcfg.product_desc,
                    StringResource.str("grp_target"), pcfg.target,
                    StringResource.str("grp_uvar"), pcfg.upper_var,
                    StringResource.str("grp_dvar"), pcfg.lower_var
                );
                n.Id = idlist[startpos];

        }
        private void UpdateDisplay()
        {
            int startpos = (pageIndex - 1) * PageSize; //start from 0
            lbl_page.Text = pageIndex.ToString() + "/" + ((totalNum / PageSize) + 1).ToString();
            UpdateOne(schemeItem1, startpos++);
            UpdateOne(schemeItem2, startpos++);
            UpdateOne(schemeItem3, startpos++);
            UpdateOne(schemeItem4, startpos++);
            UpdateOne(schemeItem5, startpos++);
            UpdateOne(schemeItem6, startpos++);
            UpdateOne(schemeItem7, startpos++);
            UpdateOne(schemeItem8, startpos++);
            UpdateOne(schemeItem9, startpos++);

        }
    }
}