// RunitemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "hello2.h"
#include "hello2dlg.h"
#include "RunitemDialog.h"
#include "ProbeDialog.h"
#include "SystemDialog.h"
#include "FilterDialog.h"
#include "Setting.h"
#include "Wendu.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRunitemDialog dialog



extern CString SjGetAppDirectory();
CRunitemDialog::CRunitemDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRunitemDialog::IDD, pParent), cfg_now(CFGTYPE_NONE), runcnt(0), show_type(SHOW_MAINDATA)
{
	//{{AFX_DATA_INIT(CRunitemDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_data.RemoveAll();
	m_output.RemoveAll();
	sidedata = NULL;
}


void CRunitemDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRunitemDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRunitemDialog, CDialog)
	//{{AFX_MSG_MAP(CRunitemDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRunitemDialog message handlers
BOOL CRunitemDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CRunitemDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	// TODO: Add your message handler code here
}

CString CRunitemDialog::GetSingleSetting(CString file,CString id)
{
	CString ret;
	if(file == PRB_DEFINFO)
	{
			CSetting<PROBECFG> prbf;
			prbf.InstallFromFile(PRB_DEFINFO);
			PROBECFG prb=prbf.SelectParm(id);
			ret.Format(_T("prb_3wire=%i|prb_sqrt=%i|prb_extref=%f|prb_curr=%i|prb_convert=%i|prb_ref=%i|"),\
		prb.b3woffset,prb.bSqtcurrent,prb.dExtref,prb.iCurrent,prb.iConvert,prb.iReference);
			return ret;
	}
	if(file == FLT_DEFINFO)
	{
			CSetting<FILTERCFG> fltf;
			fltf.InstallFromFile(FLT_DEFINFO);
			FILTERCFG flt=fltf.SelectParm(id);
			ret.Format(_T("flt_speed=%i|flt_thresh=%f|flt_average=%i|flt_integral=%i|flt_interval=%i|flt_response=%i|"),\
		flt.iSpeed,flt.dThresh,flt.iAverage,flt.iIntegral,flt.iInterval,flt.iResponse);
			return ret;
	}
	if(file == SYS_DEFINFO)
	{
			CSetting<SYSTEMCFG> sysf;
			sysf.InstallFromFile(SYS_DEFINFO);
			SYSTEMCFG sys=sysf.SelectParm(id);
			ret.Format(_T("u_10k=%f|u_1k=%f|u_100=%f|u_10=%f|u_1=%f|\
						  u_a1=%f|u_a10=%f|u_a100=%f|u_a1k=%f|u_a10k=%f|u_b1=%f|u_b10=%f|u_b100=%f|u_b1k=%f|u_b10k=%f|u_tempnow=%f|"),\
						  sys.d_10kohm,sys.d_1kohm,sys.d_100ohm,sys.d_10ohm,sys.d_1ohm,sys.d_alpha1,sys.d_alpha10,sys.d_alpha100,sys.d_alpha1k,sys.d_alpha10k,\
						  sys.d_beta1,sys.d_beta10,sys.d_beta100,sys.d_beta1k,sys.d_beta10k,sys.d_tempnow);
			return ret;
	}
	return _T("");
}
void CRunitemDialog::DoCacu()
{
	//update the cacu value here;
}
void CRunitemDialog::Reset(int times)
{
	_times = times;
	runcnt = times;
	m_data.RemoveAll();
	m_output.RemoveAll();
	m_prg.Abort();
}
void CRunitemDialog::PrepareRun(int times) 
{
	//step 1 load all the variables
	//step 2 load the program
	_times = times;
	CString cfg;
	cfg.Format(_T("PTR=%i|"),times-runcnt);
	m_prg.SetEnv(cfg);
	if(cfg_now == CFGTYPE_BORE)
	{
		m_prg.InstallScheme(_T("bore_test"));
	}
	if(cfg_now == CFGTYPE_THMO)
	{
		m_prg.InstallScheme(_T("thmo_test"));
	}
}
bool CRunitemDialog::IsAllDone()
{
	if(!IsValid())
		return true;
	if((_times > 0) && (runcnt <= 0))
		return true;
	return false;
}
bool CRunitemDialog::IsValid()
{
	return (cfg_now != 0);
}
bool CRunitemDialog::IsCacu()
{
	return (cfg_now == CFGTYPE_CACU);
}


bool CRunitemDialog::PostRun()
{
	//a reading has been available, need upload the diagram and global channel result
	CString result,cfg;
	result = m_prg.GetEnv(_T("OUTPUT"));
	
	double d_ret = wcstod(result,NULL);
	
	
	if(cfg_now == CFGTYPE_BORE)
	{
		m_output.AddTail(d_ret);	
		result.Format(_T("%.4f Ω"),CGraph::GetAverage(&m_output));
		m_smalltxt = result;
		double cc[16];
		CProbeDialog::GetCoef(m_prbid,cc);
		d_ret = RValueToTValue(d_ret,cc);
		
		m_data.AddTail(d_ret);
		result.Format(_T("%.4f ℃"),CGraph::GetAverage(&m_data));
		m_bigtxt = result;
		status.Format(_T("通道%i(第%i个读数):%s"),m_ch,runcnt,result);

	}
	
	if(cfg_now == CFGTYPE_THMO)
	{
		m_output.AddTail(d_ret);	
		result.Format(_T("%.4f mV"),CGraph::GetAverage(&m_output)*1000);
		m_smalltxt = result;
		
		d_ret = MValueToTValue(d_ret,m_prbid.GetAt(0));

		m_data.AddTail(d_ret);
		result.Format(_T("%.4f ℃"),CGraph::GetAverage(&m_data));
		m_bigtxt = result;

		status.Format(_T("通道%i(第%i个读数):%s"),m_ch,runcnt,result);

	}
	if(runcnt >= 0)
		runcnt--;
	return (false);
}



void CRunitemDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
}

