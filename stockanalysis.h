// stockanalysis.h : stockanalysis DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CstockanalysisApp
// �йش���ʵ�ֵ���Ϣ������� stockanalysis.cpp
//

class CstockanalysisApp : public CWinApp
{
public:
	CstockanalysisApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
