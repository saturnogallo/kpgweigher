#include "stdafx.h"
#include "msword8.h"
#include "Replace.h"

/*
	Replace the Word Document using bookmark as the 
	placeholder
	tfname : the template file
	smap  : source map of string translate (replace from keyvalue to objectvalue)
*/
BOOL SJReplaceWordByBookmark(LPCTSTR tfname,const CMapStringToString& smap)
{
	// TODO: Add your control notification handler code here
	CString m_SourcePath;
	m_SourcePath.Format("%s",tfname);


	COleVariant m_True((short)TRUE),m_False((short)FALSE), m_Long((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	COleVariant m_GotoBookmark((short)-1);
	long m_LoopVar;
	CString bm,text;	
	
	//check whether the file exists
	CFile tf;
	if(!tf.Open(tfname,CFile::modeReadWrite))
		return FALSE;
	tf.Close();
	
	_Application appObj;
	Documents docsObj;
	_Document docObj;
	try{
		
		if(!appObj.CreateDispatch("Word.Application"))
		{
			AfxMessageBox("Coud Not Create The Application Object");
		}
		appObj.SetVisible(FALSE);
		
		docsObj = appObj.GetDocuments();
		
		docObj.AttachDispatch(docsObj.Open(COleVariant(m_SourcePath),m_False,m_False,m_False,m_Long,m_Long ,m_False,m_Long,m_Long,m_Long  ));
		
		Selection selObj(appObj.GetSelection());
		
		Bookmarks m_Books(docObj.GetBookmarks());	
		
		Bookmark m_Book;
		
		for(m_LoopVar=1;m_LoopVar<= m_Books.GetCount();m_LoopVar++)
		{
			docObj.Activate();
			
			m_Book.AttachDispatch(m_Books.Item(COleVariant(m_LoopVar)));
			
			bm = m_Book.GetName();
			bm.Remove('_');
			bm.MakeUpper();
			if(!smap.Lookup(bm,text)) //can't be found in smap
			{
				bm.MakeUpper();
				
				if(!smap.Lookup(bm,text))
				{
					bm.MakeLower();
					if(!smap.Lookup(bm,text))
						text.Format("/");
				}
			}
			if(text == _T("空"))
				text = _T("") ;
			
			if(((bm.GetAt(0) == 'z') || (bm.GetAt(0) == 'Z')) && (bm.GetAt(1) >= '0') && (bm.GetAt(1) <= '9'))
			{
				if(!((bm.GetLength() >= 4) && (bm.GetAt(1) >='4')))
				{
					if(text.FindOneOf("0123456789") < 0)
						text.Format("/");
				}
			}

			bm = m_Book.GetName();
			m_Book.DetachDispatch();		
			
			selObj.GoTo(COleVariant(long(-1)),m_Long,m_Long,COleVariant(bm));
			selObj.TypeText(text);
		}
		selObj.ReleaseDispatch();
		
		docObj.Close(m_Long,m_Long,m_Long);
		docObj.ReleaseDispatch();
		
		//docsObj.Close(m_Long,m_Long,m_Long);
		docsObj.ReleaseDispatch();

		appObj.Quit(m_Long,m_Long,m_Long);
		appObj.ReleaseDispatch();
		return true;
		//		MessageBox("Completed Successfully","Easy Split",MB_ICONINFORMATION);	
	}
	catch(COleException *e) //有异常？我就逮、逮、逮！
	{
		LPVOID lpMsg;
		::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, NULL, e->m_sc,
			MAKELANGID(LANG_NEUTRAL, 
			SUBLANG_DEFAULT),(LPTSTR) &lpMsg,
			0, NULL);
		::MessageBox(NULL, (LPCTSTR)lpMsg, "COM Error(COM错误)", MB_OK | MB_SETFOREGROUND);
		::LocalFree( lpMsg );
	}
	catch(COleDispatchException *e) //有异常？我就捉、捉、捉！
	{
		char msg[512];
		sprintf(msg, "Run-time error '%d':\n\n%s", 
			e->m_scError & 0x0000FFFF, e->m_strDescription);
		::MessageBox(NULL, msg, "Server Error(服务器错误)", MB_OK | MB_SETFOREGROUND);
	}
	return false;
}
/*
	Replace the Txt(or html,dat,etc) Document using Mark as the 
	placeholder 
	tfname : the template file
	dfname : the destination file 
	smark  : the mark style given , exampl , "@@*.*@@"
			 remember do not put the * as the first or the last char
			 remember do not put the ** in the smark
	func   : the callback function when the case is found, 
			 '*' in smark will be replaced by the text found
			 then the text will be passed back as parameter
*/
BOOL SJReplaceTxtByMark(LPCTSTR tfname,LPCTSTR dfname,CString smark,LPCTSTR (*func)(LPCTSTR))
{
	CStdioFile	tfile,dfile;//template file and data file
	CString		tline,dline;
	
	
	//check the illegal format
	if((smark.Find("**",0) >= 0)||
		(smark.GetAt(smark.GetLength()-1) == '*')||
		(smark[0]== '*')||
		smark.IsEmpty())
		return false;

	if(!dfile.Open(dfname,CFile::modeCreate|CFile::modeWrite))
		return false;

	if(!tfile.Open(tfname,CFile::modeRead)) 
	{
		dfile.Close();
		return false;
	}
	
	while(tfile.ReadString(tline))
	{
		int		markfill=0;
		bool	passall=false;
		int		nNow = tline.Find(smark[0],0)-1;
		int		nStart;
		int		nShift=0;
		
		if ( nNow < -1)
		{	//not found , continue //save a lot of time
			dfile.WriteString(tline+"\n");
			continue;
		}
		dline.Empty();
		nNow = -1;
		while(nNow++ < (tline.GetLength()-1))
		{
			dline += tline[nNow];
			if(tline[nNow] == smark[markfill])
			{
				if(markfill != (smark.GetLength()-1)) //not the last char
				{
					if(markfill == 0) //find the first char
						nStart = nNow;
					if(passall = (smark[++markfill] == '*'))
						++markfill;
					continue;
				}
				//find the last char
				
				dline.Delete(nStart+nShift,nNow+1-nStart);
				dline += (*func)(tline.Mid(nStart,nNow+1-nStart));
				nShift = dline.GetLength()-nNow-1;
				passall = false;
			}
			if(!passall)
				markfill = 0;
		}
		dfile.WriteString(dline+"\n");
	}

	tfile.Close();
	dfile.Close();
	return true;
}

/*
	Extract the Txt(or html,dat,etc) Document using Mark as the 
	placeholder 
	tfname : the template file
	dfname : the data file 
	smark  : the mark style given , exampl , "@@*.*@@"
			 remember do not put the * as the first or the last char
			 remember do not put the ** in the smark
	func   : the callback function when the case is found, 
			 '*' in smark will be replaced by the text found
			 then the mark text and data text will be passed back as parameter
			 func ( mark,data )
	NOTE   : char after smark should not appear in data text , or data will be mismatched
*/
BOOL SJExtractTxtByMark(LPCTSTR tfname,LPCTSTR dfname,CString smark,void (*func)(LPCTSTR,LPCTSTR))
{
	CStdioFile	tfile,dfile;//template file and data file
	CString		tline,dline;
	
	//check the illegal format
	if((smark.Find("**",0) >= 0)||
		(smark.GetAt(smark.GetLength()-1) == '*')||
		(smark[0]== '*')||
		smark.IsEmpty())
		return false;

	if(!dfile.Open(dfname,CFile::modeRead))
		return false;

	if(!tfile.Open(tfname,CFile::modeRead)) 
	{
		dfile.Close();
		return false;
	}
	
	while(dfile.ReadString(dline)&&tfile.ReadString(tline))
	{
		dline.Remove(' ');
		tline.Remove(' ');

		int		markfill=0;
		bool	passall=false;
		int		nNow = tline.Find(smark[0],0)-1;
		int		tdshift = 0;		
		if ( nNow < -1)
		{	//not found , continue //save a lot of time
			continue;
		}
		while(nNow++ < (tline.GetLength()-1))
		{
			int	nStart;
			
			if(tline[nNow] == smark[markfill])
			{
				if(markfill != (smark.GetLength()-1)) //not the last char
				{
					if(markfill == 0) //find the first char
						nStart = nNow;
					if(passall = (smark[++markfill] == '*'))
						++markfill;
					continue;
				}
				//find the last char
				passall = false;

				if (nNow >= tline.GetLength())	//come to the end of the line
					(*func)(dline.Right(dline.GetLength()-(nNow+tdshift)),tline.Mid(nStart,nNow-nStart));
				else
				{
					int i = tdshift;
					while((dline[nStart+ i++] != tline[nNow+1])&&
							(i < (dline.GetLength()-nStart)))
							;
					i--;
					(*func)(tline.Mid(nStart,nNow+1-nStart),dline.Mid(nStart+tdshift,i-tdshift));
					tdshift = i+nStart-(nNow+1);
				}
			}
			if(!passall)
				markfill = 0;
			if(markfill == 0)
			nNow = tline.Find(smark[0],nNow+1)-1;
			if(nNow < -1) break;
		}
	}
	tfile.Close();
	dfile.Close();
	return true;
}
/*
 *	Find the first match string in smatch list which is sperated by '\n'
 *	the result is returned by replace the smatch;
 *	return value indicate whether match is found.
 */
BOOL SJFindFirstMatchByTxt(CString fname,CString& smatch)
{
	CStdioFile tfile;
	CString tline;
	CString mfirst;
	CStringArray samatch;
	int pos=0;

	while(0 <= (pos=smatch.Find('\n')))
	{
		samatch.Add(smatch.Left(pos));
		mfirst += smatch[0];
		smatch = smatch.Right(smatch.GetLength()-pos-1);
	}
	if(!smatch.IsEmpty())
	{
		samatch.Add(smatch);
		mfirst += smatch[0];
	}
	
	if(!tfile.Open(fname,CFile::modeRead))
		return false;
	while(tfile.ReadString(tline))
	{
//		if(tline.FindOneOf(mfirst) < 0) //most case
//			continue;
		pos=samatch.GetSize();
		while(pos-- > 0)
		{
			if(tline.Find(samatch[pos]) < 0)
				continue;
			smatch = samatch[pos];
			return (pos >= 0);
		}
	};
	return FALSE;
}

