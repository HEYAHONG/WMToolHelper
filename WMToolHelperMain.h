#ifndef HEADER_730D9BA91A1DDE62
#define HEADER_730D9BA91A1DDE62

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
#include "wxrc.h"
#include <mutex>
#include <functional>
#include <queue>
#include <string>
#ifdef STATIC
#ifndef PCRE2_STATIC
#define PCRE2_STATIC
#endif
#endif // STATIC
#include <pcre2posix.h>

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
    virtual void OnButtonHistory( wxCommandEvent& event );


private:
    virtual void OnClose(wxCloseEvent& event);
    virtual void OnQuit(wxCommandEvent& event);
    virtual void OnAbout(wxCommandEvent& event);
    virtual void OnRefreshTimer( wxTimerEvent& event );

    wxString wm_tool_path;
    FlashProcess *flashprocess;
    long      flashprocess_pid;
    time_t    retry_timestamp;//重试时间戳
    wxStandardPaths &paths;//程序路径
    wxFile          MacHistory;//Mac地址历史
    wxString ProcessDir;//本进程数据存储目录

    void SetButtonQrCode(wxString str);
    void AddMacHistory(wxString mac);


    std::string stdoutline;//标准输出(行)
    void OnSubProcessStdout(int C);

    //清理文件
    void OnCleanupFile();


    //UI刷新函数队列(其它线程的函数转发至队列执行)
    std::queue<std::function<void()>> UIEvent;
    std::mutex UIEventLock;
    void ProcessUIEvent()
    {
        std::lock_guard<std::mutex> lock(UIEventLock);
        while(UIEvent.size()>0)
        {
            std::function<void()> cb=UIEvent.front();
            UIEvent.pop();
            if(cb!=NULL)
            {
                cb();
            }
        }
    };
public:
    void AddUIEvent(std::function<void()> cb)
    {
        std::lock_guard<std::mutex> lock(UIEventLock);
        UIEvent.push(cb);
    };

};
#endif // WMTOOLHELPERMAIN_H
#endif // header guard

