#pragma once


// DataTips
#include <vector>
class DataTips : public CWnd
{
	DECLARE_DYNAMIC(DataTips)

public:
	DataTips();
	virtual ~DataTips();
	void SetPos(int mx, int my);
	void Relayout();
	int getcx() const { return m_cx; }
	int getcy() const { return m_cy; }
	void SetValues(std::vector<double>&v);
	void SetNames(CArray<CString, CString>&m_arr);
	void SetIndex(int nIndex) { m_nIndex = nIndex; }
private:
	std::vector<double> m_values;
	CArray<CString, CString>m_arrName;
	int m_nIndex{-1};
	std::vector<COLORREF> m_color{ RGB(174, 0, 0),RGB(248, 52, 52),RGB(255, 128, 128) ,RGB(10, 130, 10),RGB(39,183,41),RGB(119,233,122) };
	CPoint m_Startp{ 0,0 };
	int m_cx{ 160 };
	int m_cy{30};

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};


