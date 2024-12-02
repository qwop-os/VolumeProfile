
// MFCApplication37Dlg.h: 头文件
//

#pragma once
#include<vector>
#include "DataTips.h"
// CMFCApplication37Dlg 对话框
class CMFCApplication37Dlg : public CDialogEx
{
// 构造
public:
	CMFCApplication37Dlg(CWnd* pParent = nullptr);	// 标准构造函数
	virtual ~CMFCApplication37Dlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION37_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	int IsPointInSector(int mx, int my, int cx, int cy, double r);
	double DegreesToRadians(double degrees);
	void InitOtherRadius(int nIndex);
private:
	std::vector<COLORREF>m_color{ RGB(174, 0, 0),RGB(248, 52, 52),RGB(255, 128, 128) ,RGB(10, 130, 10),RGB(39,183,41),RGB(119,233,122)};
	std::vector<COLORREF>m_colorExpand{ RGB(191, 0, 0),RGB(255, 57, 57),RGB(255, 140, 140) ,RGB(11, 143, 11),RGB(42,201,45),RGB(130,255,134) };
	std::vector<int>m_angles{60,30,90,60,30,90};//不同区域所占的角度
	std::vector<int>m_Radius{100,100,100,100,100,100};//r
	double m_Radiu{ 100 };
	int m_linelen{ 150 };
	CPoint m_centerp;
	int m_nIndex{ -1 };
	bool m_bTrack{ false };
	CArray<CString, CString> m_arrStr;
	CRect m_rcTitle;
	CRect m_rcClose;
	DataTips* m_ptip;
	CString m_STime;
	CRect m_rcTime;
public:
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
