// RemoteClientDoc.cpp : CRemoteClientDoc 类的实现
//

#include "stdafx.h"
#include "RemoteClient.h"

#include "RemoteClientDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRemoteClientDoc

IMPLEMENT_DYNCREATE(CRemoteClientDoc, CDocument)

BEGIN_MESSAGE_MAP(CRemoteClientDoc, CDocument)
END_MESSAGE_MAP()


// CRemoteClientDoc 构造/析构

CRemoteClientDoc::CRemoteClientDoc()
{
	// TODO: 在此添加一次性构造代码

}

CRemoteClientDoc::~CRemoteClientDoc()
{
}

BOOL CRemoteClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CRemoteClientDoc 序列化

void CRemoteClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CRemoteClientDoc 诊断

#ifdef _DEBUG
void CRemoteClientDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRemoteClientDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CRemoteClientDoc 命令
