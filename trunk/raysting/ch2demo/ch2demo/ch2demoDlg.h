// ch2demoDlg.h : header file
//

#pragma once
#define MAX_CH_NUM 2

#define PRBTYPE_PT100	0xf1
#define PRBTYPE_PT25	0xf2
#define PRBTYPE_PT1000  0xf3
#define PRBTYPE_MAX     PRBTYPE_PT1000
#define PRBTYPE_MIN     PRBTYPE_PT100

#define PRBTYPE_K       0x03
#define PRBTYPE_N	0x04
#define PRBTYPE_E	0x05
#define PRBTYPE_B	0x06
#define PRBTYPE_J	0x07
#define PRBTYPE_S	0x08
#define PRBTYPE_T	0x09
#define PRBTYPE_R	0x0A
#define PRBTYPE_BIT	0x10
#define PRBTYPE_INVALID	0x80

#define LED_NUM	8
typedef struct _DISPLAY
{
	unsigned char LED_CODE[LED_NUM]; //after translate
}DISPLAY:

typedef struct _PRBDATA
{
	double param1[MAX_CH_NUM];
	double param2[MAX_CH_NUM];
	double param3[MAX_CH_NUM];                      
	double rtp[MAX_CH_NUM];
}PRBDATA;


typedef struct _SYSDATA{
	double          R0;  //zero offset
	double          V0;  //zero offset
	double          Rs1; //jiao-zheng zhi for PT100
	int             ktime;//time for switch
	uchar           prbmode;
	uchar           kttmode;      
	double          Rs2; //for PT1000              
}SYSDATA;


typedef struct _RUNDATA
{
	double  reading[MAX_CH_NUM];		  //reading on each channel
	double  temperature[MAX_CH_NUM];	  //temperature result on each channel

	double 	Rx;		  //final result
	double  stdV;		  //voltage on stdV;
}RUNDATA;

// Cch2demoDlg dialog
class Cch2demoDlg : public CDialog
{
// Construction
public:
	Cch2demoDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_CH2DEMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck8();
};
