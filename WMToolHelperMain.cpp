﻿/***************************************************************
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
#include <wx/clipbrd.h>
#include <wx/dir.h>
#include <wx/utils.h>
#include <wx/zipstrm.h>
#include <wx/wfstream.h>
#include <functional>

class FlashProcess:public wxProcess
{
    WMToolHelperDialog &parent;
    int ExitCode;
    std::function<void(FlashProcess &)> OnExit;
public:
    FlashProcess(WMToolHelperDialog &_parent):parent(_parent)
    {
        ExitCode=0;
    }
    int GetExitCode()
    {
        return ExitCode;
    }
    void SetOnExit( std::function<void(FlashProcess &)> func)
    {
        OnExit=func;
    }
protected:
    virtual void OnTerminate(int  pid,int status)
    {
        ExitCode=status;
        if(OnExit!=NULL)
        {
            OnExit(*this);
        }
        wxLogMessage(_T("烧录进程%d已停止,退出代码:%d"),pid,status);
    }

};

WMToolHelperDialog::WMToolHelperDialog(wxDialog *dlg)
    : GUIDialog(dlg),paths(wxStandardPaths::Get())
{


    SetButtonQrCode(_T("WMToolHelper"));
    {
        //显示说明
        const char *readme="ReadMe.txt";
        if(wxRCGetSize(readme))
        {
            m_textCtrl_log->AppendText(wxString::FromUTF8((char *)wxRCGetHandle(readme),wxRCGetSize(readme)));
        }
    }

    wxLog::EnableLogging(true);
    {
        //设置日志窗口
        wxLogTextCtrl *logger=new wxLogTextCtrl(m_textCtrl_log);
        wxLog *oldlog=wxLog::SetActiveTarget(logger);
        if(oldlog!=NULL)
        {
            delete oldlog;
        }
    }



    {
        //设置环境变量
        wxString PATH;
        if(wxGetEnv(_T("PATH"),&PATH))
        {
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

    //用户数据目录
    {
        wxLogMessage(_T("用户数据文件目录:%s"),paths.GetUserDataDir());
        if(!wxDir::Exists(paths.GetUserDataDir()))
        {
            wxDir::Make(paths.GetUserDataDir());
        }

        ProcessDir=paths.GetUserDataDir()+_T("/")+std::to_string(wxGetProcessId());
        ProcessDir.Replace(_T("\\"),_T("/"));
        if(!wxDir::Exists(ProcessDir))
        {
            wxDir::Make(ProcessDir);
        }

        {
            //初始化数据文件
            wxString MacHistoryPath=ProcessDir+_T("/MacHistoy.csv");
            MacHistory.Open(MacHistoryPath,wxFile::write_append);
            if(MacHistory.IsOpened())
            {
                wxLogMessage(_T("Mac地址历史记录文件:%s"),MacHistoryPath);
            }

        }
    }

    {
        //检测wm_tool是否存在
        wm_tool_path="";
        if(FindExecutable(_T("wm_tool")))
        {
            wm_tool_path=_T("wm_tool");
        }

        //若检测到wm600_tool则使用wm600_tool
        if(FindExecutable(_T("wm600_tool")))
        {
            wm_tool_path=_T("wm600_tool");
        }

        if(wm_tool_path.empty())
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

    wxLogMessage(_T("程序已启动(版本:%s)!"),_T(PROJECT_VERSION_STR));
}

WMToolHelperDialog::~WMToolHelperDialog()
{
    OnCleanupFile();
    {
        wxLog::DontCreateOnDemand();
        wxLog *log=wxLog::SetActiveTarget(new wxLogStderr);
        if(log!=NULL)
        {
            wxLogChain *chain=NULL;
            do
            {
                chain=dynamic_cast<wxLogChain *>(log);
                if(chain!=NULL)
                {
                    log=chain->GetOldLog();
                    delete chain;
                }
            }
            while(chain!=NULL);
            if(log!=NULL)
            {
                delete log;
            }
        }
    }
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

    wxString cmd=wm_tool_path+" ";

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
    //刷新UI
    ProcessUIEvent();

    //处理FlashProcess
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
                    int C=input->GetC();
                    OnSubProcessStdout(C);
                    char i=(char)C;
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

                if( FindExecutable(_T("putty")) && (i >= 1200 || i <= 2000000)&& debug_type=="putty" && (p->GetExitCode()==0))
                {
                    wxString cmd="putty ";
                    cmd+=wxString::Format(_T(" -serial -sercfg %ld,8,n,1 "),i);
                    wxString com=m_comboBox_com->GetStringSelection();
#ifndef WIN32
                    com=wxString(_T("/dev/"))+com;
#endif // WIN32
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

void WMToolHelperDialog::OnbpButtonQrCodeClick( wxCommandEvent& event )
{
    if (wxTheClipboard->Open())
    {
        wxTheClipboard->SetData(new wxTextDataObject(m_bpButtonQrCode->GetLabel()));
        wxTheClipboard->Close();
    }
}

void WMToolHelperDialog::OnButtonHistory( wxCommandEvent& event )
{
    //以默认浏览器打开目录
    wxLaunchDefaultBrowser(ProcessDir);
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

wxBitmap WMToolHelperDialog::GetQrCode(wxString data,int QrCodeDotHeight,int version, QRecLevel level, QRencodeMode hint, int casesensitive)
{
    QRcode *qrcode=QRcode_encodeString(data.ToStdString().c_str(),version,level,hint,casesensitive);
    if(qrcode==NULL)
    {
        return wxBitmap();
    }
    wxMemoryDC dc;
    wxBitmap bitmap(wxSize((qrcode->width+2)*QrCodeDotHeight,(qrcode->width+2)*QrCodeDotHeight));
    dc.SelectObject(bitmap);
    dc.SetBackground(*wxWHITE_BRUSH);
    dc.Clear();
    dc.SetBrush(*wxBLACK_BRUSH);
    dc.SetPen(*wxBLACK_PEN);

    int qrcodex=(bitmap.GetSize().GetWidth()-qrcode->width*QrCodeDotHeight)/2;
    if(qrcodex<0)
    {
        qrcodex=0;
    }
    int qrcodey=0;
    if(qrcodey<0)
    {
        qrcodey=0;
    }

    for(size_t i=0; i<qrcode->width*qrcode->width; i++)
    {
        if(qrcode->data[i]&0x01)
        {
            dc.DrawRectangle(qrcodex+(i%qrcode->width*QrCodeDotHeight),qrcodey+(i/qrcode->width*QrCodeDotHeight),QrCodeDotHeight,QrCodeDotHeight);
        }
    }
    QRcode_free(qrcode);
    dc.SelectObject(wxNullBitmap);
    return bitmap;

}

void WMToolHelperDialog::SetButtonQrCode(wxString str)
{
    m_bpButtonQrCode->SetBitmap(GetQrCode(str));
    m_bpButtonQrCode->SetLabel(str);
}

void WMToolHelperDialog::AddMacHistory(wxString mac)
{
    auto ProcessExit=[=](FlashProcess &process)
    {
        wxVector<wxVariant> data;
        time_t now=time(NULL);
        data.push_back( wxVariant(wxString(std::to_string(now))) );
        data.push_back( wxVariant(mac) );
        wxString Date=wxString(asctime(localtime(&now)));
        Date.Replace("\n","");
        Date.Replace("\r","");
        data.push_back( wxVariant(Date));
        data.push_back(wxVariant(std::to_string(process.GetExitCode())));
        auto UpdateUI=[=]()
        {
            {
                //写历史表
                m_dataViewListCtrl_History->InsertItem(0,data );
            }

            {
                //写历史记录文件
                wxString MacHistoryLine;
                for(auto it=data.begin(); it!=data.end(); it++)
                {
                    MacHistoryLine+=wxString((*it).GetString())+",";
                }
                //末尾替换换行符
                MacHistoryLine=MacHistoryLine.replace(MacHistoryLine.length()-1,1,"\n");

                MacHistory.Write(MacHistoryLine);
                MacHistory.Flush();
            }

            {
                //保存mac地址二维码
                wxBitmap qrcode=GetQrCode(mac);
                qrcode.SaveFile(ProcessDir+"/"+mac+".png",wxBITMAP_TYPE_PNG);
            }
        };
        AddUIEvent(UpdateUI);
    };
    if(flashprocess!=NULL)
    {
        flashprocess->SetOnExit(ProcessExit);
    }
}

void WMToolHelperDialog::OnSubProcessStdout(int C)
{
    //子线程标准输出(一般用于在标准输出中查找信息)
    bool iseol=false;//是否为行结束符
    if(C == '\n' || C == '\r')
    {
        iseol=true;
    }
    else
    {
        if(C>0)
        {
            stdoutline+=((char)C);
        }
    }


    if(iseol)
    {
        //使用正则表达式查找mac地址(形如AA-BB-CC-DD-EE-FF)
        const char *regstr="([0-9]|[a-f]|[A-F]){2}(-([0-9]|[a-f]|[A-F]){2}){5}";
        regex_t reg= {0};
        if(regcomp(&reg,regstr,REG_EXTENDED)==0)
        {
            regmatch_t match[1];
            if(regexec(&reg,stdoutline.c_str(),sizeof(match)/sizeof(match[0]),match,0)==0)
            {
                if(match[0].rm_eo>match[0].rm_so)
                {
                    wxString Mac=wxString::FromUTF8(stdoutline.substr(match[0].rm_so,match[0].rm_eo-match[0].rm_so).c_str());
                    Mac.Replace("-","");
                    //处理mac字符串
                    SetButtonQrCode(Mac);
                    AddMacHistory(Mac);

                }
            }
        }
        regfree(&reg);
    }

    if(iseol)
    {
        stdoutline.clear();
    }


}

void WMToolHelperDialog::OnCleanupFile()
{
    //清理文件(进程目录)，通常退出时调用
    if(m_dataViewListCtrl_History->GetItemCount() <= 0)
    {
        wxLogMessage(_T("无历史文件，直接删除进程目录!"));
        //清理文件
        if(MacHistory.IsOpened())
        {
            MacHistory.Close();
        }
        wxDir::Remove(ProcessDir,wxPATH_RMDIR_RECURSIVE);
    }
    else
    {
        wxLogMessage(_T("存在历史文件，提示保存!"));
        {

            wxMessageDialog msgdlg(this,_T("是否保存历史文件"),_T("提示"),wxCENTER|wxYES_NO);
            if(wxID_YES==msgdlg.ShowModal())
            {
                wxFileDialog dlg(this,_T("保存文件:"),"","","Zip files (*.zip)|*.zip",wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
                if(wxID_OK==dlg.ShowModal())
                {
                    wxDir dir(ProcessDir);
                    wxFile file;
                    file.Open(dlg.GetPath(),wxFile::write);
                    if(file.IsOpened() && dir.IsOpened())
                    {
                        wxFileOutputStream filestream(file);
                        //保存zip文件
                        wxZipOutputStream zip(filestream);
                        wxArrayString filelist;
                        if(dir.GetAllFiles(ProcessDir,&filelist))
                        {
                            for(auto it=filelist.begin(); it!=filelist.end(); it++)
                            {
                                //处理文件路径与文件名
                                wxString filepath=(*it);
                                filepath.Replace("\\","/");
                                wxString filename=filepath.substr(ProcessDir.length());
                                while(filename.Find("/")==0)
                                {
                                    filename=filename.substr(1);
                                }

                                wxLogMessage(_T("文件路径:%s,文件名:%s"),filepath,filename);

                                wxFile subfile;
                                subfile.Open(filepath);
                                if(subfile.IsOpened())
                                {
                                    {
                                        //创建子文件
                                        zip.PutNextEntry(filename);
                                        wxFileInputStream input(subfile.fd());
                                        //输出子文件内容到zip文件
                                        zip<<input;
                                    }
                                    subfile.Close();
                                }

                            }
                        }

                        zip.Close();
                        filestream.Close();

                    }

                }
            }
        }

        //清理文件
        if(MacHistory.IsOpened())
        {
            MacHistory.Close();
        }
        wxDir::Remove(ProcessDir,wxPATH_RMDIR_RECURSIVE);
    }
}
