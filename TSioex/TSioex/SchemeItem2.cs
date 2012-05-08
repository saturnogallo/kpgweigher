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
    public partial class SchemeItem2 : UserControl
    {
        public event EventHandler ValidClick;
        public event EventHandler DeleteClick;
        public SchemeItem2()
        {
            InitializeComponent();
            txtBox.GotFocus += new EventHandler(txtBox_GotFocus);
            this.BackColor = Color.Gray;
            this.ForeColor = Color.Gray;
            txtBox.BackColor = Color.Gray;
            delBtn.bgColor = Color.Gray;
            picBox.Click += new EventHandler(picBox_Click);
            delBtn.Text = StringResource.str("delete");
            delBtn.SetStyle(Color.Goldenrod, MyButtonType.roundButton);
            delBtn.ValidClick += new EventHandler(delBtn_ValidClick);
        }

        void delBtn_ValidClick(object sender, EventArgs e)
        {
            this.DeleteClick(this, e);
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
            delBtn.Visible = !ProdNum.bReadOnly;
            txtBox.Text = base.Text;

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
        private string _id;
        public string Id
        {
            get
            {
                return _id;
            }
            set
            {
                _id = value;
                delBtn.Visible = (_id != "");
                UpdateText();
            }
        }
    }
}
