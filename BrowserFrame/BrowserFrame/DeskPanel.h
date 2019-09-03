#pragma once
#include "DeskContent.h"
#include "DeskButton.h"
#include <vector>
using namespace std;

typedef struct _DESK
{
	CDeskButton deskButton;
	CDeskContent m_wndPanel;

}DESK, *PDESK;

class CDeskPanel :
	public CPanel
{
public:
	CDeskPanel(void);
	~CDeskPanel(void);

public:
	int AddDesk(CString strTitle);
	DESK* GetDesk(int nIndex);
private:
	void ArrangeDesk();
	

public:
	vector<DESK*> m_vecDesk;
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
};
