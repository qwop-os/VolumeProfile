// DataTips.cpp: 实现文件
//

#include "pch.h"
#include "MFCApplication37.h"
#include "DataTips.h"


// DataTips

IMPLEMENT_DYNAMIC(DataTips, CWnd)

DataTips::DataTips()
{

}

DataTips::~DataTips()
{
}


BEGIN_MESSAGE_MAP(DataTips, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()



// DataTips 消息处理程序

void DataTips::SetPos(int mx, int my)
{
	m_Startp.x = mx;
	m_Startp.y = my;
	Relayout();
}


void DataTips::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	dc.FillRect(rc, &CBrush(m_color[m_nIndex]));
	CPen Pen(PS_SOLID, 1, RGB(119, 233, 122));
	dc.SelectObject(Pen);
	dc.SelectObject(CreateSolidBrush(RGB(119, 233, 122)));
	CRect RcCircle;
	RcCircle.top = rc.Height() / 2 - 8;
	RcCircle.left = rc.left + 8;
	RcCircle.right = RcCircle.left + 14;
	RcCircle.bottom = RcCircle.top + 14;
	dc.Ellipse(RcCircle);
	RcCircle.left	=RcCircle.right + 3;
	RcCircle.right = rc.right;
	dc.SetTextColor(RGB(255, 255, 255));
	dc.SetBkMode(TRANSPARENT);
	CString str;
	str.Format("%s:%.4f亿", m_arrName[m_nIndex], m_values[m_nIndex]);
	dc.DrawText(str, RcCircle, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
}

void DataTips::SetValues(std::vector<double>&v)
{
	m_values = v;
}

void DataTips::SetNames(CArray<CString, CString>&m_arr)
{
	m_arrName.SetSize(m_arr.GetSize());
	m_arrName.Copy(m_arr);
}

void DataTips::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	Relayout();
}
void DataTips::Relayout()
{
	MoveWindow(m_Startp.x,m_Startp.y,m_cx, m_cy);
}