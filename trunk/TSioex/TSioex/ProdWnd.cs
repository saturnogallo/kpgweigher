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
    public delegate void ProdClickHandler(string data);
    public partial class ProdWnd : Form
    {
        private static int PageSize = 9;
        public ProdClickHandler phandler;
        private int totalNum = 0;
        private int pageIndex = 1;
        public ProdWnd()
        {
            InitializeComponent();
            imglist = new List<string>();
            if (Directory.Exists(ProdNum.baseDir + "\\prodpic" + StringResource.language + "\\"))
                Directory.CreateDirectory(ProdNum.baseDir + "\\prodpic" + StringResource.language + "\\");
            foreach (string f in Directory.GetFiles(ProdNum.baseDir + "\\prodpic\\" + StringResource.language + "\\", "*.jpg"))
            {
                totalNum++;
                FileInfo fi = new FileInfo(f);
                string nm = fi.Name.Remove(fi.Name.Length - 4, 4).Replace(' ', '_');
                imglist.Add(nm);
            }
            schemeItem1.ValidClick +=new EventHandler(ImageSelected);
            schemeItem2.ValidClick += new EventHandler(ImageSelected);
            schemeItem3.ValidClick += new EventHandler(ImageSelected);
            schemeItem4.ValidClick += new EventHandler(ImageSelected);
            schemeItem5.ValidClick += new EventHandler(ImageSelected);
            schemeItem6.ValidClick += new EventHandler(ImageSelected);
            schemeItem7.ValidClick += new EventHandler(ImageSelected);
            schemeItem8.ValidClick += new EventHandler(ImageSelected);
            schemeItem9.ValidClick += new EventHandler(ImageSelected);
            btn_ret_run.Click +=new EventHandler(btn_ret_run_Click);
            img_pglast.Click +=new EventHandler(lbl_pglast_MouseLeftButtonUp);
            img_pgnext.Click += new EventHandler(lbl_pgnext_MouseLeftButtonUp);
            this.lbl_title.Text = StringResource.str("selectprod");
            UpdateDisplay();
        }
        private List<string> imglist;
        public void Init(ProdClickHandler h)
        {
            this.phandler = h;
            this.Show();
            UpdateDisplay();
        }
        private void ImageSelected(object sender, EventArgs arg)
        {
            if (phandler is ProdClickHandler)
            {
                phandler((sender as SchemeItem).Product);
            }
            this.Hide();
        }

        private void btn_ret_run_Click(object sender, EventArgs e)
        {
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
            if(pageIndex*PageSize <　totalNum)
                pageIndex++;
            UpdateDisplay();
            
        }
        private void UpdateOne(SchemeItem n, int startpos)
        {
            n.Visible =  (startpos < totalNum);
            if(n.Visible)
            {
                n.Product = imglist[startpos];
            }
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