Imports System.Random
Imports System.Windows.Forms
Imports System.Runtime.InteropServices
Imports System.Threading
Imports System.Globalization
Imports System.Text.RegularExpressions
Imports Microsoft.Office.Interop


Public Class ConfigDlg
    Inherits System.Windows.Forms.Form
    Dim b_run As Boolean
    Dim b_formula As Boolean
    Dim i_lastrow As Integer
    Dim i_lastcol As Integer
    Dim s_port As String
    Dim s_found As String
    Dim once As Boolean
    Dim rand As Random


    Dim moRS232 As Rs232


#Region " Windows 窗体设计器生成的代码 "

    Public Sub New()
        MyBase.New()

        '该调用是 Windows 窗体设计器所必需的。
        InitializeComponent()

        '在 InitializeComponent() 调用之后添加任何初始化
        once = False
        rand = New Random(123)
        b_run = False
        s_port = ""
        i_lastrow = -1
        i_lastcol = -1
        moRS232 = New Rs232
        Me.CBPort.SelectedIndex = 0
    End Sub

    '窗体重写 dispose 以清理组件列表。
    Protected Overloads Overrides Sub Dispose(ByVal disposing As Boolean)
        If disposing Then
            If Not (components Is Nothing) Then
                components.Dispose()
            End If
        End If
        MyBase.Dispose(disposing)
    End Sub

    'Windows 窗体设计器所必需的
    Private components As System.ComponentModel.IContainer

    '注意: 以下过程是 Windows 窗体设计器所必需的
    '可以使用 Windows 窗体设计器修改此过程。
    '不要使用代码编辑器修改它。
    Friend WithEvents Label1 As System.Windows.Forms.Label
    Friend WithEvents Timer1 As System.Windows.Forms.Timer
    Friend WithEvents Label3 As System.Windows.Forms.Label
    Friend WithEvents CBPort As System.Windows.Forms.ComboBox
    Friend WithEvents BtnOk As System.Windows.Forms.Button
    Friend WithEvents BtCancel As System.Windows.Forms.Button
    <System.Diagnostics.DebuggerStepThrough()> Private Sub InitializeComponent()
        Me.components = New System.ComponentModel.Container
        Me.Label1 = New System.Windows.Forms.Label
        Me.CBPort = New System.Windows.Forms.ComboBox
        Me.Timer1 = New System.Windows.Forms.Timer(Me.components)
        Me.Label3 = New System.Windows.Forms.Label
        Me.BtnOk = New System.Windows.Forms.Button
        Me.BtCancel = New System.Windows.Forms.Button
        Me.SuspendLayout()
        '
        'Label1
        '
        Me.Label1.Location = New System.Drawing.Point(32, 24)
        Me.Label1.Name = "Label1"
        Me.Label1.Size = New System.Drawing.Size(64, 16)
        Me.Label1.TabIndex = 1
        Me.Label1.Text = "串口号"
        '
        'CBPort
        '
        Me.CBPort.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList
        Me.CBPort.Items.AddRange(New Object() {"COM1", "COM2", "COM3", "COM4", "COM5", "COM6", "COM7", "COM8", "COM9", "COM10"})
        Me.CBPort.Location = New System.Drawing.Point(128, 16)
        Me.CBPort.Name = "CBPort"
        Me.CBPort.Size = New System.Drawing.Size(112, 20)
        Me.CBPort.TabIndex = 2
        '
        'Timer1
        '
        Me.Timer1.Enabled = True
        Me.Timer1.Interval = 1000
        '
        'Label3
        '
        Me.Label3.Location = New System.Drawing.Point(32, 56)
        Me.Label3.Name = "Label3"
        Me.Label3.Size = New System.Drawing.Size(208, 24)
        Me.Label3.TabIndex = 5
        Me.Label3.Text = "波特率:9600, 无校验，1位停止位"
        '
        'BtnOk
        '
        Me.BtnOk.Location = New System.Drawing.Point(56, 88)
        Me.BtnOk.Name = "BtnOk"
        Me.BtnOk.Size = New System.Drawing.Size(88, 24)
        Me.BtnOk.TabIndex = 6
        Me.BtnOk.Text = "确定"
        '
        'BtCancel
        '
        Me.BtCancel.Location = New System.Drawing.Point(192, 88)
        Me.BtCancel.Name = "BtCancel"
        Me.BtCancel.Size = New System.Drawing.Size(88, 24)
        Me.BtCancel.TabIndex = 6
        Me.BtCancel.Text = "取消"
        '
        'ConfigDlg
        '
        Me.AutoScaleBaseSize = New System.Drawing.Size(6, 14)
        Me.ClientSize = New System.Drawing.Size(304, 125)
        Me.ControlBox = False
        Me.Controls.Add(Me.BtnOk)
        Me.Controls.Add(Me.Label3)
        Me.Controls.Add(Me.CBPort)
        Me.Controls.Add(Me.Label1)
        Me.Controls.Add(Me.BtCancel)
        Me.MaximizeBox = False
        Me.MinimizeBox = False
        Me.Name = "ConfigDlg"
        Me.ShowInTaskbar = False
        Me.Text = "Raysting Data Collector"
        Me.ResumeLayout(False)

    End Sub

#End Region

    Public WithEvents m_appObject As Excel.Application                          '带应用程序事件的Excel应用程序对象
    Private Sub Timer1_Tick(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles Timer1.Tick
        If b_run = False Then _
            Return

        Try
            Dim sheet As Microsoft.Office.Interop.Excel.Worksheet

            Dim str As String

            Dim cell As Microsoft.Office.Interop.Excel.Range

            cell = m_appObject.ActiveCell

            If i_lastrow >= 0 And i_lastcol >= 0 And i_lastrow = cell.Row And i_lastcol = cell.Column Then
                If cell.AllowEdit Then
                    If Me.moRS232.IsOpen Then
                        Try
                            moRS232.Write(":fetch?" & Chr(10))

                            Dim datastr As String
                            datastr = ""
                            While (1 = moRS232.Read(1))
                                datastr = datastr + moRS232.InputStreamString
                                If datastr.IndexOfAny(Chr(13) & Chr(10)) > 0 Then
                                    Dim rv As Double
                                    rv = Double.Parse(datastr.Trim())
                                    If Me.b_formula Then
                                        If cell.Formula.ToString.IndexOf(Me.s_found) > 0 Then
                                            cell.Formula = cell.Formula.ToString.Replace("VALUE(""" + Me.s_found, "VALUE(""" + rv.ToString)
                                            i_lastcol = -1
                                        End If
                                    Else
                                        cell.Value = Math.Round(rv * 10000000) / 10000000.0
                                    End If
                                    moRS232.ClearInputBuffer()
                                    Me.Text = cell.Value.ToString
                                    Exit While
                                End If
                            End While
                        Catch ex As Exception
                            'cell.Value = ex.Message.ToString
                            Return
                        End Try
                    End If
                End If
            Else
                StartRun()
            End If

        Catch ex As Exception
            Return
            'MessageBox.Show("run:" + ex.Message())
        End Try

    End Sub
    Public Sub StartRun()
        b_run = False
        Try
            If moRS232.IsOpen Then
                moRS232.Close()
            End If
            With moRS232
                .Port = Me.CBPort.SelectedIndex + 1                                                    '// Uses COM1 
                .BaudRate = 9600                                        ' // 2400 baud rate 
                .DataBit = 8                                                 '// 8 data bits                                                   
                .StopBit = Rs232.DataStopBit.StopBit_1         '// 1 Stop bit 
                .Parity = Rs232.DataParity.Parity_None           '// No Parity 
                .Timeout = 500                                             '// 500 ms of timeout admitted to get all required bytes 
            End With
            '// Initializes and Open 
            moRS232.Open()
            If moRS232.IsOpen Then
                'moRS232.Write("*RST\n*CLS\n:INIT:CONT ON;:ABORT\n:SENS:FUNC 'VOLT:DC'\n:SENSE:CHAN 1\n:sens:volt:rang:auto on\n:sens:volt:chan1:lpas off\n:SENS:VOLT:DC:NPLC 1\nVOLT:DIG 8\n:syst:azer on\n")
            Else
                MessageBox.Show("串口无法打开")
            End If
        Catch ex As Exception
            MessageBox.Show("启动失败:" + ex.Message)
            Return
        End Try


        Dim sheet As Excel.Worksheet
        Try
            Dim cell As Excel.Range
            Dim ftext As String
            Dim d As Double
            cell = m_appObject.ActiveCell

            Me.b_run = True
            Me.b_formula = False
            i_lastrow = -1
            i_lastcol = -1
            If 1 = cell.Count Then
                If cell.AllowEdit Then
                    If ((cell.Text = "") Or (Double.TryParse(cell.Text, NumberStyles.Any, Nothing, d) And (Not cell.Formula.ToString.StartsWith("=")))) Then
                        i_lastrow = cell.Row
                        i_lastcol = cell.Column
                        Me.b_formula = False
                        Me.Timer1_Tick(Me, System.EventArgs.Empty)
                    Else
                        Dim reg As New System.Text.RegularExpressions.Regex("VALUE\(""(?<data>[-0-9.Ee\+]+)""\)")
                        Dim m As System.Text.RegularExpressions.Match

                        m = reg.Match(cell.Formula.ToString)

                        If m.Success And cell.Formula.ToString.StartsWith("=") Then
                            Me.s_found = m.Groups("data").Value.ToString
                            i_lastrow = cell.Row
                            i_lastcol = cell.Column
                            Me.b_formula = True
                            Me.Timer1_Tick(Me, System.EventArgs.Empty)
                        End If
                    End If
                End If
            End If
        Catch ex As Exception
            Return
            'MessageBox.Show("启动失败:" + ex.Message())
        End Try


    End Sub
    Public Sub StopRun()
        Me.b_run = False
        moRS232.Close()
    End Sub

    Private Sub BtCancel_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtCancel.Click
        If Me.s_port <> "" Then _
            Me.CBPort.SelectedText = Me.s_port
        Me.Visible = False
    End Sub

    Private Sub BtnOk_Click(ByVal sender As System.Object, ByVal e As System.EventArgs) Handles BtnOk.Click
        Me.s_port = Me.CBPort.SelectedText
        Me.Visible = False
    End Sub
End Class
