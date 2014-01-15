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

namespace QJApp
{
    /// <summary>
    /// Interaction logic for SysConfig.xaml
    /// </summary>
    public partial class SysConfig : Window
    {
        private InfoManager<SysInfo> sysinfomgr;
        private SysInfo sys_info;
        public IEnumerable<int> gripaddrs;
        public SysConfig()
        {
            sysinfomgr = new InfoManager<SysInfo>("SysInfo", SysInfo.CreateNew);
            sys_info = sysinfomgr["Default"];
            gripaddrs = Enumerable.Range(0, 32);
            InitializeComponent();
            this.cb_MasterAddr.ItemsSource = Enumerable.Range(1, 32);
            this.cb_Scan1Addr.ItemsSource = gripaddrs;
            this.cb_Scan2Addr.ItemsSource = gripaddrs;
            this.cb_OvenAddr.ItemsSource = gripaddrs;
            this.cb_CurrExtAddr.ItemsSource = gripaddrs;

            this.cb_MasterType.ItemsSource = new string[] { "QJ55A" };
            this.cb_Scan1Type.ItemsSource = new string[] { "RT2010" };
            this.cb_Scan2Type.ItemsSource = new string[] { "RT2010" };
            this.cb_OvenType.ItemsSource = new string[] { "RT25" };
            this.cb_CurrExtType.ItemsSource = new string[] { "RT10" };





            this.cb_MasterAddr.SelectedValue = sys_info.iMasterAddr;
            this.cb_Scan1Addr.SelectedValue = sys_info.iScanner1Addr;
            this.cb_Scan2Addr.SelectedValue = sys_info.iScanner2Addr;
            this.cb_OvenAddr.SelectedValue = sys_info.iOvenAddr;
            this.cb_CurrExtAddr.SelectedValue = sys_info.iCurrentExtAddr;

            this.cb_MasterAddr.SelectionChanged += new SelectionChangedEventHandler((a, b) => { sys_info.iMasterAddr = Int32.Parse(cb_MasterAddr.SelectedValue.ToString()); });
            this.cb_Scan1Addr.SelectionChanged += new SelectionChangedEventHandler((a, b) => { sys_info.iScanner1Addr = Int32.Parse(cb_Scan1Addr.SelectedValue.ToString()); });
            this.cb_Scan2Addr.SelectionChanged += new SelectionChangedEventHandler((a, b) => { sys_info.iScanner1Addr = Int32.Parse(cb_Scan2Addr.SelectedValue.ToString()); });
            this.cb_OvenAddr.SelectionChanged += new SelectionChangedEventHandler((a, b) => { sys_info.iOvenAddr = Int32.Parse(cb_OvenAddr.SelectedValue.ToString()); });
            this.cb_CurrExtAddr.SelectionChanged += new SelectionChangedEventHandler((a, b) => { sys_info.iCurrentExtAddr = Int32.Parse(cb_CurrExtAddr.SelectedValue.ToString()); });


            this.cb_MasterType.SelectedValue = sys_info.sMasterType;
            this.cb_Scan1Type.SelectedValue = sys_info.sScanner1Type;
            this.cb_Scan2Type.SelectedValue = sys_info.sScanner2Type;
            this.cb_OvenType.SelectedValue = sys_info.sOvenType;
            this.cb_CurrExtType.SelectedValue = sys_info.sCurrentExtType;

            this.cb_MasterType.SelectionChanged += new SelectionChangedEventHandler((a, b) => { sys_info.sMasterType = cb_MasterType.SelectedValue.ToString(); });
            this.cb_Scan1Type.SelectionChanged += new SelectionChangedEventHandler((a, b) => { sys_info.sScanner1Type = cb_Scan1Type.SelectedValue.ToString(); });
            this.cb_Scan2Type.SelectionChanged += new SelectionChangedEventHandler((a, b) => { sys_info.sScanner2Type = cb_Scan2Type.SelectedValue.ToString(); });
            this.cb_OvenType.SelectionChanged += new SelectionChangedEventHandler((a, b) => { sys_info.sOvenType = cb_OvenType.SelectedValue.ToString(); });
            this.cb_CurrExtType.SelectionChanged += new SelectionChangedEventHandler((a, b) => { sys_info.sCurrentExtType = cb_CurrExtType.SelectedValue.ToString(); });

            this.tb_MasterSerial.SetBinding(TextBox.TextProperty, new Binding("sMasterSerial") { Source = sys_info, Mode= BindingMode.TwoWay });
            this.tb_Scan1Serial.SetBinding(TextBox.TextProperty, new Binding("sScanner1Serial") { Source = sys_info, Mode = BindingMode.TwoWay });
            this.tb_Scan2Serial.SetBinding(TextBox.TextProperty, new Binding("sScanner2Serial") { Source = sys_info, Mode = BindingMode.TwoWay });
            this.tb_OvenSerial.SetBinding(TextBox.TextProperty, new Binding("sOvenSerial") { Source = sys_info, Mode = BindingMode.TwoWay });
            this.tb_CurrExtSerial.SetBinding(TextBox.TextProperty, new Binding("sCurrentExtSerial") { Source = sys_info, Mode = BindingMode.TwoWay });
            /*
            this.tb_MasterSerial.Text = sys_info.sMasterSerial;
            this.cb_Scan1Serial.Text = sys_info.sScanner1Serial;
            this.cb_Scan2Serial.Text = sys_info.sScanner2Serial;
            this.cb_OvenSerial.Text = sys_info.sOvenSerial;
            this.cb_CurrExtSerial.Text = sys_info.sCurrentExtSerial;
             */
        }
    }
}
