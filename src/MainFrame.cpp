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
	wxVector<unsigned char> outputVector;
	for (size_t i = 0, j = 0; i < dsp.size() / 2; i += 2, j++)
	{
		wxString hexDigits = dsp.Mid(i, 2);
		//output[j] = wxHexToDec(hexDigits);
		outputVector.push_back(wxHexToDec(hexDigits));
	}
	std::copy(outputVector.begin(), outputVector.end(), output);
}

bool SanitizeString(wxString& input)
{
	wxRegEx clearSpacesAndXs(wxT("[^0-9A-F]\s*"), wxRE_NEWLINE);
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
	
	/*unsigned char output[2069] = { 0x9F, 0x33, 0x03, 0xE0, 0xB0, 0xC8, 0x9F, 0x34, 0x03, 0x1E, 0x03, 0x00,
									0x9F, 0x35, 0x01, 0x22, 0x9F, 0x10, 0x12, 0x01, 0x10, 0xA0, 0x00, 0x01, 0x24, 0x00, 0x00, 0x00,
									0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0x82, 0x02, 0x58, 0x00, 0x9F, 0x36, 0x02,
									0x08, 0xBE, 0x9F, 0x26, 0x08, 0xF7, 0xAD, 0x29, 0x00, 0x51, 0x45, 0x76, 0x32, 0x9F, 0x27, 0x01,
									0x80, 0x95, 0x05, 0x42, 0x40, 0x00, 0x80, 0x00, 0x9F, 0x37, 0x04, 0xF1, 0x00, 0x6E, 0xEB, 0x9F,
									0x02, 0x06, 0x00, 0x00, 0x00, 0x03, 0x33, 0x33, 0x9F, 0x03, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00,
									0x00, 0x9F, 0x1A, 0x02, 0x02, 0x14, 0x5F, 0x2A, 0x02, 0x02, 0x14, 0x9A, 0x03, 0x22, 0x10, 0x11,
									0x9C, 0x01, 0x00, 0x9F, 0x1E, 0x09, 0x32, 0x36, 0x32, 0x38, 0x31, 0x34, 0x31, 0x32, 0x36, 0x5F,
									0x34, 0x01, 0x00, 0x84, 0x07, 0xA0, 0x00, 0x00, 0x00, 0x04, 0x10, 0x10, 0x9F, 0x09, 0x02, 0x00,
									0x02, 0x9F, 0x41, 0x04, 0x00, 0x00, 0x00, 0x05, 0x9F, 0x53, 0x01, 0x52 }; */
	//unsigned char output[2069] = { 0x00,0x00,0x00 };
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
		/*for (size_t i = 0; i < tindex; i++)
		{
			wxString msg = wxEmptyString;
			msg.Printf("%X\n", t[i].tlv.Tag);
			strcat(result, msg.c_str());
		}*/
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