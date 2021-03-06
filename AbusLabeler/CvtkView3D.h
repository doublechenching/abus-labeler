#pragma once
#include "vtkSmartPointer.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"

// CvtkView3D

class CvtkView3D : public CStatic
{
	DECLARE_DYNAMIC(CvtkView3D)

public:
	CvtkView3D();
	virtual ~CvtkView3D();

protected:
	DECLARE_MESSAGE_MAP()
	virtual void PreSubclassWindow();
public:
	afx_msg void OnPaint();
	void Render();
	vtkSmartPointer<vtkRenderWindowInteractor> GetInteractor() { return m_RenderWindow->GetInteractor(); }
	vtkSmartPointer<vtkRenderer> GetRenderer() { return m_Renderer; }

public:
	vtkSmartPointer<vtkRenderer>     m_Renderer;
	vtkSmartPointer<vtkRenderWindow> m_RenderWindow;
};


