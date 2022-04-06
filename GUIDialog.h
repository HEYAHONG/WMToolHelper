///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/textctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/stattext.h>
#include <wx/combobox.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/choice.h>
#include <wx/filepicker.h>
#include <wx/checkbox.h>
#include <wx/valtext.h>
#include <wx/timer.h>
#include <wx/dialog.h>
#include <wx/aui/aui.h>

///////////////////////////////////////////////////////////////////////////

#define wxID_REFRESH_TIMER 1000

///////////////////////////////////////////////////////////////////////////////
/// Class GUIDialog
///////////////////////////////////////////////////////////////////////////////
class GUIDialog : public wxDialog
{
	private:

	protected:
		wxPanel* m_panel_center;
		wxTextCtrl* m_textCtrl_log;
		wxPanel* m_panel_settings;
		wxStaticText* m_staticText1;
		wxPanel* m_panel_com;
		wxComboBox* m_comboBox_com;
		wxButton* m_button_refresh_com;
		wxStaticText* m_staticText2;
		wxChoice* m_choice_downloadspeed;
		wxStaticText* m_staticText3;
		wxChoice* m_choice_resettype;
		wxStaticText* m_staticText4;
		wxFilePickerCtrl* m_filePicker_firmware;
		wxStaticText* m_staticText5;
		wxCheckBox* m_checkBox_eraseoption;
		wxStaticText* m_staticText6;
		wxTextCtrl* m_textCtrl_debugspeed;
		wxStaticText* m_staticText7;
		wxChoice* m_choice_debugtype;
		wxStaticText* m_staticText9;
		wxPanel* m_panel_operation;
		wxButton* m_button_start;
		wxButton* m_button_stop;
		wxStaticText* m_staticText_retry;
		wxCheckBox* m_checkBox_FlashProgressExitAbnormal;
		wxCheckBox* m_checkBox_FlashProgressExitNormal;
		wxTimer m_timer_refresh;

		// Virtual event handlers, override them in your derived class
		virtual void OnClose( wxCloseEvent& event ) { event.Skip(); }
		virtual void OnButtonRefreshCom( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonStart( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonStop( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRefreshTimer( wxTimerEvent& event ) { event.Skip(); }


	public:
		wxString debugspeed_validator;

		GUIDialog( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("WMToolHelper"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxMAXIMIZE_BOX|wxMINIMIZE_BOX|wxRESIZE_BORDER );
		wxAuiManager m_mgr;

		~GUIDialog();

};

