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
using System.Windows.Forms;

namespace ioex_cs
{
    /// <summary>
    /// Interaction logic for History.xaml
    /// </summary>
    public partial class History : Window
    {
        public History()
        {
            InitializeComponent();
            //change the display format of the date time picker
            DateTimePicker dtp = this.startdateHost.Child as DateTimePicker;
            dtp.Format = DateTimePickerFormat.Short;
            dtp = this.enddateHost.Child as DateTimePicker;
            dtp.Format = DateTimePickerFormat.Short;
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {

        }

        private void query_Click(object sender, RoutedEventArgs e)
        {
        }

        private void return_Click(object sender, RoutedEventArgs e)
        {
            Topmost = false;
            Hide();
        }
    }
}
