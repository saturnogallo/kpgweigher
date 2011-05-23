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

namespace ioex_cs
{
    public delegate void KbdDataHandler(string param,string data);
    /// <summary>
    /// Interaction logic for kbd.xaml
    /// </summary>
    public partial class kbd : Window
    {
        private StringBuilder data;
        
        public string note {get;set;}
        public string param {get;set;} //parameter for window in callback
        public bool password { get; set; }
        public KbdDataHandler kbdhandler;
        
        public kbd()
        {
            InitializeComponent();
            data = new StringBuilder("", 10);
            note = "Note";
            Topmost = true;
            password = false;
            Hide();
        }
        public void Init(string init_note,string init_param, bool init_pwd, KbdDataHandler handler)
        {
            data.Remove(0, data.Length);
            note = init_note;
            param = init_param;
            password = init_pwd;
            kbdhandler = handler;
            UpdateData();
            this.Show();
        }
        private void UpdateData()
        {
            if (!password)
            {
                datalabel.Content = data.ToString();
            }else{
                datalabel.Content = new String('*', data.Length);
            }
            
            notelabel.Content = note;
        }
        private void addchar(char c)
        {
            if (data.Length >= 10)
                return;
            if (c >= '0' && c <= '9')
            {
                data.Append(c);
                
            }
            if (c == '-')
            {
                if (data.Length > 1 && (data[0] != '-'))
                {
                    data.Insert(0, '-');
                }
                
            }
            if (c == '.')
            {
                for (int i = 0; i < data.Length;i++ )
                {
                    if (data[i] == '.')
                    {
                        return;
                    }
                }
                if (data.Length >= 1 && (data[data.Length - 1] != '-'))
                {
                    data.Append(c);
                }
                
            }
            UpdateData();
        }
        private void btn_num1_Click(object sender, RoutedEventArgs e)
        {
            addchar('1');
        }

        private void btn_num2_Click(object sender, RoutedEventArgs e)
        {
            addchar('2');
        }

        private void btn_num3_Click(object sender, RoutedEventArgs e)
        {
            addchar('3');
        }

        private void btn_num4_Click(object sender, RoutedEventArgs e)
        {
            addchar('4');
        }

        private void btn_num5_Click(object sender, RoutedEventArgs e)
        {
            addchar('5');
        }

        private void btn_num6_Click(object sender, RoutedEventArgs e)
        {
            addchar('6');
        }

        private void btn_num7_Click(object sender, RoutedEventArgs e)
        {
            addchar('7');
        }

        private void btn_num8_Click(object sender, RoutedEventArgs e)
        {
            addchar('8');
        }

        private void btn_num9_Click(object sender, RoutedEventArgs e)
        {
            addchar('9');
        }

        private void btn_num0_Click(object sender, RoutedEventArgs e)
        {
            addchar('0');
        }

        private void btn_numP_Click(object sender, RoutedEventArgs e)
        {
            addchar('.');
        }

        private void btn_numN_Click(object sender, RoutedEventArgs e)
        {
            addchar('-');
        }

        private void btn_ok_Click(object sender, RoutedEventArgs e)
        {
            string llimit = StringResource.str(param+"_llimit");
            string ulimit = StringResource.str(param + "_ulimit");
            
            if (data.ToString() == "")
            {
                MessageBox.Show(StringResource.str("emptydata"));
                return;
            }
            try
            {
                if (llimit == "Invalid String Key") //no limits setting
                {
                    kbdhandler(param, data.ToString());
                    Hide();
                    return;
                }

                if (llimit.IndexOf(".") < 0) //int case
                {
                        Int32 ll = Int32.Parse(llimit);
                        Int32 hl = Int32.Parse(ulimit);

                        if (Int32.Parse(data.ToString()) < ll || Int32.Parse(data.ToString()) > hl)
                            throw new Exception("input out of range");
                }
                else
                {
                    double ll = double.Parse(llimit);
                    double hl = double.Parse(ulimit);
                    if (double.Parse(data.ToString()) < ll || double.Parse(data.ToString()) > hl)
                        throw new Exception("input out of range");
                }
                kbdhandler(param, data.ToString());
            }
            catch(Exception err)
            {
                MessageBox.Show(err.Message);
                return;
            }
            
            Hide();
        }

        private void btn_clr_Click(object sender, RoutedEventArgs e)
        {
            data.Remove(0, data.Length);
            UpdateData();
        }

        private void btn_backspace_Click(object sender, RoutedEventArgs e)
        {
            if (data.Length > 0)
            {
                data.Remove(data.Length - 1, 1);
                UpdateData();
            }
        }

        private void btn_quit_Click(object sender, RoutedEventArgs e)
        {
            Hide();
        }
    }
}
