Imports Microsoft.Office.Core
imports Extensibility
Imports System.Runtime.InteropServices
Imports Microsoft.Office.Core.MsoControlType
Imports System.Windows.Forms



#Region " Read me for Add-in installation and setup information. "
' When run, the Add-in wizard prepared the registry for the Add-in.
' At a later time, if the Add-in becomes unavailable for reasons such as:
'   1) You moved this project to a computer other than which is was originally created on.
'   2) You chose 'Yes' when presented with a message asking if you wish to remove the Add-in.
'   3) Registry corruption.
' you will need to re-register the Add-in by building the MyAddin1Setup project 
' by right clicking the project in the Solution Explorer, then choosing install.
#End Region

'<GuidAttribute("1DB934B1-0DAC-469C-B934-28935731314D"), ProgIdAttribute("RT96WordAddin.Connect")> _
<GuidAttribute("C28943AF-36C1-4f8a-84E7-47706A046779"), ProgIdAttribute("RT96WordAddin.Connect")> _
Public Class Connect
    Implements Extensibility.IDTExtensibility2

    Dim b_runing As Boolean
    Dim WithEvents DCStart As CommandBarButton         '菜单条按钮。注意带了WithEvents关键哦，是带有事件的！
    Dim WithEvents DCEnd As CommandBarButton           '菜单条按钮。注意带了WithEvents关键哦，是带有事件的！
    Dim WithEvents DCConfig As CommandBarButton        '菜单条按钮。注意带了WithEvents关键哦，是带有事件的！
    Dim applicationObject As Object
    Dim addInInstance As Object
    Dim cfgdlg As ConfigDlg

    '当开始关闭加载宏时，执行该事件过程
    Public Sub OnBeginShutdown(ByRef custom As System.Array) Implements Extensibility.IDTExtensibility2.OnBeginShutdown
        On Error Resume Next
        If DCStart Is Nothing Then
            Return
        End If
        DCStart.Delete()                                                     '删除加载宏建立的菜单条按钮
        DCStart = Nothing                                                   '清除菜单条按钮内存空间
        DCEnd.Delete()                                                     '删除加载宏建立的菜单条按钮
        DCEnd = Nothing                                                   '清除菜单条按钮内存空间
        DCConfig.Delete()                                                     '删除加载宏建立的菜单条按钮
        DCConfig = Nothing                                                   '清除菜单条按钮内存空间
        cfgdlg = Nothing


    End Sub

    '当有COM加载宏卸载或添加时执行以下代码
    Public Sub OnAddInsUpdate(ByRef custom As System.Array) Implements Extensibility.IDTExtensibility2.OnAddInsUpdate
    End Sub

    '程序启动完毕后执行该过程
    Public Sub OnStartupComplete(ByRef custom As System.Array) Implements Extensibility.IDTExtensibility2.OnStartupComplete
        Dim oCommandBars As CommandBars
        Dim oStandardBar As CommandBar
        Dim oBarPopup As CommandBarPopup
        Dim oControl As CommandBarControl
        Dim i As Integer
        On Error Resume Next
        oCommandBars = applicationObject.CommandBars                                        '获取Word应用程序的CommandBars对象
        oStandardBar = oCommandBars.Item("Standard")                                           '获取“标准”CommandBar对象

        oBarPopup = oStandardBar.Controls.Add(Type:=msoControlPopup, Temporary:=True)  '添加弹出菜单栏
        oBarPopup.Caption = "RT9604AddIn"                                                                                    '设置弹出菜单栏Caption属性
        Me.b_runing = False


        DCStart = oBarPopup.Controls.Add(Type:=msoControlButton)                                       '为菜单栏添加按钮
        With DCStart
            .Caption = "开始采集数据"                                                                               '设置菜单按钮的Caption属性
            .Style = MsoButtonStyle.msoButtonCaption                                                                  '指定按钮的style属性
            .Visible = True            '设置按钮可见
        End With
        DCEnd = oBarPopup.Controls.Add(Type:=msoControlButton)                                       '为菜单栏添加按钮
        With DCEnd
            .Caption = "停止采集数据"                                                                               '设置菜单按钮的Caption属性
            .Style = MsoButtonStyle.msoButtonCaption                                                                  '指定按钮的style属性
            .Visible = True            '设置按钮可见
        End With
        DCConfig = oBarPopup.Controls.Add(Type:=msoControlButton)                                       '为菜单栏添加按钮
        With DCConfig
            .Caption = "采集选项配置.."                                                                               '设置菜单按钮的Caption属性
            .Style = MsoButtonStyle.msoButtonCaption                                                                  '指定按钮的style属性
            .Visible = True            '设置按钮可见
        End With
        Me.UpdateButtonStatus()


        oStandardBar = Nothing
        oCommandBars = Nothing

        BuildCfgDlg()
        Me.cfgdlg.Visible = False
    End Sub
    Private Sub BuildCfgDlg()
        If Me.cfgdlg Is Nothing Then
            Me.cfgdlg = New ConfigDlg
            Me.cfgdlg.m_appObject = applicationObject                                                                          '指定窗体的m_appObject属性值
            Me.cfgdlg.Visible = False
            'Me.cfgdlg.Show()

        End If
    End Sub

    Private Sub UpdateButtonStatus()
        DCStart.Enabled = Not Me.b_runing
        DCEnd.Enabled = Me.b_runing
        DCConfig.Enabled = Not Me.b_runing
    End Sub

    Public Sub OnDisconnection(ByVal RemoveMode As Extensibility.ext_DisconnectMode, ByRef custom As System.Array) Implements Extensibility.IDTExtensibility2.OnDisconnection
        On Error Resume Next
        If RemoveMode <> Extensibility.ext_DisconnectMode.ext_dm_HostShutdown Then
            OnBeginShutdown(custom)
        End If
        applicationObject = Nothing
    End Sub

    Public Sub OnConnection(ByVal application As Object, ByVal connectMode As Extensibility.ext_ConnectMode, ByVal addInInst As Object, ByRef custom As System.Array) Implements Extensibility.IDTExtensibility2.OnConnection

        applicationObject = application
        addInInstance = addInInst


        If (connectMode <> Extensibility.ext_ConnectMode.ext_cm_Startup) Then
            OnStartupComplete(custom)
        End If
    End Sub

    '菜单按钮的单击事件过程
    Private Sub DCStart_Click(ByVal Ctrl As Microsoft.Office.Core.CommandBarButton, ByRef CancelDefault As Boolean) Handles DCStart.Click
        '    Dim fm As New frmHide                                                                                               '实例化frmHide窗体
        '   fm.m_appObject = applicationObject                                                                          '指定窗体的m_appObject属性值
        '  fm.Show()                 
        '显示窗体

        If Me.b_runing = False Then
            Me.cfgdlg.Visible = False
            Me.cfgdlg.StartRun()
            Me.b_runing = True
            Me.UpdateButtonStatus()

        End If
    End Sub
    Private Sub DCEnd_Click(ByVal Ctrl As Microsoft.Office.Core.CommandBarButton, ByRef CancelDefault As Boolean) Handles DCEnd.Click
        If Me.b_runing = True Then
            Me.cfgdlg.Visible = False
            Me.cfgdlg.StopRun()
            Me.b_runing = False
            Me.UpdateButtonStatus()
        End If

    End Sub
    Private Sub DCConfig_Click(ByVal Ctrl As Microsoft.Office.Core.CommandBarButton, ByRef CancelDefault As Boolean) Handles DCConfig.Click
        If Me.b_runing Then _
            Return

        Me.cfgdlg.Visible = True
        Me.cfgdlg.Show()
    End Sub

End Class


