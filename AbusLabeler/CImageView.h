#pragma once
#include <opencv2/opencv.hpp>
#include <vector>
using std::vector;


// CImageView
class CImageView : public CStatic
{
	DECLARE_DYNAMIC(CImageView)

public:
	CImageView();
	virtual ~CImageView();
	void CreatMemDC();
protected:
	DECLARE_MESSAGE_MAP()
public:
	void SetViewID(int view_id);
	void DrawCusor(CDC* pDC, int x, int y);
	void SavePhoto(CString name);
	afx_msg void OnPaint();
	void MatToCImage(cv::Mat& image, cv::Mat& label, double alpha);
	void ImageFusion();
	void BorderFusion();
	void ShowCVImage(cv::Mat& vMat, cv::Mat& label);
	void ScreenToLabel();
	CPoint PointConvertToImage(CPoint point);
	void SetDisplayParm(int orgin_image_height, int orgin_image_width);
	void ClearScreen(CDC * pDC);
	void UpdateScreen(CDC * pDC);
	CDC m_memDC;
	CBitmap m_bmp;
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	COLORREF GetPesudoColor(int index);
	uchar PesudoColorToGray(COLORREF& color);
	afx_msg void OnDestroy();
	bool m_use_mem_dc;
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	CString m_view_name;
	int m_view_id;
	CPoint m_old_point;
	CPoint m_old_drag_point;
public:
	bool m_b_draw_line;
	bool m_b_draw_mode;
	bool m_b_show_border;
	int m_line_type;
	int m_offset_height;
	int m_offset_width;
	double m_scale_factor;
	double m_min_scale;
	double m_min_scale_resolving;
	double m_change_scale_rate;
	vector<CPoint> m_points;
	CImage m_image;				// 单通道的灰度图像
	CImage m_label_image;		// 单通道的标记图像
	CImage m_fusion_image;		// 3通道的融合图像
	double m_alpha;				// 图像融合比例
	bool m_b_transform;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

};


