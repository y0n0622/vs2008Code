// RemoteClientDoc.cpp : CRemoteClientDoc ���ʵ��
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


// CRemoteClientDoc ����/����

CRemoteClientDoc::CRemoteClientDoc()
{
	// TODO: �ڴ����һ���Թ������

}

CRemoteClientDoc::~CRemoteClientDoc()
{
}

BOOL CRemoteClientDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)

	return TRUE;
}




// CRemoteClientDoc ���л�

void CRemoteClientDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// CRemoteClientDoc ���

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


// CRemoteClientDoc ����
