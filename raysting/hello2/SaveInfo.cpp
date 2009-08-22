#include "stdafx.h"
//////////////////////////////////////////////////////////////////////////////////////
/////     说 明
////
// 1. 如果下列函数执行时返回错误，用GetLastError()得到错误信息
// 2. 读和写都按标准的Ini文件格式
// 3. 在CATCH{}宏里可以得到异常信息

#define  MAX_LEN   1000      
///用于CString，使用CString类前最好估算整个操作过程中可能出现的最大                             
///字符长度，用GetBuffer(MAX_LEN)来创建缓冲区。这将避免频繁创建、删除
///缓冲区，提高了效率，也避免了内存碎片。
CString  strIniPath = _T("\\内部存储\\profile.ini");  ///全局变量，预先设置一个文件路径。
                                                 ///文件路径没有盘符，如C:\ 、D:\ 。
                                                 ///必须为绝对路径，CE下没相对路径概念。

BOOL  MyWriteProfileString(const CString strSection, const CString strEntry, const CString strValue)
{
	////先判断所有参数是否为空
	if(strSection == L"" || strEntry == L"" || strValue == L"")
	{
		return FALSE;
	}
	CFile    IniFile;
	CString  strCombine;

	TRY
	{
		////打开文件
		if(! IniFile.Open(strIniPath, CFile::modeReadWrite|CFile::modeCreate|CFile::modeNoTruncate))
		{
			return FALSE;
		}
		///判断文件是否为空，为空则直接写数据，就不必先把文件内容读出来。
		if(IniFile.GetLength() == 0)
		{
			strCombine = L"[" + strSection + L"]" + L"\r\n" 
				         + strEntry + L"=" + strValue + L"\r\n";
			///得到strCombine包含的缓冲区首地址（方法有两种）						
			LPTSTR  lpCombine = strCombine.GetBuffer(0);  
			IniFile.Write(lpCombine, strCombine.GetLength() * 2);
			IniFile.Close();
			return TRUE;
		}
		///读出文件所有数据到缓冲区
		WCHAR  *pBuf;
		pBuf = new WCHAR[IniFile.GetLength() / 2 + 1];  ///文件内容为UNICODE，所以文件长度一定是2的倍数
		if(pBuf == NULL)
		{
			IniFile.Close();
			return FALSE;
		}
		if(IniFile.Read(pBuf, IniFile.GetLength()) != IniFile.GetLength())
		{
			delete[]  pBuf;
			IniFile.Close();
			return FALSE;
		}
		///把缓冲区地址赋给CString对象，为了使用CString包含的函数。
		///一般Ini文件比较小，所以从缓冲区到CString的复制过程用时很短、耗费资源也很少
		pBuf[IniFile.GetLength() / 2] = NULL;
		strCombine.GetBuffer(MAX_LEN);   ///先创建大的缓冲区
		strCombine = pBuf;
		delete[]   pBuf;

		//////开始查找，看section和entry是否已经存在
		int iIndex1, iIndex2, iIndex3, iIndexT;    
		iIndex1 = strCombine.Find(L"[" + strSection + L"]\r\n");
		if(iIndex1 == -1)  ///没找到
		{
			strCombine += L"[" + strSection + L"]" + L"\r\n" 
				          + strEntry + L"=" + strValue + L"\r\n";
			///得到strCombine包含的缓冲区首地址
			LPTSTR  lpCombine = strCombine.GetBuffer(0);
			IniFile.SetLength(0);   ///删除原来数据
			IniFile.SeekToBegin();
			IniFile.Write(lpCombine, strCombine.GetLength() * 2);
			IniFile.Close();
			return TRUE;
		}
		iIndexT = iIndex1 + 4 + strSection.GetLength();
		iIndex2 = strCombine.Find(strEntry + L"=", iIndexT);
		if(iIndex2 == -1)  ///没找到
		{
			strCombine.Insert(iIndexT, strEntry + L"=" + strValue + L"\r\n");
			///得到strCombine包含的缓冲区首地址
			LPTSTR  lpCombine = strCombine.GetBuffer(0);
			IniFile.SetLength(0);
			IniFile.SeekToBegin();
			IniFile.Write(lpCombine, strCombine.GetLength() * 2);
			IniFile.Close();
			return TRUE;
		}
		else
		{
			iIndex3 = strCombine.Find(L"\r\n", iIndex2 + 1);
			if(iIndex3 == -1)
			{
				IniFile.Close();
				return FALSE;
			}
			iIndexT = iIndex2 + 1 + strEntry.GetLength();
			strCombine.Delete(iIndexT, iIndex3 - iIndexT);
			strCombine.Insert(iIndexT, strValue);
			///得到strCombine包含的缓冲区首地址
			LPTSTR  lpCombine = strCombine.GetBuffer(0);
			IniFile.SetLength(0);  
			IniFile.SeekToBegin();
			IniFile.Write(lpCombine, strCombine.GetLength() * 2);
			IniFile.Close();
			return TRUE;
		}
		
	} ///end TRY
	CATCH(CFileException, e)
	{
/*		////用于调试，得到出错信息
		CString strT;
		switch(e->m_cause)
		{
		case CFileException::generic:
		case CFileException::badPath:
		case CFileException::accessDenied:
		case CFileException::hardIO:
		case CFileException::diskFull:
		...............
		default:
		}
*/	}
	END_CATCH     ///结束调试宏
	
	IniFile.Close();  
	return FALSE;
}


CString GetProfileString(const CString strSection, const CString strEntry, const CString strDefault)
{
	////先判断前两个参数是否为空
	if(strSection == L"" || strEntry == L"")
	{
		return strDefault;   ///不成功则返回默认值
	}
	CFile    IniFile;
	CString  strCombine;

	TRY
	{
		////打开文件
		if(! IniFile.Open(strIniPath, CFile::modeRead))
		{
			return strDefault;
		}
		///判断文件是否为空
		if(IniFile.GetLength() == 0)
		{
			IniFile.Close();
			return strDefault;
		}
		///读出文件所有数据到缓冲区
		WCHAR  *pBuf;
		pBuf = new WCHAR[IniFile.GetLength() / 2 + 1];  
		if(pBuf == NULL)
		{
			IniFile.Close();
			return strDefault;
		}
		if(IniFile.Read(pBuf, IniFile.GetLength()) != IniFile.GetLength())
		{
			delete[]  pBuf;
			IniFile.Close();
			return strDefault;
		}
		pBuf[IniFile.GetLength() / 2] = NULL;
		strCombine.GetBuffer(MAX_LEN);   ///先创建大的缓冲区
		strCombine = pBuf;
		delete[]   pBuf;

		//////开始查找，看section和entry是否已经存在
		int iIndex1, iIndex2, iIndex3, iIndexT;    
		iIndex1 = strCombine.Find(L"[" + strSection + L"]\r\n");
		if(iIndex1 == -1)  ///没找到
		{
			IniFile.Close();
			return strDefault;
		}
		iIndexT = iIndex1 + 4 + strSection.GetLength();
		iIndex2 = strCombine.Find(strEntry + L"=", iIndexT);
		if(iIndex2 == -1)  ///没找到
		{
			IniFile.Close();
			return strDefault;
		}
		else
		{
			iIndex3 = strCombine.Find(L"\r\n", iIndex2 + 1);
			if(iIndex3 == -1)
			{
				IniFile.Close();
				return strDefault;
			}
			iIndexT = iIndex2 + 1 + strEntry.GetLength();
			IniFile.Close();
			return  strCombine.Mid(iIndexT, iIndex3 - iIndexT);
		}
	}
	CATCH(CFileException, e)
	{		
	}
	END_CATCH      ///结束调试宏
	
	IniFile.Close();  
	return strDefault;
}


BOOL  WriteProfileInt(const CString strSection, const CString strEntry, const int iValue)
{
	////代码同WriteProfileString相似。
	////只需将int iValue转成CString strValue
	////使用函数：wchar_t * _itow( int value, wchar_t *string, int radix );
	return TRUE;
} 


int  GetProfileInt(const CString strSection, const CString strEntry, const int iDefault)
{
	////代码同GetProfileString相似。
	////只需将得到的CString类型转换成int型
	////使用函数：int _wtoi( const wchar_t *string );
	return iDefault;
}  

