#pragma once

class CSomeThing
{
public:
	CSomeThing(void);
	~CSomeThing(void);
	BOOL FreeBMP(LPCTSTR pszResName,LPCTSTR pszResType,LPCTSTR szFileName); 
	CString MyGetOS(int id);

	CString MyCharToWide(char * ch);
	char* MyWideToChar(CString st);
};
