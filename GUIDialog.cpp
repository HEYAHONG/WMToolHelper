///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif //__BORLANDC__

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif //WX_PRECOMP

#include "GUIDialog.h"

///////////////////////////////////////////////////////////////////////////

GUIDialog::GUIDialog( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxSize( 800,600 ), wxDefaultSize );
	m_mgr.SetManagedWindow(this);
	m_mgr.SetFlags(wxAUI_MGR_DEFAULT);

	m_panel_center = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel_center->SetMinSize( wxSize( 600,600 ) );

	m_mgr.AddPane( m_panel_center, wxAuiPaneInfo() .Left() .CaptionVisible( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).MinSize( wxSize( 600,600 ) ).CentrePane() );

	wxBoxSizer* bSizer1;
	bSizer1 = new wxBoxSizer( wxVERTICAL );

	m_textCtrl_log = new wxTextCtrl( m_panel_center, wxID_ANY, wxT("WMToolHelper说明:\n\t串口      --->烧写芯片使用的串口号。\n\t烧写速率--->烧写芯片使用的速率。\n\t复位方式--->复位使用的方式。none=手动,at=使用AT指令,rts=使用串口的rts引脚。\n\t固件路径--->待烧写的固件路径。\n\t擦除操作--->是否擦除，通常不擦除会烧写失败。\n\t调试速率--->烧写完成后,是否进行串口调试。不需要调试时烧录完成后将自动退出。当调试速率不正确时，不启用调试功能。\n\t调试格式--->调试数据显示的格式。str=字符串,hex=16进制格式。\n\t操作      --->使用官方工具进行烧录操作。\n\t重试烧录--->单次烧录完成后是否重试烧录,启用调试操作时无效。异常退出=当烧录失败后重试烧录，正常退出=当烧录成功后重试烧录。\n"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY );
	bSizer1->Add( m_textCtrl_log, 1, wxALL|wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );


	m_panel_center->SetSizer( bSizer1 );
	m_panel_center->Layout();
	bSizer1->Fit( m_panel_center );
	m_panel_settings = new wxPanel( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	m_panel_settings->SetMinSize( wxSize( 200,600 ) );

	m_mgr.AddPane( m_panel_settings, wxAuiPaneInfo() .Left() .CloseButton( false ).PinButton( true ).Dock().Resizable().FloatingSize( wxDefaultSize ).BottomDockable( false ).TopDockable( false ).MinSize( wxSize( 300,600 ) ) );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	m_staticText1 = new wxStaticText( m_panel_settings, wxID_ANY, wxT("串口:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText1->Wrap( -1 );
	fgSizer1->Add( m_staticText1, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	m_panel_com = new wxPanel( m_panel_settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxHORIZONTAL );

	m_comboBox_com = new wxComboBox( m_panel_com, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 );
	m_comboBox_com->SetMinSize( wxSize( 120,-1 ) );

	bSizer3->Add( m_comboBox_com, 0, wxALL, 5 );

	m_button_refresh_com = new wxButton( m_panel_com, wxID_ANY, wxT("刷新串口"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer3->Add( m_button_refresh_com, 0, wxALL, 5 );


	m_panel_com->SetSizer( bSizer3 );
	m_panel_com->Layout();
	bSizer3->Fit( m_panel_com );
	fgSizer1->Add( m_panel_com, 1, wxEXPAND|wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	m_staticText2 = new wxStaticText( m_panel_settings, wxID_ANY, wxT("烧写速率:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText2->Wrap( -1 );
	fgSizer1->Add( m_staticText2, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_choice_downloadspeedChoices[] = { wxT("115200"), wxT("460800"), wxT("921600"), wxT("1000000"), wxT("2000000") };
	int m_choice_downloadspeedNChoices = sizeof( m_choice_downloadspeedChoices ) / sizeof( wxString );
	m_choice_downloadspeed = new wxChoice( m_panel_settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_downloadspeedNChoices, m_choice_downloadspeedChoices, 0 );
	m_choice_downloadspeed->SetSelection( 0 );
	m_choice_downloadspeed->SetMinSize( wxSize( 120,-1 ) );

	fgSizer1->Add( m_choice_downloadspeed, 0, wxALL, 5 );

	m_staticText3 = new wxStaticText( m_panel_settings, wxID_ANY, wxT("复位方式:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText3->Wrap( -1 );
	fgSizer1->Add( m_staticText3, 0, wxALL|wxALIGN_CENTER_VERTICAL|wxALIGN_RIGHT, 5 );

	wxString m_choice_resettypeChoices[] = { wxT("none"), wxT("at"), wxT("rts") };
	int m_choice_resettypeNChoices = sizeof( m_choice_resettypeChoices ) / sizeof( wxString );
	m_choice_resettype = new wxChoice( m_panel_settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_resettypeNChoices, m_choice_resettypeChoices, 0 );
	m_choice_resettype->SetSelection( 2 );
	m_choice_resettype->SetMinSize( wxSize( 120,-1 ) );

	fgSizer1->Add( m_choice_resettype, 0, wxALL, 5 );

	m_staticText4 = new wxStaticText( m_panel_settings, wxID_ANY, wxT("固件路径:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText4->Wrap( -1 );
	fgSizer1->Add( m_staticText4, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_filePicker_firmware = new wxFilePickerCtrl( m_panel_settings, wxID_ANY, wxEmptyString, wxT("选择固件"), wxT("*.fls;*.FLS"), wxDefaultPosition, wxDefaultSize, wxFLP_DEFAULT_STYLE|wxFLP_FILE_MUST_EXIST|wxFLP_OPEN|wxFLP_SMALL );
	m_filePicker_firmware->SetMinSize( wxSize( 200,-1 ) );

	fgSizer1->Add( m_filePicker_firmware, 0, wxALL, 5 );

	m_staticText5 = new wxStaticText( m_panel_settings, wxID_ANY, wxT("擦除操作:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText5->Wrap( -1 );
	fgSizer1->Add( m_staticText5, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_checkBox_eraseoption = new wxCheckBox( m_panel_settings, wxID_ANY, wxT("all"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkBox_eraseoption->SetValue(true);
	fgSizer1->Add( m_checkBox_eraseoption, 0, wxALL, 5 );

	m_staticText6 = new wxStaticText( m_panel_settings, wxID_ANY, wxT("调试速率:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText6->Wrap( -1 );
	fgSizer1->Add( m_staticText6, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_textCtrl_debugspeed = new wxTextCtrl( m_panel_settings, wxID_ANY, wxT("115200"), wxDefaultPosition, wxDefaultSize, wxTE_CENTER|wxTE_PROCESS_ENTER );
	m_textCtrl_debugspeed->SetMinSize( wxSize( 120,-1 ) );

	fgSizer1->Add( m_textCtrl_debugspeed, 0, wxALL, 5 );

	m_staticText7 = new wxStaticText( m_panel_settings, wxID_ANY, wxT("调试格式:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText7->Wrap( -1 );
	fgSizer1->Add( m_staticText7, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	wxString m_choice_debugtypeChoices[] = { wxT("str"), wxT("hex"), wxT("nodebug"), wxT("putty") };
	int m_choice_debugtypeNChoices = sizeof( m_choice_debugtypeChoices ) / sizeof( wxString );
	m_choice_debugtype = new wxChoice( m_panel_settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choice_debugtypeNChoices, m_choice_debugtypeChoices, 0 );
	m_choice_debugtype->SetSelection( 0 );
	m_choice_debugtype->SetMinSize( wxSize( 120,-1 ) );

	fgSizer1->Add( m_choice_debugtype, 0, wxALL, 5 );

	m_staticText9 = new wxStaticText( m_panel_settings, wxID_ANY, wxT("操作:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText9->Wrap( -1 );
	fgSizer1->Add( m_staticText9, 0, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5 );

	m_panel_operation = new wxPanel( m_panel_settings, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer( wxHORIZONTAL );

	m_button_start = new wxButton( m_panel_operation, wxID_ANY, wxT("开始"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer2->Add( m_button_start, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_button_stop = new wxButton( m_panel_operation, wxID_ANY, wxT("停止"), wxDefaultPosition, wxDefaultSize, 0 );
	m_button_stop->Enable( false );

	bSizer2->Add( m_button_stop, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


	m_panel_operation->SetSizer( bSizer2 );
	m_panel_operation->Layout();
	bSizer2->Fit( m_panel_operation );
	fgSizer1->Add( m_panel_operation, 1, wxEXPAND | wxALL, 5 );

	m_staticText_retry = new wxStaticText( m_panel_settings, wxID_ANY, wxT("重试烧录:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticText_retry->Wrap( -1 );
	fgSizer1->Add( m_staticText_retry, 0, wxALL, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxHORIZONTAL );

	m_checkBox_FlashProgressExitAbnormal = new wxCheckBox( m_panel_settings, wxID_ANY, wxT("异常退出"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_checkBox_FlashProgressExitAbnormal, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );

	m_checkBox_FlashProgressExitNormal = new wxCheckBox( m_panel_settings, wxID_ANY, wxT("正常退出"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizer4->Add( m_checkBox_FlashProgressExitNormal, 1, wxALL|wxALIGN_CENTER_VERTICAL|wxEXPAND, 5 );


	fgSizer1->Add( bSizer4, 1, wxEXPAND, 5 );


	m_panel_settings->SetSizer( fgSizer1 );
	m_panel_settings->Layout();
	fgSizer1->Fit( m_panel_settings );
	m_timer_refresh.SetOwner( this, wxID_REFRESH_TIMER );
	m_timer_refresh.Start( 200 );


	m_mgr.Update();

	// Connect Events
	this->Connect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIDialog::OnClose ) );
	m_button_refresh_com->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialog::OnButtonRefreshCom ), NULL, this );
	m_button_start->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialog::OnButtonStart ), NULL, this );
	m_button_stop->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialog::OnButtonStop ), NULL, this );
	this->Connect( wxID_REFRESH_TIMER, wxEVT_TIMER, wxTimerEventHandler( GUIDialog::OnRefreshTimer ) );
}

GUIDialog::~GUIDialog()
{
	// Disconnect Events
	this->Disconnect( wxEVT_CLOSE_WINDOW, wxCloseEventHandler( GUIDialog::OnClose ) );
	m_button_refresh_com->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialog::OnButtonRefreshCom ), NULL, this );
	m_button_start->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialog::OnButtonStart ), NULL, this );
	m_button_stop->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( GUIDialog::OnButtonStop ), NULL, this );
	this->Disconnect( wxID_REFRESH_TIMER, wxEVT_TIMER, wxTimerEventHandler( GUIDialog::OnRefreshTimer ) );

	m_mgr.UnInit();

}
