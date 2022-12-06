#ifndef MAIN_FRAME_H_
#define MAIN_FRAME_H_

#include <wx/wxprec.h>

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

enum Ids
{
	ID_SAVE_LOG,
	ID_TXT_EMV_INPUT,
	ID_TXT_EMV_OUTPUT,
	ID_BTN_PARSER,
	ID_BTN_CLEAR
};

class MainFrame : public wxFrame
{
public:
	MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
	~MainFrame();

	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	void OnSaveLog(wxCommandEvent& event);
	
	void OnParse(wxCommandEvent& event);
	void OnClear(wxCommandEvent& event);

protected:
	wxTextCtrl* txtEmvInput;
	wxTextCtrl* txtEmvOutput;
};

#endif
