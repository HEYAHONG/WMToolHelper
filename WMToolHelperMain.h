/***************************************************************
 * Name:      WMToolHelperMain.h
 * Purpose:   Defines Application Frame
 * Author:    HYH (cdwzdl.cn)
 * Created:   2022-03-27
 * Copyright: HYH (cdwzdl.cn)
 * License:
 **************************************************************/

#ifndef WMTOOLHELPERMAIN_H
#define WMTOOLHELPERMAIN_H


#include "Version.h"
#include "WMToolHelperApp.h"
#include "wx/process.h"
#include <wx/stdpaths.h>
#include <wx/bitmap.h>
#include <wx/dcmemory.h>
#include <wx/file.h>
#include <string>

#include "GUIDialog.h"
#include "time.h"
#include "qrencode.h"

class FlashProcess;

class WMToolHelperDialog: public GUIDialog
{
public:
    WMToolHelperDialog(wxDialog *dlg);
    ~WMToolHelperDialog();

    bool FindExecutable(wxString name);

    wxBitmap GetQrCode(wxString data,int QrCodeDotHeight=5,int version=0, QRecLevel level=QR_ECLEVEL_H, QRencodeMode hint=QR_MODE_8, int casesensitive=1);



protected:
    virtual void OnButtonRefreshCom( wxCommandEvent& event );
    virtual void OnButtonStart( wxCommandEvent& event );
    virtual void OnButtonStop( wxCommandEvent& event );
    virtual void OnbpButtonQrCodeClick( wxCommandEvent& event );


private:
    virtual void OnClose(wxCloseEvent& event);
    virtual void OnQuit(wxCommandEvent& event);
    virtual void OnAbout(wxCommandEvent& event);
    virtual void OnRefreshTimer( wxTimerEvent& event );

    FlashProcess *flashprocess;
    long      flashprocess_pid;
    time_t    retry_timestamp;//重试时间戳
    wxStandardPaths &paths;//程序路径
    wxFile          MacHistory;//Mac地址历史
    wxString ProcessDir;//本进程数据存储目录

    void SetButtonQrCode(wxString str);
    void AddMacHistory(wxString mac);
    void OnSubProcessStdout(int C);

};
#endif // WMTOOLHELPERMAIN_H
