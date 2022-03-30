/***************************************************************
 * Name:      WMToolHelperMain.cpp
 * Purpose:   Code for Application Frame
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

#include "WMToolHelperMain.h"
#include <wx/msgdlg.h>
#include <wx/log.h>
#include <wx/arrstr.h>
#include <wx/utils.h>

WMToolHelperDialog::WMToolHelperDialog(wxDialog *dlg)
    : GUIDialog(dlg)
{

    {
        //设置环境变量
        wxString PATH;
        if(wxGetEnv(_T("PATH"),&PATH))
        {
#ifdef WIN32
            PATH+=_T(";.");
#else
            PATH+=_T(":.");
#endif // WIN32
            wxSetEnv(_T("PATH"),PATH);

        }
    }


    wxLog::EnableLogging(true);
    {
        //设置日志窗口
        wxLogTextCtrl *logger=new wxLogTextCtrl(m_textCtrl_log);
        wxLog::SetActiveTarget(logger);
    }
    {
        //检测wm_tool是否存在
        if(wxExecute(_T("wm_tool -v"),wxEXEC_SYNC|wxEXEC_HIDE_CONSOLE)!=0)
        {
            wxMessageBox(_T("未找到wm_tool,请检查安装!"),_T("警告"));
            Close();
        }
    }
    flashprocess=NULL;
    wxLogMessage(_T("程序已启动!"));
}

WMToolHelperDialog::~WMToolHelperDialog()
{
    wxLog::SetActiveTarget(NULL);
}

void WMToolHelperDialog::OnButtonRefreshCom( wxCommandEvent& event )
{
    //选择串口
    wxArrayString choices;
    wxString      selectedcom=m_comboBox_com->GetStringSelection();
#ifdef WIN32
    {
        //检测串口
        for(size_t i=0;i<255;i++)
        {
            int fd=wxOpen(wxString::Format("\\\\.\\COM%d",(int)i),O_RDWR,0755);
            if(fd>=0)
            {
                choices.Add(wxString::Format("COM%d",(int)i));
                wxClose(fd);
            }
        }
    }
#else
    {
        //检测串口(仅USB转串口)
        for(size_t i=0;i<255;i++)
        {
            int fd=wxOpen(wxString::Format("/dev/ttyUSB%d",(int)i),O_RDWR,0755);
            if(fd>=0)
            {
                choices.Add(wxString::Format("ttyUSB%d",(int)i));
                wxClose(fd);
            }
        }
    }
#endif // WIN32

    m_comboBox_com->Set(choices);

    m_comboBox_com->SetSelection(0);

    {
        for(size_t i=0;i<choices.size();i++)
        {
            if(choices[i]==selectedcom)
            {
                m_comboBox_com->SetSelection(i);
                break;
            }
        }
    }

}
void WMToolHelperDialog::OnButtonStart( wxCommandEvent& event )
{
    if(flashprocess!=NULL)
    {
        return;
    }

    wxString cmd="wm_tool ";

    wxString com=m_comboBox_com->GetStringSelection();
    if(com.empty())
    {
        wxLogMessage(_T("未选择串口!"));
        return;
    }
    else
    {
        wxLogMessage(_T("已选择串口:")+com);
    }
    cmd+=(wxString("-c ")+com+" ");

    wxString downloadspeed=m_choice_downloadspeed->GetString(m_choice_downloadspeed->GetSelection());
    wxLogMessage(_T("已选择下载速度:")+downloadspeed);
    cmd+=(wxString("-ds ")+downloadspeed+" ");

    wxString resettype=m_choice_resettype->GetString(m_choice_resettype->GetSelection());
    wxLogMessage(_T("已选择重启类型:")+resettype);
    cmd+=(wxString("-rs ")+resettype+" ");

    wxString filepath=m_filePicker_firmware->GetPath();
    if(filepath.empty())
    {
        wxLogMessage(_T("未选择固件!"));
        return;
    }
    else
    {
        wxLogMessage(_T("已选择固件:")+filepath);
    }
    cmd+=(wxString("-dl ")+filepath+" ");

    if(m_checkBox_eraseoption->IsChecked())
    {
        wxLogMessage(_T("擦除扇区!"));
        cmd+=(wxString("-eo all "));
    }
    else
    {
        wxLogMessage(_T("不擦除扇区!"));
    }

    {
        wxString debugspeed=m_textCtrl_debugspeed->GetValue();
        long i=0;
        debugspeed.ToCLong(&i);
        if(i >= 1200 && i < 2000000)
        {
            wxLogMessage(_T("启用调试:")+wxString::Format("%d",(int)i));
            cmd+=(wxString("-ws ")+wxString::Format("%d",(int)i)+" ");

            wxString debug_type=m_choice_debugtype->GetString(m_choice_debugtype->GetSelection());
            wxLogMessage(_T("调试格式:")+debug_type);
            cmd+=(wxString("-sl ")+debug_type+" ");
        }
        else
        {
            wxLogMessage(_T("不启用调试!"));
        }
    }

    wxLogMessage(_T("即将执行命令:")+cmd);

    flashprocess=new wxProcess;
    flashprocess->Redirect();
    wxExecute(cmd,wxEXEC_ASYNC|wxEXEC_HIDE_CONSOLE,flashprocess);
    flashprocess_pid=flashprocess->GetPid();
    m_button_start->Enable(false);
    m_button_stop->Enable(true);

}

void WMToolHelperDialog::OnRefreshTimer( wxTimerEvent& event )
{
    if(flashprocess!=NULL)
    {
        if(wxProcess::Exists(flashprocess_pid))
        {
            wxInputStream *input=flashprocess->GetInputStream();
            if(input!=NULL)
            {
                wxString flashprocesslog;
                while(wxProcess::Exists(flashprocess_pid) && input->CanRead())
                {
                    flashprocesslog+=(char)input->GetC();
                }
                m_textCtrl_log->AppendText(flashprocesslog);
            }
        }
        else
        {
            m_button_start->Enable(true);
            m_button_stop->Enable(false);
            auto p=flashprocess;
            flashprocess=NULL;
            //delete p;
        }
    }
}
void WMToolHelperDialog::OnButtonStop( wxCommandEvent& event )
{
    if(flashprocess!=NULL)
    {
        wxProcess::Kill(flashprocess->GetPid(),wxSIGKILL);
    }
}

void WMToolHelperDialog::OnClose(wxCloseEvent &event)
{
    Destroy();
}

void WMToolHelperDialog::OnQuit(wxCommandEvent &event)
{
    Destroy();
}

void WMToolHelperDialog::OnAbout(wxCommandEvent &event)
{

}
