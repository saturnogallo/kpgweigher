#ifndef _SJ_CALIB_H_
#define _SJ_CALIB_H_

/*
structure used to do the calibration caculation
_PREC_TYPE_RGROUP : A group of resistance
_PREC_TYPE_RSINGLE: Single resistance to test
_PREC_TYPE_RIF: similar to RSINGLE, except have condition that some Z..value exist
_PREC_TYPE_RIF: similar to RGROUP, except have condition that some Z..value exist
_PREC_TYPE_FORMAT: just format the data, not pass fail check
_PREC_TYPE_RCAN: 电阻箱残余电阻判断
_PREC_TYPE_BIANCHA: 电阻箱变差判断
*/
#define PREC_TYPE_RGROUP	0	
#define PREC_TYPE_RSINGLE	1
#define PREC_TYPE_RIFZ		2
#define PREC_TYPE_RGIFZ		3
#define PREC_TYPE_FORMAT	4
#define PREC_TYPE_RCAN		5
#define PREC_TYPE_BIANCHA	6
//len 11
#define	JHSEL_0005_10	"0.005,0.01,0.02,0.05,0.1,0.2,0.5,1,2,5,10,"
//len 13
#define	JHSEL_0005_50	"0.005,0.01,0.02,0.05,0.1,0.2,0.5,1,2,5,10,20,50,"
//len 15
#define JHSEL_0001_50	"0.001,0.002,0.005,0.01,0.02,0.05,0.1,0.2,0.5,1,2,5,10,20,50,"
#define	MANTI_11_1		"1,1,1,1,1,1,1,1,1,1,1,"
#define	MANTI_13_1		"1,1,1,1,1,1,1,1,1,1,1,1,1,"
#define	MANTI_15_1		"1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,"
#define MANTI_13_512	"5,1,2,5,1,2,5,1,2,5,1,2,5,"
//len 11
#define PREC_5E5_1E1	"5e-5,1e-4,2e-4,5e-4,1e-3,2e-3,5e-3,1e-2,2e-2,5e-2,1e-1,"
//len 13
#define PREC_5E5_5E1	"5e-5,1e-4,2e-4,5e-4,1e-3,2e-3,5e-3,1e-2,2e-2,5e-2,1e-1,2e-1,5e-1,"
//len 15
#define PREC_1E5_5E1	"1e-5,2e-5,5e-5,1e-4,2e-4,5e-4,1e-3,2e-3,5e-3,1e-2,2e-2,5e-2,1e-1,2e-1,5e-1,"
typedef struct _PRECBASE2{
	unsigned char type; //type of the calibration routine will be used, PREC_TYPE_RGROUP | PREC_TYPE_RSINGLE | PREC_TYPE_RIFZ | PREC_TYPE_RGIFZ
	int istart;     //rules apply when index >= istart index <= iend;
	int iend;   
	int ibasepos;	//used for RGROUP | RGIFZ type as the offset position to minus with
	const char* sScope; //standard value should be iscope * (ipos-ibasepos) or iscope * ibasepos, ex "1000.0", if sScope < 0, means std limit ex: "-0.01"
	const char* sJhsel; //list of jhsel to use, ex: "jhsel0" or just position of sLevel ex:"1", and sMantissa
	const char* sLevel; //list of jhsel valid values: ex: "0.05,0.1"
	const char* sDigipos;  //list of digital position to round off, ex: "-9",is a short form for -3,-3,-3,-2,-2,-2,-1...  or "-3,-3,-3,-2,..."
	const char* sMantissa; //list of mantissa for each sLevel, ex: MANTI_11_1
	const char* sPrecious; //list of Precious to compare, ex: PREC_5E5_1E1, 
	const char* sRef; //other config based on prec_type, ex: RIFZ | RGIFZ might use this as the reference position, ex: "Z123"
	const TCHAR* sErrMsg; //special error message, ex: X{val_scope}ohm盘第{rel_pos}点不合格 val_scope and rel_pos will be replaced by sScope and ipos-ibasepos
}PRECBASE2;
//add , 61234 will be modified as 61,234
void sjInsertColon(CString &val)
{

	CString value(val);
	if((val.FindOneOf(_T("0123456789")) < 0) || (val.Find('.') > 0))
		return;
	
	if(val.Find("<font color=") >= 0)
	{
		value.Delete(0,value.Find('>',0)+1);
		value.Replace(_T("</font>"),_T(""));
	}
	if(value.GetLength() <= 6)
		return;
	CString newval;
	while(value.GetLength() > 3)
	{
		newval =_T(",")+value.Right(3)+newval;
		value.Delete(value.GetLength() - 3,3);
	}
	val = value + newval;
}
bool sjExtractInt(CString &sParam,int &value)
{
	int pos1,pos2;
	if(sParam.IsEmpty())
		return false;

	pos1 = sParam.Find(',',0);
	pos2 = sParam.Find(']',0);
	if(pos1 == -1) pos1 = pos2;
	if(pos1 != -1)
	{
		value = atoi(sParam.Left(pos1));
		sParam = sParam.Right(sParam.GetLength()-pos1-1);
		return true;
	}else{
		value = atoi(sParam);
		sParam.Empty();
	}
	return true;	
};
bool sjExtractDouble(CString &sParam,double &value)
{
	int pos1,pos2;
	if(sParam.IsEmpty())
		return false;
	pos1 = sParam.Find(',',0);
	pos2 = sParam.Find(']',0);
	if(pos1 == -1) pos1 = pos2;
	if(pos1 != -1)
	{
		value = atof(sParam.Left(pos1));
		sParam = sParam.Right(sParam.GetLength()-pos1-1);
		return true;
	}else{
		value = atof(sParam);
		sParam.Empty();
	}
	return true;	
};
bool sjExtractString(CString &sParam,CString &value)
{
	int pos1,pos2;
	if(sParam.IsEmpty())
	{
		value.Empty();
		return false;
	}

	pos1 = sParam.Find(',',0);
	pos2 = sParam.Find(']',0);
	if(pos1 == -1) pos1 = pos2;
	if(pos1 != -1)
	{
		value = sParam.Left(pos1);
		sParam = sParam.Right(sParam.GetLength()-pos1-1);
		return true;
	}else{
		value = sParam;
		sParam.Empty();
	}
	return true;	
};
bool sjExtractChar(CString &sParam,char &value)
{
	int pos1,pos2;
	if(sParam.IsEmpty())
		return false;

	pos1 = sParam.Find(',',0);
	pos2 = sParam.Find(']',0);
	if(pos1 == -1) pos1 = pos2;
	if(pos1 != -1)
	{
		value = sParam.GetAt(0);
		sParam = sParam.Right(sParam.GetLength()-pos1-1);
		return true;
	}else{
		value = sParam.GetAt(0);
		sParam.Empty();
	}
	return true;	
};
//rule to convert double to string with position to digipos, mantissa will be 5,2,1
CString DataIntergrate(double value,int digipos,int mantissa)
{
	char nchar = SJD2SGetchar(value,digipos-1);//last char
	char n1char = SJD2SGetchar(value,digipos);//next last char
	int  carry = SJD2SGetcarryI(n1char-'0',nchar-'0',mantissa);//get carry char

	value += carry*pow(10.0,digipos);//adjust

	return SJD2SByPosition(value,digipos);
}

//just return whether matched item (check range only) is found,  if found return position else  return -1
int MatchItem(PRECBASE2 calib_cfg[], int calib_cfg_size, CMapStringToString &mstr,int zindex)
{
	for(int i=0;i<calib_cfg_size;i++)
	{
		if((calib_cfg[i].istart > zindex) || (calib_cfg[i].iend < zindex))
			continue;
		
		return i;
	}
	return -1;
}
//just return the pattern position in line ex : sjStrInStr("0.5,0.01","0.01") returns 2
int sjStrInStr(CString line,CString pattern)
{
	CString ret;
	int pos = 1;
	while(sjExtractString(line,ret))
	{
		if(ret.Compare(pattern) == 0)
			return pos;
		pos++;
	}
	return -1;
}
//just the string at pos ex: sjStrPosStr("0.5,0.01",1) return 0.5
CString sjStrPosStr(CString line, int pos){
	CString ret;
	while(pos-- > 0){
		sjExtractString(line,ret);
	}
	return ret;
}
double neikou(CMapStringToString &mstr,CString jh)
{
	CString ret;
	if(jh.CompareNoCase("jhsel0") && mstr.Lookup("jhsel10",ret))
		return atof(ret);
	if(jh.CompareNoCase("jhsel1") && mstr.Lookup("jhsel11",ret))
		return atof(ret);
	if(jh.CompareNoCase("jhsel2") && mstr.Lookup("jhsel12",ret))
		return atof(ret);
	if(jh.CompareNoCase("jhsel3") && mstr.Lookup("jhsel13",ret))
		return atof(ret);
	if(jh.CompareNoCase("jhsel4") && mstr.Lookup("jhsel14",ret))
		return atof(ret);
	if(jh.CompareNoCase("jhsel5") && mstr.Lookup("jhsel15",ret))
		return atof(ret);
	if(jh.CompareNoCase("jhsel6") && mstr.Lookup("jhsel16",ret))
		return atof(ret);
	if(jh.CompareNoCase("jhsel7") && mstr.Lookup("jhsel17",ret))
		return atof(ret);
	if(jh.CompareNoCase("jhsel8") && mstr.Lookup("jhsel18",ret))
		return atof(ret);
	if(jh.CompareNoCase("jhsel9") && mstr.Lookup("jhsel19",ret))
		return atof(ret);
	return 1;
}
#define CHECK_REC_PASS		0
#define CHECK_REC_FAIL		1
#define CHECK_REC_NOVAL		-1
#define CHECK_REC_NOMATCH	-2
#define CHECK_REC_NOJHSEL	-3
#define CHECK_REC_NOJHITEM  -4
#define CHECK_REC_ERROR		-5
#define CHECK_REC_NOREF		-6
//FORMAT:
//{PREC_TYPE_RSINGLE,47,47,0,"19","jhsel8",JHSEL_0001_50,"-9",MANTI_15_1,PREC_1E5_5E1,NULL,"19Ω不合格"},// "-9"  is a short form for -3,-3,-3,-2,-2,-2,-1...
//{PREC_TYPE_RCAN,19,19,0,"-0.02,0.001,0.01,0.1,1,10,100,1000,10000","jhsel0,jhsel1,jhsel2,jhsel3,jhsel4,jhsel5,jhsel6,jhsel7","1","-4","1","1","Z31,Z51,Z71,Z91,Z111,Z131,Z151,Z171","残余电阻不合格"}
//{PREC_TYPE_RCAN,17,17,0,"-0.02,0.001,0.01,0.1,1,10,100,1000,10000","jhsel0,jhsel1,jhsel2,jhsel3,jhsel4,jhsel5,jhsel6,jhsel7","1","-4","1","1","Z31,Z51,Z71,Z91,Z111,Z131,Z151,Z171","开关变差不合格"}
//bcheckonly means whether we should modify mstr for report integration
int sjCheckOneRecordFull(PRECBASE2 calib_cfg[],int calib_cfg_size,CMapStringToString &mstr,int indexid,bool colormark,bool bdiff,CString &errmsg,bool bcheckonly,bool bneikou)
{
	try{
		//get value
		CString sid,perrecord,ys_perrecord;
		if(indexid > 400)
			return CHECK_REC_PASS;
		sid.Format("Z%d",indexid);
		if(!mstr.Lookup(sid,perrecord) || perrecord.FindOneOf("0123456789") < 0 || (perrecord.Find("--") >= 0))
			return CHECK_REC_NOVAL;

		double dvalue = atof(perrecord);
		
		//get matched item
		int position = MatchItem(calib_cfg,calib_cfg_size,mstr,indexid);
		if(position < 0)
			return CHECK_REC_NOMATCH;
		PRECBASE2 rec = calib_cfg[position];

		//get stdval, iDigipos, iMantissa;
        double dstdval = 0;		//standard
		double dprec = 0;		//precious
		int iDigipos,iMantissa; //digipos and mantissa
		bool passflag;
		CString ret;
		if(rec.type == PREC_TYPE_BIANCHA && !bcheckonly)
		{
			CString jhs = CString(rec.sJhsel);
			CString zs = CString(rec.sRef);
			CString scopes = CString(rec.sScope);
			CString jh,zv,sv;

			passflag = false;
			sjExtractString(scopes,sv);


			while(1)
			{
				sjExtractString(jhs,jh);
				sjExtractString(zs,zv);
				sjExtractString(scopes,sv);
				if(zv.IsEmpty())
					return CHECK_REC_NOJHITEM; //no data at all
							
				if(mstr.Lookup(zv,ret))
				{
					if(mstr.Lookup(jh,ret)){
						double r = bneikou ? neikou(mstr,jh) : 1;
						
						if((abs(dvalue) * 50.0) < (atof(sv)*atof(ret)*r))
							passflag = true;
					}
					break;
				}
			}
			perrecord = DataIntergrate(dvalue,atoi(rec.sDigipos),1);
			ys_perrecord = DataIntergrate(dvalue,atoi(rec.sDigipos)-1,1);
			if(!passflag)
			{
				//format error message
				if(rec.sErrMsg != NULL){
					errmsg = CString(rec.sErrMsg)+CString("\n");
					CString r;
					r.Format(_T("%d"),indexid-rec.ibasepos); //point
					errmsg.Replace("{rel_pos}",r);
					r = CString(rec.sScope);
					r.Replace("-","");
		            errmsg.Replace("{val_scope}",r); //scope
					if(colormark)
						perrecord = "<font color='red'>"+perrecord+"</font>";
				}
			}
			if(!bcheckonly)
			{
				mstr.SetAt(sid,perrecord);
				mstr.SetAt(sid+CString("YS"),ys_perrecord);
			}
			return passflag? CHECK_REC_PASS:CHECK_REC_FAIL;

		}
		if(rec.type == PREC_TYPE_RCAN && !bcheckonly)
		{
			CString jhs = CString(rec.sJhsel);
			CString zs = CString(rec.sRef);
			CString scopes = CString(rec.sScope);
			CString jh,zv,sv;

			passflag = false;
			sjExtractString(scopes,sv);
			if(dvalue < abs(atof(sv)))
			{
				passflag = true;
			}

			while(1)
			{
				sjExtractString(jhs,jh);
				sjExtractString(zs,zv);
				sjExtractString(scopes,sv);
				if(zv.IsEmpty())
					return CHECK_REC_NOJHITEM; //no data at all
							
				if(mstr.Lookup(zv,ret))
				{
					if(mstr.Lookup(jh,ret)){
						double r = bneikou ? neikou(mstr,jh) : 1;
						if((abs(dvalue-atof(sv)) * 100.0) < (atof(sv)*atof(ret)*r))
							passflag = true;
					}
					break;
				}
			}
			perrecord = DataIntergrate(dvalue,atoi(rec.sDigipos),1);
			ys_perrecord = DataIntergrate(dvalue,atoi(rec.sDigipos)-1,1);
			if(!passflag)
			{
				//format error message
				if(rec.sErrMsg != NULL){
					errmsg = CString(rec.sErrMsg)+CString("\n");
					CString r;
					r.Format(_T("%d"),indexid-rec.ibasepos); //point
					errmsg.Replace("{rel_pos}",r);
					r = CString(rec.sScope);
					r.Replace("-","");
		            errmsg.Replace("{val_scope}",r); //scope
					if(colormark)
						perrecord = "<font color='red'>"+perrecord+"</font>";
				}
			}
			if(!bcheckonly)
			{
				mstr.SetAt(sid,perrecord);
				mstr.SetAt(sid+CString("YS"),ys_perrecord);
			}
			return passflag? CHECK_REC_PASS:CHECK_REC_FAIL;
		}
		if((rec.type == PREC_TYPE_RGROUP) || (rec.type == PREC_TYPE_RSINGLE)  || (rec.type == PREC_TYPE_RIFZ) || (rec.type == PREC_TYPE_RGIFZ) || (rec.type == PREC_TYPE_FORMAT))
		{

			//check reference
			
			if((rec.type == PREC_TYPE_RIFZ) || (rec.type == PREC_TYPE_RGIFZ))
			{
				if(!mstr.Lookup(CString(rec.sRef),ret))
					return CHECK_REC_NOREF;
			}
			
			//get iDigipos, iMantissa, dprec by jhsel
			int strId = 0;
			if(rec.sJhsel == NULL)
				return CHECK_REC_NOJHSEL;
			if(!mstr.Lookup(CString(rec.sJhsel),ret)){
				strId = atoi(rec.sJhsel);
				dprec = atof(sjStrPosStr(CString(rec.sPrecious),strId));
				if(bneikou)
					dprec = dprec * neikou(mstr,CString(rec.sJhsel));
			}else{
				strId = sjStrInStr(CString(rec.sLevel),ret);
				if(strId <= 0)
					return CHECK_REC_NOJHITEM;
				dprec = atof(sjStrPosStr(CString(rec.sPrecious),strId));
			}

			
			if(strId <= 0)
				return CHECK_REC_NOJHITEM;
		
			//get stdval
			ret = CString(rec.sScope);
			if(ret.Find(",")> 0){
				ret = sjStrPosStr(ret,strId);
			}
			if((rec.type == PREC_TYPE_RGROUP) || (rec.type == PREC_TYPE_RGIFZ))
				dstdval = atof(ret)*(indexid-rec.ibasepos);//group case
			else
				dstdval = atof(ret);//single case

			CString sdigi = CString(rec.sDigipos);
			if(sdigi.Find(',') > 0)
				iDigipos = atoi(sjStrPosStr(CString(rec.sDigipos),strId));
			else
				iDigipos = floor((atoi(rec.sDigipos)+strId-1)/3.0);
			
			iMantissa = atoi(sjStrPosStr(CString(rec.sMantissa),strId));
			

			//get integrated record string

			perrecord = bdiff ? DataIntergrate(dvalue-fabs(dstdval),iDigipos,iMantissa) : DataIntergrate(dvalue,iDigipos,iMantissa);
			ys_perrecord = bdiff ? DataIntergrate(dvalue-fabs(dstdval),iDigipos-1,iMantissa) : DataIntergrate(dvalue,iDigipos-1,iMantissa);
			
			if(dstdval > 0){
					passflag = fabs(dvalue-dstdval) < fabs(dstdval*dprec);
			}else{
					passflag = fabs(dvalue) < fabs(dstdval);
			}
			if(rec.type == PREC_TYPE_FORMAT)
			{
				passflag = true;
			}
			if(!passflag)
			{
				//format error message
				if(rec.sErrMsg != NULL){
					errmsg = CString(rec.sErrMsg)+CString("\n");
					CString r;
					r.Format(_T("%d"),indexid-rec.ibasepos); //point
					errmsg.Replace("{rel_pos}",r);
					r = CString(rec.sScope);
					r.Replace("-","");
		            errmsg.Replace("{val_scope}",r); //scope
					if(colormark)
						perrecord = "<font color='red'>"+perrecord+"</font>";
				}
			}
			if(!bcheckonly)
			{
				mstr.SetAt(sid,perrecord);
				mstr.SetAt(sid+CString("YS"),ys_perrecord);
			}

			return passflag? CHECK_REC_PASS:CHECK_REC_FAIL;
		}
	}catch(... )
	{
		return CHECK_REC_ERROR;
	}
	return CHECK_REC_PASS;
}
int sjCheckOneRecord(PRECBASE2 calib_cfg[],int calib_cfg_size,CMapStringToString &mstr,int indexid,bool colormark,bool bdiff,CString &errmsg,bool bcheckonly)
{
	return sjCheckOneRecordFull(calib_cfg,calib_cfg_size,mstr,indexid,colormark,bdiff,errmsg,bcheckonly,false);
}
#endif