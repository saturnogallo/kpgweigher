using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.DirectoryServices;
using System.IO;

namespace ioex_cs
{
    public delegate void ProdClickHandler(string data);
    /// <summary>
    /// Interaction logic for ProdWnd.xaml
    /// </summary>
    public partial class ProdWnd : Window
    {
        public ProdClickHandler phandler;
        public ProdWnd()
        {
            InitializeComponent();
            imglist = new Dictionary<string, string>();
            if (Directory.Exists(ProdNum.baseDir + "\\prodpic" + StringResource.language + "\\" ))
                Directory.CreateDirectory(ProdNum.baseDir + "\\prodpic" + StringResource.language + "\\");
            foreach (string f in Directory.GetFiles(ProdNum.baseDir + "\\prodpic\\" + StringResource.language + "\\", "*.jpg"))
            {
                FileInfo fi = new FileInfo(f);
                string nm = fi.Name.Remove(fi.Name.Length-4,4).Replace(' ','_');
                imglist[nm] = fi.FullName;
                Button n = new Button();
                ControlTemplate ct = this.FindResource("imgBtn") as ControlTemplate;
                n.Template = ct;
                n.Name = "ID"+nm;
                n.Content = nm;
                n.Click += ImageSelected;
                n.ApplyTemplate();
                ImageBrush ib = ct.FindName("ImageFill",n) as ImageBrush;
                
                ib.ImageSource = new BitmapImage(new Uri(fi.FullName));
                this.wrapPanel1.Children.Add(n);
            }
        }
        private Dictionary<string, string> imglist;
        public void Init(ProdClickHandler h)
        {
            this.phandler = h;
            this.Show();
        }
        private void ImageSelected(object sender, EventArgs arg)
        {
            if (phandler is ProdClickHandler)
            {
                phandler((sender as Button).Name.Remove(0,2));
            }
            this.Hide();
        }

        private void btn_ret_run_Click(object sender, RoutedEventArgs e)
        {
            this.Hide();
        }


        private void lbl_pglast_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            this.scrollViewer1.PageUp();
        }

        private void lbl_pgnext_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            this.scrollViewer1.PageDown();
        }
    }
}
