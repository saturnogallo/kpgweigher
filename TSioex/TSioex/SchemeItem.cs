using System;
using System.Linq;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using System.IO;
namespace TSioex
{
    public partial class SchemeItem : UserControl
    {
        public event EventHandler ValidClick;
        public SchemeItem()
        {
            InitializeComponent();
            txtBox.GotFocus += new EventHandler(txtBox_GotFocus);
            picBox.Click += new EventHandler(picBox_Click);
            txtBox.BackColor = SingleModeWnd.bgWindow;
            this.panel1.BackColor = SingleModeWnd.bgWindow;
            this.BackColor = SingleModeWnd.bgWindow;
        }

        void picBox_Click(object sender, EventArgs e)
        {
            this.ValidClick(this, e);
        }

        void txtBox_GotFocus(object sender, EventArgs e)
        {
            picBox.Focus();
        }
        private string _product;
        private void UpdateText()
        {
            txtBox.Text = _product;
        }
        public string Product
        {
            get{
                return _product;
            }
            
            set
            {
                _product = value;
                string path_to_jpg;
                path_to_jpg = ProdNum.baseDir + "\\prodpic\\" + StringResource.language + "\\" + _product + ".jpg";
                if (!File.Exists(path_to_jpg))
                    path_to_jpg = ProdNum.baseDir + "\\prodpic\\default.jpg";
                picBox.Image = (Image)(new Bitmap(path_to_jpg));
                UpdateText();
                this.Invalidate();
            }
        }
    }
}
