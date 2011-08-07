using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.IO;
namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for ProdNum.xaml
    /// </summary>
    public partial class ProdNum : Window
    {
        public ProdClickHandler phandler;
        private Dictionary<string, string> imglist;
        private UIPacker curr_packer
        {
            get
            {
               App p = (Application.Current as App);
                return p.packers[0];
            }
        }
        public ProdNum()
        {
            InitializeComponent();
            imglist = new Dictionary<string, string>();
        }
        public static string baseDir
        {
            get
            {
                return System.Threading.Thread.GetDomain().BaseDirectory;
            }
        }
        public void Init(ProdClickHandler h,bool CanDelete)
        {
            this.phandler = h;
            this.wrapPanel1.Children.Clear();
            PackerConfig pcfg = new PackerConfig();
            foreach (string id in curr_packer.all_conf.Keys)
            {
                pcfg.FromElement(XElement.Parse(curr_packer.all_conf[id]));
                
                FileInfo fi = new FileInfo(ProdNum.baseDir +"\\prodpic\\"+pcfg.product_desc +".jpg");
                imglist[id] = fi.FullName;

                Label n = new Label();
                ControlTemplate ct = this.FindResource("imgBtn") as ControlTemplate;
                n.Template = ct;
                n.Name = "ID" + id;
                //Product num: Pru
                n.Content = String.Format("{0}:{1}\n{2}:{3}\n{4}:{5}\n{6}:{7}\n{8}:{9}",
                    StringResource.str("product_no"),pcfg.product_no,
                    StringResource.str("product_desc"),pcfg.product_desc,
                    StringResource.str("grp_target"),pcfg.target,
                    StringResource.str("grp_uvar"),pcfg.upper_var,
                    StringResource.str("grp_dvar"),pcfg.lower_var
                    );

                //n.Click += ImageSelected;
                n.ApplyTemplate();
                Rectangle rectimg = ct.FindName("ImageRect", n) as Rectangle;
                rectimg.Name = "Rt" + id;
                Button delBtn = ct.FindName("DelBtn", n) as Button;
                delBtn.Name = "De" + id;
                rectimg.MouseLeftButtonUp += ImageRect_MouseLeftButtonUp;
                delBtn.Click += Delbtn_Click;
                if (!CanDelete)
                    delBtn.Visibility = Visibility.Hidden;
                ImageBrush ib = ct.FindName("ImageFill", n) as ImageBrush;

                ib.ImageSource = new BitmapImage(new Uri(fi.FullName));
                this.wrapPanel1.Children.Add(n);
            }
            this.Show();
        }
        private void ImageSelected(object sender, EventArgs arg)
        {
            if (phandler is ProdClickHandler)
            {
                phandler((sender as Button).Name.Remove(0, 2));
            }
            this.Hide();
        }

        private void btn_ret_run_Click_1(object sender, RoutedEventArgs e)
        {
            this.Hide();
        }

        private void Delbtn_Click(object sender, EventArgs e)
        {
            string id = (sender as Button).Name.Remove(0, 2);
            
            string lastcfg = curr_packer.all_conf.cfg_name;
            if (id == curr_packer.all_conf.cfg_name)
            {
                MessageBox.Show(StringResource.str("cfg_inuse"));
                return;
            }
            curr_packer.all_conf.RemoveConfig(id);
            curr_packer.all_conf.LoadConfig(lastcfg);
            curr_packer.all_conf.SaveConfigToFile();
            Init(phandler,true);
        }

        private void ImageRect_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            Rectangle rect = sender as Rectangle;
            if (rect is  Rectangle)
            {
                if (phandler is ProdClickHandler)
                {
                    phandler((sender as Rectangle).Name.Remove(0, 2));
                }
                this.Hide();
            }
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
