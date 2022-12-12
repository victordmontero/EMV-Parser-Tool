#include<MainFrame.h>
#include<wx/regex.h>
extern "C"
{
#include<emvTagList.h>
}

dict_t* dict[HASHSIZE];

MainFrame::MainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
	: wxFrame(NULL, wxID_ANY, title, pos, size)
{
	SetMinSize(size);

	wxMenuBar* menuBar = new wxMenuBar();

	wxMenu* menuFile = new wxMenu();
	wxMenu* menuAbout = new wxMenu();

	menuFile->Append(ID_SAVE_LOG, wxT("Save Logs\tCtlr-S"));
	menuFile->AppendSeparator();
	menuFile->Append(wxID_EXIT, wxT("Exit\tAlt-x"));
	menuAbout->Append(wxID_ABOUT);

	menuBar->Append(menuFile, "&File");
	menuBar->Append(menuAbout, "&About");

	SetMenuBar(menuBar);

	CreateStatusBar();
	SetStatusText("Welcome to Emv Parser Tool");

	wxPanel* panel = new wxPanel(this);

	wxSizer* sizerA = new wxBoxSizer(wxVERTICAL);
	wxSizer* sizerB = new wxBoxSizer(wxVERTICAL);
	wxSizer* sizerC = new wxBoxSizer(wxHORIZONTAL);
	wxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

	txtEmvInput = new wxTextCtrl(panel, ID_TXT_EMV_INPUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
	txtEmvOutput = new wxTextCtrl(panel, ID_TXT_EMV_OUTPUT, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_READONLY | wxTE_PROCESS_TAB);

	wxButton* buttonParse = new wxButton(panel, ID_BTN_PARSER, wxT("Parse"));
	wxButton* buttonClear = new wxButton(panel, ID_BTN_CLEAR, wxT("Clear"));

	sizerA->Add(txtEmvInput, 1, wxALL | wxEXPAND, 5);
	sizerB->Add(txtEmvOutput, 1, wxALL | wxEXPAND, 5);
	sizerC->Add(buttonParse);
	sizerC->Add(buttonClear);

	sizerA->Add(sizerC, 0, wxALL | wxEXPAND, 5);

	mainSizer->Add(sizerA, 1, wxALL | wxEXPAND);
	mainSizer->Add(sizerB, 1, wxALL | wxEXPAND);

	panel->SetSizerAndFit(mainSizer);

	Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
	Bind(wxEVT_MENU, &MainFrame::OnQuit, this, wxID_EXIT);
	Bind(wxEVT_MENU, &MainFrame::OnSaveLog, this, ID_SAVE_LOG);
	Bind(wxEVT_BUTTON, &MainFrame::OnParse, this, ID_BTN_PARSER);
	Bind(wxEVT_BUTTON, &MainFrame::OnClear, this, ID_BTN_CLEAR);

	memset(dict, 0, sizeof(dict));
	emvInit(dict);
}

MainFrame::~MainFrame()
{

}

void MainFrame::OnQuit(wxCommandEvent& event)
{
	Close();
	event.Skip();
}

void MainFrame::OnAbout(wxCommandEvent& event)
{
	wxString msg;
	msg.Printf(wxT("EMV Tool by Victor D. Montero built with %s"), wxVERSION_STRING);

	wxMessageBox(
		msg,
		"About EMV Tool",
		wxOK | wxICON_INFORMATION);
	event.Skip();
}

void Display2Hexadecimal(const wxString& dsp, unsigned char* output)
{
	size_t count = dsp.size();
	wxVector<unsigned char> outputVector;
	for (size_t i = 0, j = 0; i < count; i += 2, j++)
	{
		wxString hexDigits = dsp.Mid(i, 2);
		//output[j] = wxHexToDec(hexDigits);
		outputVector.push_back(wxHexToDec(hexDigits));
	}
	std::copy(outputVector.begin(), outputVector.end(), output);
}

bool SanitizeString(wxString& input)
{
	wxRegEx clearSpacesAndXs(wxT("([^0-9A-F])|(0[Xx])"));
	wxRegEx hexFormat(wxT("[^A-F0-9]"), wxRE_NEWLINE);
	input.Trim();

	clearSpacesAndXs.ReplaceAll(&input, wxEmptyString);

	return !hexFormat.Matches(input);
}

void MainFrame::OnSaveLog(wxCommandEvent& event)
{
	event.Skip();
}

void MainFrame::OnParse(wxCommandEvent& event)
{
	char result[4096] = { 0 };

	wxString input = txtEmvInput->GetValue();
	if (SanitizeString(input))
	{
		unsigned char output[4096] = { 0 };
		Display2Hexadecimal(input, output);

		tlvInfo_t* t = new tlvInfo_t[input.size() / 2];
		tlvInfo_init(t);
		int tindex = 0;
		emvParse(output, input.size() / 2, t, &tindex, 0, dict);
		emvPrint_resultString(t, tindex, result);
		txtEmvOutput->SetValue(result);

		delete[] t;
	}

	
	event.Skip();
}

void MainFrame::OnClear(wxCommandEvent& event)
{
	event.Skip();
}