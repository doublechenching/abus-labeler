// CImageView.cpp: 实现文件
//

#include "stdafx.h"
#include "AbusLabeler.h"
#include "CImageView.h"
#include "AbusLabelerDlg.h"
#include <gdiplus.h>
// CImageView

IMPLEMENT_DYNAMIC(CImageView, CStatic)

CImageView::CImageView()
{
	m_b_draw_line = FALSE;
	m_use_mem_dc = FALSE;
	m_b_draw_mode = FALSE;
	m_b_transform = FALSE;
	m_min_scale = 0.4;
	m_min_scale_resolving = 1000;
	m_change_scale_rate = 2;
	m_scale_factor = 1.0;
}


CImageView::~CImageView()
{

}

void CImageView::CreatMemDC()
{
	CRect rect;
	GetClientRect(&rect);
	CDC * pDC = GetDC();
	// 双缓冲显示缓冲
	m_memDC.DeleteDC();
	m_memDC.CreateCompatibleDC(pDC);	//依附窗口DC创建兼容内存DC
	m_bmp.DeleteObject();
	m_bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());	//创建兼容位图，大小与绘图区一致
	m_memDC.SelectObject(&m_bmp);		//将位图选择进内存DC，显示出来
	ReleaseDC(pDC);
	m_use_mem_dc = TRUE;
}


void CImageView::SetViewID(int view_id)
{
	m_view_id = view_id;
	switch (view_id)
	{
	case 1:
		m_view_name = "横截面";
		break;
	case 2:
		m_view_name = "矢状面";
		break;
	case 3:
		m_view_name = "冠状面";
		break;
	}
}

void CImageView::DrawCusor(CDC* pDC, int x, int y)
{
	CRect rect;
	GetWindowRect(&rect);
	int win_height = rect.Height();
	int win_width = rect.Width();
	int radius = 5;
	int left = 0, right = 0, top = 0, down = 0;
	if (x - radius >= 0)
	{
		left = x - radius;
	}
	else
	{
		left = 0;
	}

	if (x + radius >= win_width)
	{
		right = win_width - 1; 
	}
	else
	{ 
		right = x + radius;
	}

	if (y - radius >= 0)
	{
		top = y - radius;
	}
	else
	{
		top = 0;
	}
	if (y + radius >= win_height)
	{
		down = win_height - 1;
	}

	else
	{
		down = y + radius;
	}
	pDC->MoveTo(left, y);
	pDC->LineTo(right, y);
	pDC->MoveTo(x, top);
	pDC->LineTo(x, down);
	pDC->MoveTo(left, top);
	pDC->LineTo(right, down);
	pDC->MoveTo(right, top);
	pDC->LineTo(left, down);
}

void CImageView::SavePhoto(CString name)
{
	CDC* pDC = this->GetDC();
	CRect rect;  
	GetWindowRect(&rect);
	CImage image;
	image.Create(rect.Width(), rect.Height(), 32);
	::BitBlt(image.GetDC(), 0, 0, rect.Width(), rect.Height(), pDC->m_hDC, 0, 0, SRCCOPY);
	image.Save(name);
	ReleaseDC(pDC);
	image.ReleaseDC();
}

BEGIN_MESSAGE_MAP(CImageView, CStatic)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDBLCLK()
END_MESSAGE_MAP()



// CImageView 消息处理程序
void CImageView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CStatic::OnPaint()
	CDC * pDC = GetDC();
	CRect rect;
	GetClientRect(&rect);

	if (m_use_mem_dc)
	{
		m_memDC.SetTextColor(RGB(255, 255, 255));
		m_memDC.SetBkMode(TRANSPARENT);
		m_memDC.TextOut(0, 0, m_view_name);
		if (pDC)
		{
			pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_memDC, 0, 0, SRCCOPY);
		}
	}

	if (pDC)
	{
		ReleaseDC(pDC);
	}
}


void CImageView::MatToCImage(cv::Mat& image, cv::Mat& label, double alpha)
{
	int width = image.cols;
	int height = image.rows;
	int channels = image.channels();
	m_image.Destroy();						//这一步是防止重复利用造成内存问题
	m_image.Create(width, height, 3*8);

	m_label_image.Destroy();						//这一步是防止重复利用造成内存问题
	m_label_image.Create(width, height, 3*8);

	m_fusion_image.Destroy();						//这一步是防止重复利用造成内存问题
	m_fusion_image.Create(width, height, 3*8);
	uchar* ps;
	uchar* pl;
	uchar* p_image = (uchar*)m_image.GetBits(); //获取CImage的像素存贮区的指针
	uchar* p_label = (uchar*)m_label_image.GetBits(); //获取CImage的像素存贮区的指针
	uchar* p_fusion = (uchar*)m_fusion_image.GetBits();
	int step = m_image.GetPitch();			// 每行的字节数,注意这个返回值有正有负
	COLORREF color;
	int index = 0;
	for (int i = 0; i < height; i++)
	{
		ps = image.ptr<uchar>(i);
		pl = label.ptr<uchar>(i);
		for (int j = 0; j < width; j++)
		{
			color = GetPesudoColor(pl[j]);
			index = i * step + j * 3;
			p_image[index]   = ps[j];
			p_image[index+1] = ps[j];
			p_image[index+2] = ps[j];
			p_label[index]   = GetRValue(color);
			p_label[index+1] = GetGValue(color);
			p_label[index+2] = GetBValue(color);
			if (color <= 0)				// 如果是Label像素是黑色，显示原图像素
			{
				p_fusion[index] = p_image[index];
				p_fusion[index+1] = p_image[index+1];
				p_fusion[index+2] = p_image[index+2];
			}
			else
			{	// 如果是Label像素是其他颜色，显示混合颜色
				p_fusion[index] = (1 - alpha) * p_image[index] + alpha * p_label[index];
				p_fusion[index+1] = (1 - alpha) * p_image[index+1] + alpha * p_label[index+1];
				p_fusion[index+2] = (1 - alpha) * p_image[index+2] + alpha * p_label[index+2];
			}
		}
	}

}


void CImageView::ImageFusion()
{
	uchar* p_image = (uchar*)m_image.GetBits(); //获取CImage的像素存贮区的指针
	uchar* p_label = (uchar*)m_label_image.GetBits(); //获取CImage的像素存贮区的指针
	uchar* p_fusion = (uchar*)m_fusion_image.GetBits();
	int step = m_image.GetPitch();			// 每行的字节数,注意这个返回值有正有负
	int height = m_image.GetHeight();
	int width = m_image.GetWidth();
	double alpha = m_alpha;
	int index = 0;
	COLORREF color;
	Uint8 value;
	Uint8 black = 0;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			index = i * step + j * 3;
			color = RGB(p_label[index], p_label[index + 1], p_label[index + 2]);
			value = PesudoColorToGray(color);
			if (value > black)
			{
				p_fusion[index] = (1 - alpha) * p_image[index] + alpha * p_label[index];
				p_fusion[index + 1] = (1 - alpha) * p_image[index + 1] + alpha * p_label[index + 1];
				p_fusion[index + 2] = (1 - alpha) * p_image[index + 2] + alpha * p_label[index + 2];
			}
			else
			{
				p_fusion[index] = p_image[index];
				p_fusion[index + 1] = p_image[index + 1];
				p_fusion[index + 2] = p_image[index + 2];
			}
		}
	}
}


void CImageView::BorderFusion()
{
	cv::Mat label;
	int height = m_image.GetHeight();
	int width = m_image.GetWidth();
	int index = 0;
	COLORREF color;
	Uint8 value;
	int step = m_image.GetPitch();
	uchar* p_src_label = (uchar*)m_label_image.GetBits();
	label.create(m_label_image.GetHeight(), m_label_image.GetWidth(), CV_8UC1);
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			index = i * step + j * 3;
			color = RGB(p_src_label[index], p_src_label[index + 1], p_src_label[index + 2]);
			value = PesudoColorToGray(color);
			label.at<uchar>(i, j) = value;
		}
	}
	std::vector<std::vector<cv::Point>> contours;
	vector<cv::Vec4i> hierarchy;
	cv::findContours(label, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);
	cv::Mat result(label.size(), CV_8UC3, cv::Scalar(0, 0, 0));
	int idx = 0;
	if (!contours.size())
	{
		return;

	}
	for (; idx >= 0; idx = hierarchy[idx][0])
	{
		if (!contours[idx].size())
		{
			return;

		}
		cv::Point p = contours[idx][0];
		int x = p.x;
		int y = p.y;
		int value = label.at<uchar>(y, x);
		COLORREF color = GetPesudoColor(value);
		cv::Scalar cv_color(GetRValue(color), GetGValue(color), GetBValue(color));
		drawContours(result, contours, idx, cv_color, 1, 8, hierarchy);
	}


	uchar* pucRow;
	uchar* p_image = (uchar*)m_image.GetBits();
	uchar* p_fusion = (uchar*)m_fusion_image.GetBits();
	for (int i = 0; i < height; i++)
	{
		pucRow = (result.ptr<uchar>(i));
		for (int j = 0; j < width; j++)
		{
			color = RGB(pucRow[j * 3 + 0], pucRow[j * 3 + 1], pucRow[j * 3 + 2]);
			value = PesudoColorToGray(color);
			if (value > 0)
			{
				*(p_fusion + i * step + j * 3 + 0) = pucRow[j * 3 + 0];
				*(p_fusion + i * step + j * 3 + 1) = pucRow[j * 3 + 1];
				*(p_fusion + i * step + j * 3 + 2) = pucRow[j * 3 + 2];
			}
			else
			{
				*(p_fusion + i * step + j * 3 + 0) = p_image[i * step + j * 3 + 0];
				*(p_fusion + i * step + j * 3 + 1) = p_image[i * step + j * 3 + 1];
				*(p_fusion + i * step + j * 3 + 2) = p_image[i * step + j * 3 + 2];
			}
			
		}
	}
}

void CImageView::ShowCVImage(cv::Mat& vMat, cv::Mat& label)
{
	if (vMat.empty()) 
	{
		return;
	}
	MatToCImage(vMat, label, m_alpha);
	UpdateScreen(&m_memDC);
	OnPaint();
}



void CImageView::ScreenToLabel()
{
	GetParent()->SendMessage(WM_WRITE_TO_LABEL, (WPARAM)&m_label_image, m_view_id);
}


CPoint CImageView::PointConvertToImage(CPoint point)
{
	CPoint image_point;
	image_point.x = int((point.x - m_offset_width) * 1.0 / m_scale_factor);
	image_point.y = int((point.y - m_offset_height) * 1.0 / m_scale_factor);
	return image_point;
}


void CImageView::SetDisplayParm(int orgin_image_height, int orgin_image_width)
{
	int height = orgin_image_height;
	int width = orgin_image_width;
	CRect rect;
	GetClientRect(&rect);
	// 根据长宽比缩放图像
	double r1 = height * 1.0 / width;
	double r2 = rect.Height() * 1.0 / rect.Width();
	if (r2 > r1)
	{
		// 宽不变
		m_scale_factor = rect.Width() * 1.0 / width;
	}
	else
	{
		// 高不变
		m_scale_factor = rect.Height() * 1.0 / height;
	}
	m_offset_height = (rect.Height() - height * m_scale_factor) / 2;
	m_offset_width = (rect.Width() - width * m_scale_factor) / 2;
}

void CImageView::ClearScreen(CDC * pDC)
{
	CRect rect;
	GetWindowRect(&rect);
	CBrush brush(RGB(0, 0, 0));
	CRect r{ 0, 0, rect.Width(), rect.Height()};
	pDC->FillRect(r, &brush);
}

void CImageView::UpdateScreen(CDC * pDC)
{
	ClearScreen(pDC);
	int height = m_fusion_image.GetHeight();
	int width = m_fusion_image.GetWidth();

	// 使用GDI+绘制图形
	int new_height = height * m_scale_factor;
	int new_width = width * m_scale_factor;

	RECT roi_rect = { m_offset_width, m_offset_height, new_width + m_offset_width, new_height + m_offset_height};
	m_fusion_image.Draw(pDC->m_hDC, roi_rect, Gdiplus::InterpolationModeNearestNeighbor);
}


void CImageView::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO:  添加您的代码以绘制指定项

}


COLORREF CImageView::GetPesudoColor(int index)
{
	COLORREF color;
	switch (index)
	{
	case 1:
		color = RGB(0, 0, 255);
		break;
	case 2:
		color = RGB(0, 255, 0);
		break;
	case 3:
		color = RGB(255, 0, 0);
		break;
	case 4:
		color = RGB(255, 255, 0);
		break;
	case 5:
		color = RGB(255, 0, 255);
		break;
	case 6:
		color = RGB(0, 255, 255);
		break;
	case 7:
		color = RGB(255, 255, 255);
		break;
	case 8:
		color = RGB(0, 125, 0);
		break;
	case 9:
		color = RGB(0, 0, 0);
		break;
	default:
		color = RGB(0, 0, 0);
	}
	return color;
}

uchar CImageView::PesudoColorToGray(COLORREF& color)
{
	uchar index = 0;
	switch (color)
	{
	case RGB(0, 0, 255):
		index = 1;
		break;
	case RGB(0, 255, 0):
		index = 2;
		break;
	case RGB(255, 0, 0):
		index = 3;
		break;
	case RGB(255, 255, 0):
		index = 4;
		break;
	case RGB(255, 0, 255):
		index = 5;
		break;
	case RGB(0, 255, 255):
		index = 6;
		break;
	case RGB(255, 255, 255):
		index = 7;
		break;
	case RGB(0, 125, 0):
		index = 8;
		break;
	case RGB(0, 0, 0):
		index = 0;
		break;
	default:
		index = 0;
	}

	return index;
}

void CImageView::OnDestroy()
{
	CStatic::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
	m_memDC.DeleteDC();		//删除DC
	m_bmp.DeleteObject();
}


void CImageView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	PositionMessage mes;
	mes.x = point.x;
	mes.y = point.y;
	GetParent()->SendMessage(WM_SHOW_POS, (WPARAM)&mes, m_view_id);
	if (m_b_draw_mode && m_b_draw_line && !m_b_transform)
	{
		CPen PenForDrawAxis;
		COLORREF color;
		color = GetPesudoColor(m_line_type);
		PenForDrawAxis.CreatePen(PS_SOLID, 2, color);
		CPoint image_point = PointConvertToImage(point);
		m_points.push_back(image_point);
		HDC imageHDC = m_label_image.GetDC();
		CDC* memoryDC = CDC::FromHandle(imageHDC);
		memoryDC->SelectObject(PenForDrawAxis);
		memoryDC->MoveTo(m_old_point);
		memoryDC->LineTo(image_point);
		m_label_image.ReleaseDC();
		m_old_point = image_point;
		// 显示图像重绘
		ImageFusion();
		UpdateScreen(&m_memDC);
	}

	if (m_b_transform && (MK_CONTROL & nFlags))
	{
		if (m_points.size())
		{
			m_points.clear();
		}
		m_offset_width += point.x - m_old_drag_point.x;
		m_offset_height += point.y - m_old_drag_point.y;
		m_old_drag_point = point;
		UpdateScreen(&m_memDC);
	}
	OnPaint();
	
	CStatic::OnMouseMove(nFlags, point);
}


void CImageView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_b_draw_line = TRUE;

	if ((MK_CONTROL & nFlags))
	{
		// 图像缩放平移变换, 平移距离相对于放缩后的图像
		m_b_transform = TRUE;
		m_old_drag_point = point;
		return;
	}

	if (m_b_draw_mode)
	{
		m_old_point = PointConvertToImage(point);
		m_points.clear();
		m_points.push_back(m_old_point);
	}
	else
	{
		// 切换视图
		CDC* pDC = GetDC();
		CPen PenForDrawAxis;
		CPen*pPen;
		PenForDrawAxis.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
		pDC->SelectObject(PenForDrawAxis);
		DrawCusor(pDC, point.x, point.y);
		ReleaseDC(pDC);
		CRect rect;
		GetWindowRect(&rect);
		int win_height = rect.Height();
		int win_width = rect.Width();
		// 坐标转换
		CPoint image_point = PointConvertToImage(point);
		NormPosition mes;
		if (m_fusion_image.IsNull())
		{
			mes.x = image_point.x * 1.0 / win_width;
			mes.y = image_point.y * 1.0 / win_height;
		}
		else
		{
			mes.x = image_point.x * 1.0 / m_fusion_image.GetWidth();
			mes.y = image_point.y * 1.0 / m_fusion_image.GetHeight();
		}
		// 更新其他视图
		GetParent()->SendMessage(WM_UPDATE_VIEW, (WPARAM)&mes, m_view_id);
	}
	CStatic::OnLButtonDown(nFlags, point);
}


void CImageView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_b_draw_line = FALSE;
	if (m_b_draw_mode && m_points.size() > 0)
	{
		CRgn rgn;
		CPoint * arrpt = new CPoint[m_points.size()];
		for (int i = 0; i < m_points.size(); i++)
		{
			arrpt[i] = m_points[i];
		}
		rgn.CreatePolygonRgn(arrpt, m_points.size(), ALTERNATE);
		COLORREF color;
		color = GetPesudoColor(m_line_type);
		CBrush bsh(color);
		// 更新label界面
		HDC imageHDC = m_label_image.GetDC();
		CDC* memoryDC = CDC::FromHandle(imageHDC);
		memoryDC->FillRgn(&rgn, &bsh);
		m_label_image.ReleaseDC();
		rgn.DeleteObject();
		delete[] arrpt;
		ImageFusion();
		ScreenToLabel();
	}
	OnPaint();
	m_b_transform = FALSE;
	CStatic::OnLButtonUp(nFlags, point);
}


BOOL CImageView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (MK_CONTROL & nFlags)
	{
		if (zDelta > 120)  zDelta = 120;
		if (zDelta < -120) zDelta = -120;
		int tmpOffsetX = 0;
		int tmpOffsetY = 0;
		if (m_scale_factor > m_min_scale || zDelta / 120 > 0)
		{
			double tmpscale = zDelta / (double)m_min_scale_resolving;
			if (m_scale_factor > m_change_scale_rate) tmpscale *= m_change_scale_rate;
			// 以指针为中心改变
			tmpOffsetX = (pt.x - m_offset_width) * 1.0 * tmpscale;
			tmpOffsetX = (pt.y - m_offset_height) * 1.0 * tmpscale;
			m_scale_factor += tmpscale;
			m_offset_width -= tmpOffsetX;
			m_offset_height -= tmpOffsetY;
			UpdateScreen(&m_memDC);
			OnPaint();
		}
	}
	else
	{
		if (zDelta > 0)
		{
			GetParent()->SendMessage(WM_NEXT_VIEW, 1, m_view_id);
		}
		else
		{
			GetParent()->SendMessage(WM_NEXT_VIEW, 2, m_view_id);
		}
	}
	return CStatic::OnMouseWheel(nFlags, zDelta, pt);
}


void CImageView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	GetParent()->SendMessage(WM_SHOW_WHOLE_SCREEN, 0, m_view_id);
	CStatic::OnLButtonDblClk(nFlags, point);
}
