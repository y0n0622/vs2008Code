// RemoteClientDoc.h : CRemoteClientDoc ��Ľӿ�
//


#pragma once


class CRemoteClientDoc : public CDocument
{
protected: // �������л�����
	CRemoteClientDoc();
	DECLARE_DYNCREATE(CRemoteClientDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~CRemoteClientDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


