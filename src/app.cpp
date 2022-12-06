#include<app.h>

#include<MainFrame.h>

bool App::OnInit()
{
	MainFrame* frame = new MainFrame(wxT("Emv Parser Tool"), wxDefaultPosition, wxSize(800, 600));
	frame->Show();
	return true;
}

wxIMPLEMENT_APP(App);