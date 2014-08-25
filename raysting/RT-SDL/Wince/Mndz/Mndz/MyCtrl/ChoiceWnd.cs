using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace Mndz
{
    /*
     * The window is designed for make choice
     */
    public partial class ChoiceWnd : Form
    {
        public KbdDataHandler choice_handler;
        //at least one choice should be made
        public bool bNo0Choice {get;set;}
        //parameter for identify
        public string param { get; set; }
        //choice of items
        public string[] items { get;set; }
        //choice name
        //last choice
        public int index { get; set; }
        //last choice for MutliSelection case
        public bool[] states { get; set; }

        private static int deep = 0; //depth of multiple selection dialog
        //can be multiple select or single selection.
        private bool bMultiSelect { get; set; }
        
        private int totalnum = 0;
        private Color btColor = Color.LightBlue;
        public ChoiceWnd()
        {
            InitializeComponent();
            notelabel.ForeColor = this.ForeColor;
            this.BackColor = Color.White;
            btn_quit.colorShadow = this.BackColor;
            btn_quit.colorTop = Color.Beige;
            btn_quit.Style = MyButtonType.roundButton;
            btn_quit.Text = StringResource.str("quit");
            btn_quit.Label = StringResource.str("quit");
            btn_quit.bOn = true;
            btn_quit.ValidClick += new EventHandler(btn_quit_ValidClick);
            bNo0Choice = true;
            RectButton btn;
            foreach (int x in Enumerable.Range(1, MAX_NUMBER))
            {
                btn = find_btn(x);
                btn.Font = new System.Drawing.Font("Arial Narrow", 24F, System.Drawing.FontStyle.Bold);
                btn.colorShadow = this.BackColor;
                btn.colorTop = btColor;
                btn.Style = MyButtonType.rectButton;

                btn.ValidClick += new EventHandler(btn_ValidClick);
            }
            TopMost = true;
            Hide();
        }

        void btn_ValidClick(object sender, EventArgs e)
        {
            int i = Int32.Parse((sender as RectButton).Name.Remove(0, "btn_choice".Length)) - 1;
            Form1.DoBeep();
            if (bMultiSelect)
            {
                states[i] = !states[i];
                UpdateState(i+1);
            }
            else
            {
                choice_handler(param, i.ToString());
                //deep--;
                if (deep <= 0)
                    Hide();
                else
                    Invalidate();
            }
        }

        private const int MAX_NUMBER = 18;
        void btn_quit_ValidClick(object sender, EventArgs e)
        {
            Form1.DoBeep();
            if (bMultiSelect)
            {
                if (bNo0Choice && (states.Count( x => x) <= 0))
                {
                    Program.MsgShow(StringResource.str("atleast1choice"));
                    return;
                }
                choice_handler(param, "done");

            }
            else
            {
                choice_handler(param, "quit");
            }
            //deep--;
            if (deep <= 0)
                Hide();
            else
                Invalidate();
        }
        public void Init(string init_note, string[] init_items,int init_index, bool[] init_states, KbdDataHandler handler)
        {
            if (init_items.Length > MAX_NUMBER)
                throw new Exception("Item number exceeds in choice wnd");
            notelabel.Text = init_note;
            bMultiSelect = (init_states != null) && (init_items.Length == init_states.Length);
            items = init_items;
            if (bMultiSelect)
                states = init_states;
            else
                index = init_index;
            choice_handler = handler;
            if (init_items.Length != totalnum)
            {
                totalnum = init_items.Length;
                ReLayout();
            }
            UpdateState(0);
            //deep++;
            this.Show();
        }

        private void ReLayout()
        {
            int total = items.Length;
            bool tricol = (total > (MAX_NUMBER * 2 / 3));
            bool twocol = (!tricol) && (total > (MAX_NUMBER/3));

            RectButton btn;
            if (tricol)
            {
                int basey = btn_quit.Height + 20;
                int h = (this.Height - basey) / ((total+2) / 3);

                foreach (int i in Enumerable.Range(1, MAX_NUMBER))
                {
                    btn = find_btn(i);
                    if (i <= total)
                        btn.Text = items[i - 1];
                    btn.Width = (this.Width * 2) / 10;

                    if (i % 3 == 1)
                        btn.Left = this.Width / 6 - btn.Width / 2;
                    else if (i % 3 == 2)
                        btn.Left = this.Width  / 2  - btn.Width / 2;
                    else
                        btn.Left = this.Width * 5 / 6 - btn.Width / 2;

                    btn.Top = basey;

                    if (h > 70)
                        btn.Height = 65;
                    else
                        btn.Height = h - 5;

                    if (i % 3 == 0)
                        basey = basey + h;
                    btn.Visible = (i <= total);
                    btn.colorTop = btColor;
                }
            }
            else if (twocol)
            {
                int basey = btn_quit.Height + 20;
                int h = (this.Height - basey) / ((total+1) / 2);

                foreach (int i in Enumerable.Range(1, MAX_NUMBER))
                {
                    btn = find_btn(i);
                    if (i <= total)
                        btn.Text = items[i - 1];
                    btn.Width = (this.Width * 2) / 5;

                    if (i % 2 == 1)
                        btn.Left = this.Width / 4 - btn.Width / 2;
                    else
                        btn.Left = this.Width * 3 / 4 - btn.Width / 2;
                    btn.Top = basey;

                    if (h > 70)
                        btn.Height = 65;
                    else
                        btn.Height = h - 5;

                    if (i % 2 == 0)
                        basey = basey + h;
                    btn.Visible = (i <= total);
                    btn.colorTop = btColor;
                }
            }
            else
            {
                int basey = btn_quit.Height + 20;
                int h = (this.Height - basey) / total;

                foreach (int i in Enumerable.Range(1, MAX_NUMBER))
                {
                    btn = find_btn(i);
                    if (i <= total)
                        btn.Text = items[i - 1];
                    btn.Width = (this.Width / 2);
                    btn.Left = (this.Width - btn.Width) / 2;
                    btn.Top = basey;

                    if (h > 70)
                        btn.Height = 65;
                    else
                        btn.Height = h - 5;
                    basey = basey + h;
                    btn.Visible = (i <= total);
                    btn.colorTop = btColor;
                }
            }
            this.Invalidate();
        }
        private void UpdateState(int id)
        {
            RectButton btn;
            foreach (int i in Enumerable.Range(1, totalnum))
            {
                if (i != id && id != 0)
                    continue;
                btn = find_btn(i);

                if (btn is RectButton)
                {
                    if (bMultiSelect)
                        btn.bOn = states[i - 1];
                    else
                        btn.bOn = ((i - 1) != index);
                    btn.Label = items[i-1];
                }
            }
        }
        private RectButton find_btn(int id)
        {
            foreach (Control ctrl in this.Controls)
            {
                if(ctrl.Name == String.Format("btn_choice{0}", id))
                    return ctrl as RectButton;
            }
            throw new Exception("Control not found " + id.ToString());
        }
    }
}
