
// MFCApplication37Dlg.cpp: 实现文件
//

#include <cmath>
#include "pch.h"
#include "framework.h"
#include "MFCApplication37.h"
#include "MFCApplication37Dlg.h"
#include "afxdialogex.h"
#include "CMenDCGX.h"
#include <GdiPlus.h>
using namespace Gdiplus;
#pragma comment(lib, "gdiplus.lib")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define pi 3.14159265358979323846
static const int ID_TIMEEVENT = 10003;
static const int ID_MOUSEPAUSE = 10004;
struct TipMsg
{
	int TotoalSell;
	int TotalBuy;
	COLORREF color;
};
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication37Dlg 对话框



CMFCApplication37Dlg::CMFCApplication37Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION37_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CMFCApplication37Dlg:: ~CMFCApplication37Dlg()
{
	if (m_ptip != nullptr)
	{
		delete m_ptip;
	}
}

void CMFCApplication37Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication37Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// 转换为弧度
double CMFCApplication37Dlg::DegreesToRadians(double degrees) {
	return degrees * pi / 180.0;
}

void CMFCApplication37Dlg::InitOtherRadius(int nIndex)
{
	int len = m_Radius.size();
	for (int i = 0; i < len; i++)
	{
		m_Radius[i] = 100;
	}
	if(nIndex!=-1)
		m_Radius[nIndex] = static_cast<int>(m_Radius[nIndex]*1.2);
}

// 判断点是否在扇形内
int CMFCApplication37Dlg::IsPointInSector(int mx, int my, int cx, int cy, double r) {

	// 计算鼠标点到圆心的距离
	double dx = mx - cx;
	double dy = my - cy;
	dy = -dy;
	double distance = sqrt(dx * dx + dy * dy);

	// 计算鼠标点与圆心的连线与x轴的夹角（以弧度为单位）
	double angle = atan2(dy, dx);
	// 将角度归一化到 [0, 2*PI) 范围内
	if (angle < 0) {
		angle += 2 * pi;
	}

	// 检查距离是否在半径范围内
	bool isInSector =  distance <= r;

	if (isInSector)
	{
		int len = m_angles.size();
		int isum = 0;
		for (int i = 0; i < len; i++)
		{
			isum += m_angles[i];
			if (angle < DegreesToRadians(isum))
			{
				m_Radius[i] = static_cast<int>(m_Radius[i] * 1.2);
				InitOtherRadius(i);
				return i;
			}
		}
	}

	return -1;
}

// CMFCApplication37Dlg 消息处理程序

BOOL CMFCApplication37Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMFCApplication37Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}
// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMFCApplication37Dlg::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(rc);
	CMemDCGX dcMem(&dc,&rc,1);
	Graphics graphics(dcMem.GetSafeHdc());
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	CBrush brushOld(RGB(255, 255, 255));
	dcMem.FillRect(&rc, &brushOld);

	m_rcTitle = rc;
	m_rcTitle.bottom = m_rcTitle.top + 32;
	rc.top = m_rcTitle.bottom;
	rc.bottom = rc.top + 400;
	CRect sgRcText = m_rcTitle;
	sgRcText.left += 5;
	CFont* pOldFont=nullptr;
	CFont font;
	font.CreateFont(20, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_SWISS,"宋体");
	pOldFont = (CFont*)(dcMem.SelectObject(font));
	dcMem.DrawText("实时成交分布图", sgRcText, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
	//关闭按钮
	m_rcClose = sgRcText;
	m_rcClose.right -= 5;
	m_rcClose.top += 11;
	m_rcClose.bottom -= 11;
	m_rcClose.left = m_rcClose.right - 10;
	dcMem.MoveTo(m_rcClose.left, m_rcClose.top);
	dcMem.LineTo(m_rcClose.right, m_rcClose.bottom);
	dcMem.MoveTo(m_rcClose.right, m_rcClose.top);
	dcMem.LineTo(m_rcClose.left, m_rcClose.bottom);

	CFont font1;
	font1.CREATE_FONT(14, "宋体");
	dcMem->SelectObject(font1);
	sgRcText.right = m_rcClose.left - 3;
	sgRcText.left = sgRcText.right - dcMem.GetTextExtent("更新时间:15:30").cx;
	m_rcTime = sgRcText;
	dcMem.DrawText(m_STime, sgRcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	HPEN hpen = CreatePen(PS_SOLID, 2, RGB(47, 87, 149));
	dcMem.SelectObject(hpen);
	dcMem.MoveTo(rc.left, rc.top);
	dcMem.LineTo(rc.right, rc.top);
	DeleteObject(hpen);

	CFont font2;
	font2.CREATE_FONT(12, "宋体");
	dcMem->SelectObject(font2);
	sgRcText = rc;
	sgRcText.right -= 3;
	sgRcText.left = sgRcText.right - dcMem->GetTextExtent("单位:元").cx;
	sgRcText.top += 10;
	sgRcText.bottom = sgRcText.top + dcMem->GetTextExtent("单位:元").cy;
	dcMem->DrawText("单位:元", sgRcText, DT_LEFT | DT_VCENTER | DT_SINGLELINE);

	CPoint centerp(rc.Width() / 2, rc.Height() / 2);
	SetViewportOrgEx(dcMem, centerp.x, centerp.y, nullptr);//不设置的话是Dialog左上方为原点
	m_centerp = centerp;
	CRect rcPip = { m_centerp.x - 120, m_centerp.y - 120, m_centerp.x + 120, m_centerp.y + 120 };
	m_rcPip = rcPip;
	int len = m_angles.size();
	//画线条和文字
	int  SumAnglesLine = 0;
	for (int i = 0; i < len; i++)
	{
		int icos = static_cast<int>(cos(DegreesToRadians(SumAnglesLine+m_angles[i]/2))*m_linelen);
		int isin = static_cast<int>(-sin(DegreesToRadians(SumAnglesLine+m_angles[i]/2))*m_linelen);
		SumAnglesLine += m_angles[i];
		int x =  icos;
		int y =	 isin;
		CPen pen (PS_SOLID, 1, m_color[i]);
		dcMem.SelectObject(pen);
		dcMem.MoveTo(0,0);
		dcMem.LineTo(x, y);
		int xstart = 0;
		if (icos < 0)
			xstart = x - 50;
		else
			xstart = x + 50;
		dcMem.LineTo(xstart, y);
		if (icos < 0)
			xstart -= 10;
		else
			xstart += 10;
		int height = dcMem.GetTextExtent(m_arrStr[i]).cy;
		int weight = dcMem.GetTextExtent(m_arrStr[i]).cx;
		CRect RcText;
		if (icos < 0)
		{
			RcText.right = xstart;
			RcText.top = y-5;
			RcText.left = RcText.right- weight;
			RcText.bottom = RcText.top + height;
		}
		else
		{
			RcText.left = xstart;
			RcText.top = y-5;
			RcText.right = RcText.left + weight;
			RcText.bottom = RcText.top + height;
		}
		dcMem.SetBkMode(TRANSPARENT);
		dcMem.DrawText(m_arrStr[i], RcText, DT_VCENTER | DT_LEFT | DT_SINGLELINE);
	}


	//画扇形图
	CRect RcPie{ -m_Radius[0],-m_Radius[0] ,m_Radius[0],m_Radius[0] };

	CPoint StartP{m_Radius[0],0};
	int SumAngles{ 0 };
	CPoint EndP{int(m_Radius[0] *cos(m_angles[0]*pi/180.0)),int(-m_Radius[0] *sin(m_angles[0]*pi/180.0))};
	SumAngles += m_angles[0];
	HBRUSH hbrush = NULL;
	if (m_Radius[0] == m_Radiu)
		hbrush = CreateSolidBrush(m_color[0]);
	else
		hbrush = CreateSolidBrush(m_colorExpand[0]);
	dcMem->SelectObject(hbrush);
	CPen pen(PS_SOLID, 1, m_color[0]);
	dcMem->SelectObject(pen);
	dcMem.Pie(RcPie, StartP, EndP);
	DeleteObject(hbrush);
	for (int i = 1; i < len; i++)
	{
		CPen pen(PS_SOLID, 1, m_color[i]);
		dcMem->SelectObject(pen);
		StartP = EndP;
		SumAngles += m_angles[i];
		EndP = { int(m_Radius[i] *cos(SumAngles* pi / 180.0)), int(-m_Radius[i] * sin(SumAngles* pi / 180.0)) };
		if (m_Radius[i] == m_Radiu)
			hbrush = CreateSolidBrush(m_color[i]);
		else
			hbrush = CreateSolidBrush(m_colorExpand[i]);
		dcMem.SelectObject(hbrush);
		RcPie.left = RcPie.top = -m_Radius[i];
		RcPie.bottom =RcPie.right = m_Radius[i];
		dcMem.Pie(RcPie, StartP, EndP);
		DeleteObject(hbrush);
	}

	//画表格
	SetViewportOrgEx(dcMem, 0, 0, nullptr);//设置为原点。
	CRect RcGrid;
	GetClientRect(RcGrid);
	RcGrid.top = rc.bottom - 30;
	hpen = CreatePen(PS_SOLID, 1, RGB(188, 213, 233));
	dcMem.SelectObject(hpen);
	dcMem.MoveTo(RcGrid.left, RcGrid.top);
	dcMem.LineTo(RcGrid.right, RcGrid.top);
	CRect ceil = RcGrid;
	int lineHeight = 26;
	CSize sizeword = dcMem.GetTextExtent("大单");
	ceil.left += 30;
	ceil.right = ceil.left+sizeword.cx;
	ceil.bottom = ceil.top + lineHeight;
	dcMem.DrawText("类型", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	ceil.left = ceil.right + 100;
	ceil.right = ceil.left + sizeword.cx;
	dcMem.DrawText("流入", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	ceil.left = ceil.right + 150;
	ceil.right = ceil.left + sizeword.cx;
	dcMem.DrawText("流出", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	RcGrid.top += lineHeight;
	dcMem.MoveTo(RcGrid.left, RcGrid.top);
	dcMem.LineTo(RcGrid.right, RcGrid.top);
	ceil.top = ceil.bottom;
	ceil.bottom = ceil.top + lineHeight;
	ceil.left = RcGrid.left + 30;
	ceil.right = ceil.left + sizeword.cx;
	dcMem.DrawText("大单", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	ceil.left = ceil.right + 80;
	ceil.right = ceil.left + dcMem.GetTextExtent("3.8733亿").cx;
	dcMem.SetTextColor(RGB(255, 0, 0));
	dcMem.DrawText("3.8733亿", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	ceil.left = ceil.right + 130;
	ceil.right = ceil.left + dcMem.GetTextExtent("3.8733亿").cx;
	dcMem.SetTextColor(RGB(0, 255, 0));
	dcMem.DrawText("4.7462亿", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	RcGrid.top += lineHeight;
	dcMem.MoveTo(RcGrid.left, RcGrid.top);
	dcMem.LineTo(RcGrid.right, RcGrid.top);

	RcGrid.top += lineHeight;
	dcMem.MoveTo(RcGrid.left, RcGrid.top);
	dcMem.LineTo(RcGrid.right, RcGrid.top);
	ceil.top = ceil.bottom;
	ceil.bottom = ceil.top + lineHeight;
	ceil.left = RcGrid.left + 30;
	ceil.right = ceil.left + sizeword.cx;
	dcMem.SetTextColor(RGB(0, 0, 0));
	dcMem.DrawText("中单", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	ceil.left = ceil.right + 80;
	ceil.right = ceil.left + dcMem.GetTextExtent("3.8733亿").cx;
	dcMem.SetTextColor(RGB(255, 0, 0));
	dcMem.DrawText("5.7799亿", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	ceil.left = ceil.right + 130;
	ceil.right = ceil.left + dcMem.GetTextExtent("3.8733亿").cx;
	dcMem.SetTextColor(RGB(0, 255, 0));
	dcMem.DrawText("6.7388亿", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	RcGrid.top += lineHeight;
	dcMem.MoveTo(RcGrid.left, RcGrid.top);
	dcMem.LineTo(RcGrid.right, RcGrid.top);
	ceil.top = ceil.bottom;
	ceil.bottom = ceil.top + lineHeight;
	ceil.left = RcGrid.left + 30;
	ceil.right = ceil.left + sizeword.cx;
	dcMem.SetTextColor(RGB(0, 0, 0));
	dcMem.DrawText("小单", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	ceil.left = ceil.right + 80;
	ceil.right = ceil.left + dcMem.GetTextExtent("3.8733亿").cx;
	dcMem.SetTextColor(RGB(255, 0, 0));
	dcMem.DrawText("7.2307亿", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);

	ceil.left = ceil.right + 130;
	ceil.right = ceil.left + dcMem.GetTextExtent("3.8733亿").cx;
	dcMem.SetTextColor(RGB(0, 255, 0));
	dcMem.DrawText("6.2741亿", ceil, DT_VCENTER | DT_CENTER | DT_SINGLELINE);
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMFCApplication37Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication37Dlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_LastMoveTime = time(NULL);
	CRect rc{ m_centerp.x - static_cast<int>(m_bigRadiu),m_centerp.y - static_cast<int>(m_bigRadiu),m_centerp.x + static_cast<int>(m_bigRadiu),m_centerp.y + static_cast<int>(m_bigRadiu) };
	int nIndex = -1;
	if (m_bMousePause)
	{
		return;
	}
	//进入r100;
	if (m_nIndex == -1)
	{
		nIndex = IsPointInSector(point.x, point.y, m_centerp.x, m_centerp.y, m_Radiu);
		if (nIndex != m_nIndex)
			m_bSign = true;
	}
	//离开120，切换100;
	else
	{
		nIndex = IsPointInSector(point.x, point.y, m_centerp.x, m_centerp.y, m_Radiu);
		if (nIndex != -1 && nIndex != m_nIndex)//切换
		{
			m_bSign = true;
		}
		nIndex = IsPointInSector(point.x, point.y, m_centerp.x, m_centerp.y, m_bigRadiu);
		if (nIndex == -1)//离开
		{
			m_bSign = true;
		}
	}
	if (nIndex != -1 && nIndex == m_nIndex&&m_bSign)
	{
		if (m_ptip&&m_ptip->GetSafeHwnd() && m_ptip->IsWindowVisible())
		{
			bool up = true;
			bool left = true;
			if (point.x > m_centerp.x)
				left = false;
			if (point.y > m_centerp.y)
				up = false;
			ClientToScreen(&point);
			up ? point.y = point.y - m_ptip->getcy() - 5 : point.y += 5;
			left ? point.x = point.x - m_ptip->getcx() - 10 : point.x += 10;
			m_ptip->SetPos(point.x, point.y);
			if (m_nIndex != nIndex && nIndex != -1)
			{
				m_ptip->SetIndex(nIndex);
			}
			m_ptip->Invalidate(TRUE);
		}
		return ;
	}
	m_bRefreshPause = true;
	if (m_nIndex == -1)
	{
		nIndex = IsPointInSector(point.x, point.y, m_centerp.x, m_centerp.y, m_Radiu);
	}
	else
	{
		if (m_bSign)
		{
			nIndex = IsPointInSector(point.x, point.y, m_centerp.x, m_centerp.y, m_bigRadiu);
			if (nIndex != m_nIndex)
			{
				nIndex = IsPointInSector(point.x, point.y, m_centerp.x, m_centerp.y, m_Radiu);
			}
		}
		else
		{
			nIndex = IsPointInSector(point.x, point.y, m_centerp.x, m_centerp.y, m_Radiu);
		}
	}
	if (nIndex == -1)
	{
		if (m_ptip&&m_ptip->GetSafeHwnd() && m_ptip->IsWindowVisible())
		{
			m_ptip->ShowWindow(SW_HIDE);
		}
		if (nIndex != m_nIndex)
		{
			m_nIndex = nIndex;
			InitOtherRadius(m_nIndex);
			InvalidateRect(rc, TRUE);
		}
		m_ptip->Invalidate();
	}
	else
	{
		static int numelse = 1;
		if (m_ptip&&m_ptip->GetSafeHwnd() && !m_ptip->IsWindowVisible())
		{
			m_ptip->ShowWindow(SW_SHOW);
		}
	}
	if (m_ptip&&m_ptip->GetSafeHwnd()&&m_ptip->IsWindowVisible())
	{
		bool up = true;
		bool left = true;
		if (point.x > m_centerp.x)
			left = false;
		if (point.y > m_centerp.y)
			up = false;
		ClientToScreen(&point);
		up ? point.y = point.y - m_ptip->getcy() - 5: point.y += 5;
		left ? point.x =point.x-m_ptip->getcx()- 10 : point.x += 10;
		m_ptip->SetPos(point.x, point.y);
		if (m_nIndex != nIndex && nIndex != -1)
		{
			m_ptip->SetIndex(nIndex);
		}
		m_ptip->Invalidate(TRUE);
	}
	if (-1!=nIndex&&nIndex != m_nIndex)
	{
		m_nIndex = nIndex;
		InvalidateRect(rc,TRUE);
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


LRESULT CMFCApplication37Dlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT nHitTest = CDialog::OnNcHitTest(point);
	ScreenToClient(&point);
	// 如果鼠标在窗口客户区，则返回标题条代号给Windows
	// 使Windows按鼠标在标题条上类进行处理，即可单击移动窗口
	if (!m_rcClose.PtInRect(point) && m_rcTitle.PtInRect(point))
		return (nHitTest == HTCLIENT) ? HTCAPTION : nHitTest;
	return CDialog::OnNcHitTest(point);
}


void CMFCApplication37Dlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_rcClose.PtInRect(point))
	{
		CDialog::OnCancel();
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}


int CMFCApplication37Dlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	if (m_ptip == nullptr)
	{
		m_ptip = new DataTips();
		std::vector<double>v;
		v.push_back(3.8733);
		v.push_back(5.7799);
		v.push_back(7.2307);
		v.push_back(4.7462);
		v.push_back(6.7388);
		v.push_back(6.2741);
		m_ptip->SetValues(v);

		m_arrStr.Add("大单流入");
		m_arrStr.Add("中单流入");
		m_arrStr.Add("小单流入");
		m_arrStr.Add("大单流出");
		m_arrStr.Add("中单流出");
		m_arrStr.Add("小单流出");
		m_ptip->SetNames(m_arrStr);
	}
	if (m_ptip&&m_ptip->GetSafeHwnd() == nullptr)
	{
		CString StrClassName = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW);
		m_ptip->CreateEx(0, StrClassName, "", WS_POPUP, CRect{}, this, 0, 0);
	}
	SetTimer(ID_TIMEEVENT, 1000, NULL);
	SetTimer(ID_MOUSEPAUSE, 10, nullptr);
	m_LastMoveTime = time(NULL);

	return 0;
}


void CMFCApplication37Dlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent == ID_TIMEEVENT)
	{
		SYSTEMTIME st;
		GetLocalTime(&st);
		m_STime.Format("更新时间:%02d:%02d", st.wHour, st.wMinute);
		InvalidateRect(m_rcTime);
	}
	else if (ID_MOUSEPAUSE == nIDEvent)
	{
		time_t currentTime = time(NULL);
		double elapsed = difftime(currentTime, m_LastMoveTime);

		// 检查是否超过阈值（例如，5秒）
		if (elapsed >= 5.0&&m_bRefreshPause)
		{
			m_bSign = false;
			CPoint Currentpt;
			GetCursorPos(&Currentpt);
			ScreenToClient(&Currentpt);
			int nIndex = IsPointInSector(Currentpt.x, Currentpt.y, m_centerp.x, m_centerp.y, m_Radiu);
			if (nIndex == -1)
			{
				if (m_ptip&&m_ptip->GetSafeHwnd() && m_ptip->IsWindowVisible())
				{
					m_ptip->ShowWindow(SW_HIDE);
				}
			}
			else
			{
				if(m_ptip&&m_ptip->GetSafeHwnd() && !m_ptip->IsWindowVisible())
				{
					m_ptip->ShowWindow(SW_SHOW);
				}
			}
			m_bRefreshPause = !m_bRefreshPause;
			m_bMousePause = TRUE;
			InitOtherRadius(-1);
			//m_nIndex = -1;
			InvalidateRect(m_rcPip);
		}
		else
		{
			m_bMousePause = FALSE;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
