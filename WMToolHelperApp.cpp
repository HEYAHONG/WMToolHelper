/***************************************************************
 * Name:      WMToolHelperApp.cpp
 * Purpose:   Code for Application Class
 * Author:    HYH (cdwzdl.cn)
 * Created:   2022-03-27
 * Copyright: HYH (cdwzdl.cn)
 * License:
 **************************************************************/

#ifdef WX_PRECOMP
#include "wx_pch.h"
#endif

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#include "WMToolHelperApp.h"
#include "WMToolHelperMain.h"

IMPLEMENT_APP(WMToolHelperApp);

bool WMToolHelperApp::OnInit()
{

    WMToolHelperDialog* dlg = new WMToolHelperDialog(0L);
    dlg->Show();
    return true;
}
