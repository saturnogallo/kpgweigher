#include "stdafx.h"


//void SearchDir(LPCTSTR rpath, LPCTSTR filetype, void (*ffind)(LPCTSTR));
CString SjGetAppDirectory()
{
	CString sAppFolder;
	TCHAR szPath[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(),szPath,sizeof(szPath));
	sAppFolder=szPath;
	int id=sAppFolder.ReverseFind('\\');
	ASSERT(id!=-1);
	return sAppFolder.Left(id);
}

static CString g_Searched;
//the result will be stored in a long string splitted by fname
static void searchfind(LPCTSTR fname)
{
	g_Searched += CString(fname)+_T("\n");
}
/*
CString SSearchDir(LPCTSTR rpath, LPCTSTR filetype)
{
	g_Searched.Empty();
	SearchDir(rpath,filetype,searchfind);
	return g_Searched;
}*/
/*
void SearchDir(LPCTSTR rpath, LPCTSTR filetype, void (*ffind)(LPCTSTR))
{
	HANDLE hFinder; 
	LPWIN32_FIND_DATA lpFindFileData; 

	char DirName[MAX_PATH];
	
	lpFindFileData  = new WIN32_FIND_DATA; 
	
	if (strstr(rpath,"\\") == rpath)
	{
		strcpy(DirName,SjGetAppDirectory());
		strcat(DirName,rpath);
	}
	else
		strcpy(DirName,rpath);

	if(::SetCurrentDirectory(DirName) == FALSE)  // if can't find the dir 
	{
			  TRACE0("Failed to Locate the dir"); 
			  delete lpFindFileData;
			  return;
	} 
	CFile cf;
	hFinder = ::FindFirstFile(filetype,  // pointer to name of file to search for 
							  lpFindFileData );  // pointer to returned information 
	if(hFinder != INVALID_HANDLE_VALUE)
	{
		if( strcmp(lpFindFileData->cFileName,".") && strcmp(lpFindFileData->cFileName,"..") ) 
		{ 
			CString csTempDir; 
			csTempDir = lpFindFileData->cFileName; 
			if((lpFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) 
			{ 
				ffind(csTempDir.Left(csTempDir.GetLength()-4));
			} 
		}
	}

	while(::FindNextFile(hFinder,lpFindFileData))  // process the catalog dir; 
	{ 
		if( !wcscmp(lpFindFileData->cFileName,".") || !strcmp(lpFindFileData->cFileName,"..") ) 
			continue; 
		CString csTempDir; 
		csTempDir = lpFindFileData->cFileName; 
		if((lpFindFileData->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) 
		{ 
			ffind(csTempDir.Left(csTempDir.GetLength()-4));
		} 
	}	
	delete lpFindFileData;
}
*/