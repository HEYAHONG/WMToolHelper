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



#include "WMToolHelperApp.h"
#include "wx/process.h"


#include "GUIDialog.h"

class WMToolHelperDialog: public GUIDialog
{
    public:
        WMToolHelperDialog(wxDialog *dlg);
        ~WMToolHelperDialog();
    protected:
        virtual void OnButtonRefreshCom( wxCommandEvent& event );
		virtual void OnButtonStart( wxCommandEvent& event );
		virtual void OnButtonStop( wxCommandEvent& event );
    private:
        virtual void OnClose(wxCloseEvent& event);
        virtual void OnQuit(wxCommandEvent& event);
        virtual void OnAbout(wxCommandEvent& event);
        virtual void OnRefreshTimer( wxTimerEvent& event );

        wxProcess *flashprocess;
        long      flashprocess_pid;
};
#endif // WMTOOLHELPERMAIN_H
