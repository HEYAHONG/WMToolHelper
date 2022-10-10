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
#include <wx/stdpaths.h>

class FlashProcess:public wxProcess
{
    WMToolHelperDialog &parent;
    int ExitCode;
public:
    FlashProcess(WMToolHelperDialog &_parent):parent(_parent)
    {
        ExitCode=0;
    }
    int GetExitCode()
    {
        return ExitCode;
    }
protected:
    virtual void OnTerminate(int  pid,int status)
    {
        ExitCode=status;
        wxLogMessage(_T("烧录进程%d已停止,退出代码:%d"),pid,status);
    }

};

WMToolHelperDialog::WMToolHelperDialog(wxDialog *dlg)
    : GUIDialog(dlg)
{

    wxLog::EnableLogging(true);
    {
        //设置日志窗口
        wxLogTextCtrl *logger=new wxLogTextCtrl(m_textCtrl_log);
        wxLog::SetActiveTarget(logger);
    }

    {
        //设置环境变量
        wxString PATH;
        if(wxGetEnv(_T("PATH"),&PATH))
        {
            wxStandardPaths &paths=wxStandardPaths::Get();
            wxString apppath=paths.GetExecutablePath();
#ifdef WIN32
            if(apppath.Find('\\',true)!=wxString::npos)
            {
                apppath=apppath.substr(0,apppath.Find('\\',true));
            }
            else
            {
                apppath=".";
            }
#else
            if(apppath.Find('/',true)!=wxString::npos)
            {
                apppath=apppath.substr(0,apppath.Find('/',true));
            }
            else
            {
                apppath=".";
            }
#endif // WIN32
            wxLogMessage(_T("程序目录:%s"),apppath);

#ifdef WIN32
            PATH=apppath+_T(";")+PATH;
#else
            PATH=apppath+_T(":")+PATH;
#endif // WIN32
            wxSetEnv(_T("PATH"),PATH);

        }
    }

    {
        //检测wm_tool是否存在
        if(!FindExecutable(_T("wm_tool")))
        {
            wxMessageBox(_T("未找到wm_tool,请检查安装!"),_T("警告"));
            Close();
        }
    }

    {
        //检测putty是否存在
        if(FindExecutable(_T("putty")))
        {
            wxLogMessage(_T("已找到putty，可使用putty调试。"));
            m_choice_debugtype->SetSelection(3);
        }
        else
        {
            wxLogMessage(_T("未找到putty，请安装putty并添加至PATH目录。"));
        }
    }
    flashprocess=NULL;
    retry_timestamp=0;

    {
        wxString downloadspeed=m_choice_downloadspeed->GetString(m_choice_downloadspeed->GetSelection());
        m_textCtrl_debugspeed->SetValue(downloadspeed);
    }

    wxLogMessage(_T("程序已启动!"));
}

WMToolHelperDialog::~WMToolHelperDialog()
{
    wxLog::SetActiveTarget(NULL);
}

bool WMToolHelperDialog::FindExecutable(wxString name)
{
    if(name.empty())
    {
        return false;
    }
    wxString PATH;
    if(wxGetEnv(_T("PATH"),&PATH))
    {
        wxArrayString Paths;
        while(!PATH.empty())
        {
#ifdef WIN32
            auto pos=PATH.Find(";");
#else
            auto pos=PATH.Find(":");
#endif // WIN32
            if(pos!=wxString::npos)
            {
                Paths.push_back(PATH.substr(0,pos));
                PATH=PATH.substr(pos+1);
            }
            else
            {
                Paths.push_back(PATH);
                PATH="";
            }
        }

        for(wxString Path:Paths)
        {
#ifdef WIN32
            wxString exec_path=Path+"\\"+name;
#else
            wxString exec_path=Path+"/"+name;
#endif // WIN32
            exec_path.Replace("\\","/");
            if(wxFile::Exists(exec_path))
            {
                return true;
            }
#ifdef WIN32
            if(wxFile::Exists(exec_path+".exe"))
            {
                return true;
            }
#endif // WIN32
        }
    }
    return false;
}

void WMToolHelperDialog::OnButtonRefreshCom( wxCommandEvent& event )
{
    //选择串口
    wxArrayString choices;
    wxString      selectedcom=m_comboBox_com->GetStringSelection();
#ifdef WIN32
    {
        //检测串口
        for(size_t i=0; i<255; i++)
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
        for(size_t i=0; i<255; i++)
        {
            int fd=wxOpen(wxString::Format("/dev/ttyUSB%d",(int)i),O_RDWR,0755);
            if(fd>=0)
            {
                choices.Add(wxString::Format("ttyUSB%d",(int)i));
                wxClose(fd);
            }
        }
        //检测串口(仅USB转串口(cdc_acm模式))
        for(size_t i=0; i<255; i++)
        {
            int fd=wxOpen(wxString::Format("/dev/ttyACM%d",(int)i),O_RDWR,0755);
            if(fd>=0)
            {
                choices.Add(wxString::Format("ttyACM%d",(int)i));
                wxClose(fd);
            }
        }
        //检测串口(一般串口)
        for(size_t i=0; i<255; i++)
        {
            int fd=wxOpen(wxString::Format("/dev/ttyS%d",(int)i),O_RDWR,0755);
            if(fd>=0)
            {
                choices.Add(wxString::Format("ttyS%d",(int)i));
                wxClose(fd);
            }
        }

    }
#endif // WIN32

    m_comboBox_com->Set(choices);

    m_comboBox_com->SetSelection(0);

    {
        for(size_t i=0; i<choices.size(); i++)
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
        wxString debug_type=m_choice_debugtype->GetString(m_choice_debugtype->GetSelection());
        long i=0;
        debugspeed.ToCLong(&i);
        if((i >= 1200 && i < 2000000) && (debug_type=="str" || debug_type == "hex"))
        {
            wxLogMessage(_T("启用调试:")+wxString::Format("%d",(int)i));
            cmd+=(wxString("-ws ")+wxString::Format("%d",(int)i)+" ");
            wxLogMessage(_T("调试格式:")+debug_type);
            cmd+=(wxString("-sl ")+debug_type+" ");
        }
        else
        {
            wxLogMessage(_T("不启用WM_Tool调试!"));
        }
    }

    wxLogMessage(_T("即将执行命令:")+cmd);

    flashprocess=new FlashProcess(*this);
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
        wxString debug_type=m_choice_debugtype->GetString(m_choice_debugtype->GetSelection());
        if(wxProcess::Exists(flashprocess_pid))
        {
            wxInputStream *input=flashprocess->GetInputStream();
            if(input!=NULL)
            {
                wxString flashprocesslog;
                while(wxProcess::Exists(flashprocess_pid) && input->CanRead())
                {
                    char i=(char)input->GetC();
                    if(debug_type=="str")
                    {
                        if(i=='\r')
                        {
                            continue;
                        }
                    }
                    flashprocesslog+=i;
                }
                m_textCtrl_log->AppendText(flashprocesslog);
            }
        }
        else
        {
            m_button_start->Enable(true);
            m_button_stop->Enable(false);
            FlashProcess * p=flashprocess;
            flashprocess=NULL;

            {
                wxString debugspeed=m_textCtrl_debugspeed->GetValue();
                long i=0;
                debugspeed.ToCLong(&i);
                if((i < 1200 || i > 2000000) && debug_type=="nodebug")
                {
                    if(m_checkBox_FlashProgressExitAbnormal->IsChecked() && (p->GetExitCode()!=0) )
                    {
                        wxLogMessage(_T("将在5秒后重试"));
                        retry_timestamp=time(NULL);
                    }

                    if(m_checkBox_FlashProgressExitNormal->IsChecked() && (p->GetExitCode()==0) )
                    {
                        wxLogMessage(_T("将在5秒后重试"));
                        retry_timestamp=time(NULL);
                    }
                }

                if( (i >= 1200 || i <= 2000000)&& debug_type=="putty")
                {
                    wxString cmd="putty ";
                    cmd+=wxString::Format(_T(" -serial -sercfg %ld,8,n,1 "),i);
                    wxString com=m_comboBox_com->GetStringSelection();
                    cmd+=com;
                    wxLogMessage(_T("即将执行命令:%s"),cmd);
                    wxExecute(cmd);
                }
            }

            delete p;
        }
    }

    //取消重试
    if((!m_checkBox_FlashProgressExitAbnormal->IsChecked()) && (!m_checkBox_FlashProgressExitNormal->IsChecked()))
    {
        retry_timestamp=0;
    }

    if(retry_timestamp!=0)
    {
        if(time(NULL)> (retry_timestamp+5))
        {
            wxCommandEvent evt;
            OnButtonStart(evt);
            retry_timestamp=0;
        }
    }

}
void WMToolHelperDialog::OnButtonStop( wxCommandEvent& event )
{
    if(flashprocess!=NULL)
    {
        wxProcess::Kill(flashprocess->GetPid(),wxSIGKILL);
    }

    //取消重试
    m_checkBox_FlashProgressExitAbnormal->SetValue(false);
    m_checkBox_FlashProgressExitNormal->SetValue(false);
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
