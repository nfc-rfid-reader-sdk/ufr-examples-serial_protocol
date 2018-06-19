/***************************************************************
 * Name:      ufr_com_protocol_sending_commandMain.cpp
 * Purpose:   Code for Application Frame
 * Author:    Aleksandar (Krstic)
 * Created:   2018-06-18
 * Copyright: Aleksandar (aleksandar.krstic@d-logic.rs)
 * License:
 **************************************************************/

#include "ufr_com_protocol_sending_commandMain.h"
#include <wx/msgdlg.h>
#include "ftdi.h"
#include "ftd2xx.h"
#include "func.h"
#include <iomanip>
#include <string>
#include <string.h>
#include <cstring>
#include <iostream>
#define CMD_HEADER 0x55
#define CMD_TRAILER 0xAA

using namespace std;

FT_HANDLE ftHandle;
uint32_t ConvertStringToInt(string str, int base);
void ConvertStringToIntArray(string str, uint8_t *array, int base);
//(*InternalHeaders(ufr_com_protocol_sending_commandFrame)
#include <wx/intl.h>
#include <wx/settings.h>
#include <wx/string.h>
//*)

//helper functions
enum wxbuildinfoformat {
    short_f, long_f };

wxString wxbuildinfo(wxbuildinfoformat format)
{
    wxString wxbuild(wxVERSION_STRING);

    if (format == long_f )
    {
#if defined(__WXMSW__)
        wxbuild << _T("-Windows");
#elif defined(__UNIX__)
        wxbuild << _T("-Linux");
#endif

#if wxUSE_UNICODE
        wxbuild << _T("-Unicode build");
#else
        wxbuild << _T("-ANSI build");
#endif // wxUSE_UNICODE
    }

    return wxbuild;
}

//(*IdInit(ufr_com_protocol_sending_commandFrame)
const long ufr_com_protocol_sending_commandFrame::ID_BUTTON1 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_BUTTON2 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_TEXTCTRL1 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT1 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT2 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT3 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT4 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT5 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_TEXTCTRL2 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_TEXTCTRL3 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_TEXTCTRL4 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_TEXTCTRL5 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_BUTTON3 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT9 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_TEXTCTRL7 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT6 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_TEXTCTRL6 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_BUTTON4 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT7 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT12 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_GRID1 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_TEXTCTRL9 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT11 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_GRID3 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT8 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_GRID2 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATICTEXT10 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_TEXTCTRL8 = wxNewId();
const long ufr_com_protocol_sending_commandFrame::idMenuQuit = wxNewId();
const long ufr_com_protocol_sending_commandFrame::idMenuAbout = wxNewId();
const long ufr_com_protocol_sending_commandFrame::ID_STATUSBAR1 = wxNewId();
//*)

BEGIN_EVENT_TABLE(ufr_com_protocol_sending_commandFrame,wxFrame)
    //(*EventTable(ufr_com_protocol_sending_commandFrame)
    //*)
END_EVENT_TABLE()

ufr_com_protocol_sending_commandFrame::ufr_com_protocol_sending_commandFrame(wxWindow* parent,wxWindowID id)
{
    //(*Initialize(ufr_com_protocol_sending_commandFrame)
    wxGridSizer* GridSizer1;
    wxGridSizer* GridSizer2;
    wxGridSizer* GridSizer3;
    wxGridSizer* GridSizer4;
    wxGridSizer* GridSizer5;
    wxGridSizer* GridSizer6;
    wxMenu* Menu1;
    wxMenu* Menu2;
    wxMenuBar* MenuBar1;
    wxMenuItem* MenuItem1;
    wxMenuItem* MenuItem2;
    wxStaticBoxSizer* StaticBoxSizer1;

    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDEFAULT_FRAME_STYLE, _T("wxID_ANY"));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    GridSizer1 = new wxGridSizer(6, 1, -45, 0);
    StaticBoxSizer1 = new wxStaticBoxSizer(wxHORIZONTAL, this, wxEmptyString);
    bConnect = new wxButton(this, ID_BUTTON1, _("CONNECT"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON1"));
    StaticBoxSizer1->Add(bConnect, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    bDisconnect = new wxButton(this, ID_BUTTON2, _("DISCONNECT"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON2"));
    StaticBoxSizer1->Add(bDisconnect, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    Connect_Status = new wxTextCtrl(this, ID_TEXTCTRL1, wxEmptyString, wxDefaultPosition, wxSize(250,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL1"));
    StaticBoxSizer1->Add(Connect_Status, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer1->Add(StaticBoxSizer1, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    GridSizer2 = new wxGridSizer(3, 3, -25, -300);
    StaticText1 = new wxStaticText(this, ID_STATICTEXT1, _("CMD : "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT1"));
    GridSizer2->Add(StaticText1, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer3 = new wxGridSizer(2, 4, 0, 0);
    StaticText2 = new wxStaticText(this, ID_STATICTEXT2, _("CMD_CODE"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT2"));
    GridSizer3->Add(StaticText2, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    StaticText3 = new wxStaticText(this, ID_STATICTEXT3, _("CMD_Ext_Length"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT3"));
    GridSizer3->Add(StaticText3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    StaticText4 = new wxStaticText(this, ID_STATICTEXT4, _("CMD_Par0"), wxDefaultPosition, wxSize(45,-1), 0, _T("ID_STATICTEXT4"));
    GridSizer3->Add(StaticText4, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    StaticText5 = new wxStaticText(this, ID_STATICTEXT5, _("CMD_Par1"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT5"));
    GridSizer3->Add(StaticText5, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    CMD_CODE_LABEL = new wxTextCtrl(this, ID_TEXTCTRL2, wxEmptyString, wxDefaultPosition, wxSize(45,-1), wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL2"));
    GridSizer3->Add(CMD_CODE_LABEL, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    CMD_EXT_LENGTH_LABEL = new wxTextCtrl(this, ID_TEXTCTRL3, wxEmptyString, wxDefaultPosition, wxSize(45,-1), wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL3"));
    GridSizer3->Add(CMD_EXT_LENGTH_LABEL, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    CMD_PAR0_LABEL = new wxTextCtrl(this, ID_TEXTCTRL4, wxEmptyString, wxDefaultPosition, wxSize(45,-1), wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL4"));
    GridSizer3->Add(CMD_PAR0_LABEL, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    CMD_PAR1_LABEL = new wxTextCtrl(this, ID_TEXTCTRL5, wxEmptyString, wxDefaultPosition, wxSize(45,-1), wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL5"));
    GridSizer3->Add(CMD_PAR1_LABEL, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer2->Add(GridSizer3, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    bSend = new wxButton(this, ID_BUTTON3, _("Send"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON3"));
    GridSizer2->Add(bSend, 1, wxALL|wxALIGN_RIGHT|wxALIGN_CENTER_VERTICAL, 5);
    StaticText9 = new wxStaticText(this, ID_STATICTEXT9, _("CMD_Ext : "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT9"));
    GridSizer2->Add(StaticText9, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    CMD_EXT_LABEL = new wxTextCtrl(this, ID_TEXTCTRL7, wxEmptyString, wxDefaultPosition, wxSize(370,-1), 0, wxDefaultValidator, _T("ID_TEXTCTRL7"));
    GridSizer2->Add(CMD_EXT_LABEL, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer2->Add(-1,-1,1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    StaticText6 = new wxStaticText(this, ID_STATICTEXT6, _("CMD Status : "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT6"));
    GridSizer2->Add(StaticText6, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    Command_Status = new wxTextCtrl(this, ID_TEXTCTRL6, wxEmptyString, wxDefaultPosition, wxSize(370,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL6"));
    GridSizer2->Add(Command_Status, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    bClear = new wxButton(this, ID_BUTTON4, _("Clear"), wxDefaultPosition, wxDefaultSize, 0, wxDefaultValidator, _T("ID_BUTTON4"));
    GridSizer2->Add(bClear, 1, wxALL|wxALIGN_RIGHT|wxALIGN_TOP, 5);
    GridSizer1->Add(GridSizer2, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer4 = new wxGridSizer(2, 2, -30, 0);
    StaticText7 = new wxStaticText(this, ID_STATICTEXT7, _("SENT CMD : "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT7"));
    GridSizer4->Add(StaticText7, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    StaticText12 = new wxStaticText(this, ID_STATICTEXT12, _("Function informations : "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT12"));
    GridSizer4->Add(StaticText12, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    CMD_GRID = new wxGrid(this, ID_GRID1, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GRID1"));
    CMD_GRID->CreateGrid(1,7);
    CMD_GRID->EnableEditing(true);
    CMD_GRID->EnableGridLines(true);
    CMD_GRID->SetDefaultRowSize(30, true);
    CMD_GRID->SetDefaultColSize(100, true);
    CMD_GRID->SetColLabelValue(0, _("CMD_HEADER"));
    CMD_GRID->SetColLabelValue(1, _("CMD_CODE"));
    CMD_GRID->SetColLabelValue(2, _("CMD_TRAILER"));
    CMD_GRID->SetColLabelValue(3, _("CMD_Ext_Length"));
    CMD_GRID->SetColLabelValue(4, _("CMD_Par0"));
    CMD_GRID->SetColLabelValue(5, _("CMD_Par1"));
    CMD_GRID->SetColLabelValue(6, _("CHECKSUM"));
    CMD_GRID->SetRowLabelValue(0, _("value"));
    CMD_GRID->SetDefaultCellFont( CMD_GRID->GetFont() );
    CMD_GRID->SetDefaultCellTextColour( CMD_GRID->GetForegroundColour() );
    GridSizer4->Add(CMD_GRID, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    Function_Label = new wxTextCtrl(this, ID_TEXTCTRL9, wxEmptyString, wxDefaultPosition, wxSize(350,60), wxTE_MULTILINE|wxTE_READONLY, wxDefaultValidator, _T("ID_TEXTCTRL9"));
    GridSizer4->Add(Function_Label, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    GridSizer1->Add(GridSizer4, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    GridSizer5 = new wxGridSizer(4, 1, -30, 0);
    StaticText11 = new wxStaticText(this, ID_STATICTEXT11, _("ACKNOWLEDGE :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT11"));
    GridSizer5->Add(StaticText11, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    ACKNOWLEDGE_GRID = new wxGrid(this, ID_GRID3, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GRID3"));
    ACKNOWLEDGE_GRID->CreateGrid(1,7);
    ACKNOWLEDGE_GRID->EnableEditing(true);
    ACKNOWLEDGE_GRID->EnableGridLines(true);
    ACKNOWLEDGE_GRID->SetDefaultRowSize(30, true);
    ACKNOWLEDGE_GRID->SetDefaultColSize(100, true);
    ACKNOWLEDGE_GRID->SetColLabelValue(0, _("Byte 1"));
    ACKNOWLEDGE_GRID->SetColLabelValue(1, _("Byte 2"));
    ACKNOWLEDGE_GRID->SetColLabelValue(2, _("Byte 3"));
    ACKNOWLEDGE_GRID->SetColLabelValue(3, _("Byte 4"));
    ACKNOWLEDGE_GRID->SetColLabelValue(4, _("Byte 5"));
    ACKNOWLEDGE_GRID->SetColLabelValue(5, _("Byte 6"));
    ACKNOWLEDGE_GRID->SetColLabelValue(6, _("Byte 7"));
    ACKNOWLEDGE_GRID->SetRowLabelValue(0, _("value"));
    ACKNOWLEDGE_GRID->SetDefaultCellFont( ACKNOWLEDGE_GRID->GetFont() );
    ACKNOWLEDGE_GRID->SetDefaultCellTextColour( ACKNOWLEDGE_GRID->GetForegroundColour() );
    GridSizer5->Add(ACKNOWLEDGE_GRID, 1, wxALL|wxALIGN_TOP|wxALIGN_CENTER_HORIZONTAL, 5);
    StaticText8 = new wxStaticText(this, ID_STATICTEXT8, _("RESPONSE :"), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT8"));
    GridSizer5->Add(StaticText8, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    RSP_GRID = new wxGrid(this, ID_GRID2, wxDefaultPosition, wxDefaultSize, 0, _T("ID_GRID2"));
    RSP_GRID->CreateGrid(1,7);
    RSP_GRID->EnableEditing(true);
    RSP_GRID->EnableGridLines(true);
    RSP_GRID->SetDefaultRowSize(30, true);
    RSP_GRID->SetDefaultColSize(100, true);
    RSP_GRID->SetColLabelValue(0, _("Byte 1"));
    RSP_GRID->SetColLabelValue(1, _("Byte 2"));
    RSP_GRID->SetColLabelValue(2, _("Byte 3"));
    RSP_GRID->SetColLabelValue(3, _("Byte 4"));
    RSP_GRID->SetColLabelValue(4, _("Byte 5"));
    RSP_GRID->SetColLabelValue(5, _("Byte 6"));
    RSP_GRID->SetColLabelValue(6, _("Byte 7"));
    RSP_GRID->SetRowLabelValue(0, _("value"));
    RSP_GRID->SetDefaultCellFont( RSP_GRID->GetFont() );
    RSP_GRID->SetDefaultCellTextColour( RSP_GRID->GetForegroundColour() );
    GridSizer5->Add(RSP_GRID, 1, wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer1->Add(GridSizer5, 1, wxALL|wxALIGN_LEFT|wxALIGN_TOP, 5);
    GridSizer6 = new wxGridSizer(1, 2, -50, -620);
    StaticText10 = new wxStaticText(this, ID_STATICTEXT10, _("RESPONSE EXT : "), wxDefaultPosition, wxDefaultSize, 0, _T("ID_STATICTEXT10"));
    GridSizer6->Add(StaticText10, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    RSP_EXT_LABEL = new wxTextCtrl(this, ID_TEXTCTRL8, wxEmptyString, wxDefaultPosition, wxSize(700,-1), wxTE_READONLY|wxTE_CENTRE, wxDefaultValidator, _T("ID_TEXTCTRL8"));
    GridSizer6->Add(RSP_EXT_LABEL, 1, wxALL|wxALIGN_LEFT|wxALIGN_CENTER_VERTICAL, 5);
    GridSizer1->Add(GridSizer6, 1, wxALL|wxALIGN_LEFT|wxALIGN_BOTTOM, 5);
    SetSizer(GridSizer1);
    MenuBar1 = new wxMenuBar();
    Menu1 = new wxMenu();
    MenuItem1 = new wxMenuItem(Menu1, idMenuQuit, _("Quit\tAlt-F4"), _("Quit the application"), wxITEM_NORMAL);
    Menu1->Append(MenuItem1);
    MenuBar1->Append(Menu1, _("&File"));
    Menu2 = new wxMenu();
    MenuItem2 = new wxMenuItem(Menu2, idMenuAbout, _("About\tF1"), _("Show info about this application"), wxITEM_NORMAL);
    Menu2->Append(MenuItem2);
    MenuBar1->Append(Menu2, _("Help"));
    SetMenuBar(MenuBar1);
    StatusBar1 = new wxStatusBar(this, ID_STATUSBAR1, 0, _T("ID_STATUSBAR1"));
    int __wxStatusBarWidths_1[1] = { -1 };
    int __wxStatusBarStyles_1[1] = { wxSB_NORMAL };
    StatusBar1->SetFieldsCount(1,__wxStatusBarWidths_1);
    StatusBar1->SetStatusStyles(1,__wxStatusBarStyles_1);
    SetStatusBar(StatusBar1);
    GridSizer1->Fit(this);
    GridSizer1->SetSizeHints(this);

    Connect(ID_BUTTON1,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ufr_com_protocol_sending_commandFrame::OnbConnectClick);
    Connect(ID_BUTTON2,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ufr_com_protocol_sending_commandFrame::OnbDisconnectClick);
    Connect(ID_BUTTON3,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ufr_com_protocol_sending_commandFrame::OnbSendClick);
    Connect(ID_BUTTON4,wxEVT_COMMAND_BUTTON_CLICKED,(wxObjectEventFunction)&ufr_com_protocol_sending_commandFrame::OnbClearClick);
    Connect(idMenuQuit,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ufr_com_protocol_sending_commandFrame::OnQuit);
    Connect(idMenuAbout,wxEVT_COMMAND_MENU_SELECTED,(wxObjectEventFunction)&ufr_com_protocol_sending_commandFrame::OnAbout);
    //*)
}

ufr_com_protocol_sending_commandFrame::~ufr_com_protocol_sending_commandFrame()
{
    //(*Destroy(ufr_com_protocol_sending_commandFrame)
    //*)
}

void ufr_com_protocol_sending_commandFrame::OnQuit(wxCommandEvent& event)
{
    Close();
}

void ufr_com_protocol_sending_commandFrame::OnAbout(wxCommandEvent& event)
{
    wxString msg = wxbuildinfo(long_f);
    wxMessageBox(msg, _("Welcome to..."));
}

void ufr_com_protocol_sending_commandFrame::OnbConnectClick(wxCommandEvent& event)
{
    FT_STATUS ftStatus;

    ftStatus = ftdi_open(1000000, 500, &ftHandle);

    if(ftStatus == FT_OK){


        Connect_Status->SetValue("Port successfully opened.");
        Connect_Status->SetBackgroundColour("green");

    }else{

        Connect_Status->SetValue("Error while trying to open port.");
        Connect_Status->SetBackgroundColour("red");

    }

}



void ufr_com_protocol_sending_commandFrame::OnbDisconnectClick(wxCommandEvent& event)
{
    FT_STATUS ftStatus;

    ftStatus = FT_Close(ftHandle);

    if(ftStatus == FT_OK){


        Connect_Status->SetValue("Port successfully closed.");
        Connect_Status->SetBackgroundColour("green");

    }else{

        Connect_Status->SetValue("Error while trying to close port.");
        Connect_Status->SetBackgroundColour("red");

    }

}


void ufr_com_protocol_sending_commandFrame::OnbSendClick(wxCommandEvent& event)
{

    FT_STATUS ftStatus;
    wxString CMD_CODE = CMD_CODE_LABEL->GetValue();
    wxString CMD_EXT_LENGTH = CMD_EXT_LENGTH_LABEL->GetValue();
    wxString CMD_PAR0 = CMD_PAR0_LABEL->GetValue();
    wxString CMD_PAR1 = CMD_PAR1_LABEL->GetValue();
    wxString CMD_EXT = "";
    uint8_t Buffer[1000];
    uint8_t Buffer_EXT[1000];
    uint8_t Buffer_to_read[1000];
    uint32_t Buffer_to_read_integer[1000];
    char BUFFER_CHAR[1000] = "";
    DWORD BytesWritten;
    DWORD BytesReturned;
    wxString wxREADBUFFER = "";
    string CMD_EXT_Str = "";

    memset(Buffer, 0, sizeof(Buffer));

    string CMD_CODE_Str = string(CMD_CODE.mb_str());
    string CMD_EXT_LENGTH_Str = string(CMD_EXT_LENGTH.mb_str());
    string CMD_PAR0_Str = string(CMD_PAR0.mb_str());
    string CMD_PAR1_Str = string(CMD_PAR1.mb_str());

    CMD_CODE_Str = CMD_CODE_Str.substr(0, 2);
    CMD_EXT_LENGTH_Str = CMD_EXT_LENGTH_Str.substr(0,2);
    CMD_PAR0_Str = CMD_PAR0_Str.substr(0, 2);
    CMD_PAR1_Str = CMD_PAR1_Str.substr(0, 2);

    Buffer[0] = CMD_HEADER;
    Buffer[1] = (uint8_t)ConvertStringToInt(CMD_CODE_Str, 16);
    Buffer[2] = CMD_TRAILER;
    Buffer[3] = (uint8_t)ConvertStringToInt(CMD_EXT_LENGTH_Str, 16);
    Buffer[4] = (uint8_t)ConvertStringToInt(CMD_PAR0_Str, 16);
    Buffer[5] = (uint8_t)ConvertStringToInt(CMD_PAR1_Str, 16);
    Buffer[6] = ((((((Buffer[0]^Buffer[1])^Buffer[2])^Buffer[3])^Buffer[4])^Buffer[5])+7);


    Function_Label->Clear();
    Function_Label->SetValue("Function name : " + FuncMap[Buffer[1]] +
                             "\n\nCMD : ");
      for(unsigned int i = 0; i < 7; i++){

                Buffer_to_read_integer[i] = Buffer[i];

                sprintf(BUFFER_CHAR, "%02X", Buffer_to_read_integer[i]);

                Function_Label->AppendText("0x");
                Function_Label->AppendText(BUFFER_CHAR);
                Function_Label->AppendText(" ");
    }

    if(Buffer[3] > 0x00){

            CMD_EXT = CMD_EXT_LABEL->GetValue();

            CMD_EXT_Str = string(CMD_EXT.mb_str());

            ConvertStringToIntArray(CMD_EXT_Str, Buffer_EXT, 16);

            ftStatus = FT_Write(ftHandle, Buffer, 7, &BytesWritten);

            ftStatus = FT_Read(ftHandle, Buffer_to_read, sizeof(Buffer_to_read), &BytesReturned);

            for(unsigned int i = 0; i < 7; i++){

                Buffer_to_read_integer[i] = Buffer[i];

                sprintf(BUFFER_CHAR, "%02X", Buffer_to_read_integer[i]);

                CMD_GRID->SetCellValue(0, i, BUFFER_CHAR);

            }

            for(unsigned int i = 0; i < 7; i++){

                Buffer_to_read_integer[i] = Buffer_to_read[i];

                sprintf(BUFFER_CHAR, "%02X", Buffer_to_read_integer[i]);

                ACKNOWLEDGE_GRID->SetCellValue(0, i, BUFFER_CHAR);

            }

    if(ftStatus != FT_OK || Buffer_to_read[0] != 0xAC){

        Command_Status->SetValue("Failed to send command. Try again.");

    }else{

            Command_Status->SetValue("Command successfully sent.");

            ftStatus = FT_Write(ftHandle, Buffer_EXT, sizeof(Buffer_EXT), &BytesWritten);

            if(ftStatus != FT_OK){

        Command_Status->SetValue("Failed to send command. Try again.");

    }else{


            ftStatus = FT_Read(ftHandle, Buffer_to_read, sizeof(Buffer_to_read), &BytesReturned);

            for(unsigned int i = 0; i < 7; i++){

                    Buffer_to_read_integer[i] = Buffer_to_read[i];

                    sprintf(BUFFER_CHAR, "%02X", Buffer_to_read_integer[i]);

                    RSP_GRID->SetCellValue(0, i, BUFFER_CHAR);

                }

          if(Buffer_to_read[0] == 0xEC){

                RSP_EXT_LABEL->SetValue("ERROR.");

            }else{

            if(Buffer_to_read[3] > 0x00){

                RSP_EXT_LABEL->Clear();

                 for(unsigned int i = (BytesReturned - unsigned(Buffer_to_read[3])); i < BytesReturned; i++){

                    Buffer_to_read_integer[i] = Buffer_to_read[i];

                    sprintf(BUFFER_CHAR, "%02X", Buffer_to_read_integer[i]);


                    RSP_EXT_LABEL->AppendText(" ");
                    RSP_EXT_LABEL->AppendText(BUFFER_CHAR);

                }

            }

            }

        }
    }
 ///-----------------------------------------------------------------------------------------------------------
    }else{

    CMD_EXT_LABEL->Clear();
    ACKNOWLEDGE_GRID->ClearGrid();

    ftStatus = FT_Write(ftHandle, Buffer, 7, &BytesWritten);

    if(ftStatus != FT_OK){

        Command_Status->SetValue("Failed to send command. Try again.");

    }else{

            Command_Status->SetValue("Command successfully sent.");

            ftStatus = FT_Read(ftHandle, Buffer_to_read, sizeof(Buffer_to_read), &BytesReturned);

            for(unsigned int i = 0; i < (BytesReturned - unsigned(Buffer_to_read[3])); i++){

                Buffer_to_read_integer[i] = Buffer[i];

                sprintf(BUFFER_CHAR, "%02X", Buffer_to_read_integer[i]);

                CMD_GRID->SetCellValue(0, i, BUFFER_CHAR);

            }

            for(unsigned int i = 0; i < 7; i++){

                Buffer_to_read_integer[i] = Buffer_to_read[i];

                sprintf(BUFFER_CHAR, "%02X", Buffer_to_read_integer[i]);

                RSP_GRID->SetCellValue(0, i, BUFFER_CHAR);

            }

            if(Buffer_to_read[0] == 0xEC){

                RSP_EXT_LABEL->SetValue("ERROR.");

            }else{

            if(Buffer_to_read[3] > 0x00){

                RSP_EXT_LABEL->Clear();

                for(unsigned int i = (BytesReturned - unsigned(Buffer_to_read[3])); i < BytesReturned; i++){

                    Buffer_to_read_integer[i] = Buffer_to_read[i];

                    sprintf(BUFFER_CHAR, "%02X", Buffer_to_read_integer[i]);


                    RSP_EXT_LABEL->AppendText(" ");
                    RSP_EXT_LABEL->AppendText(BUFFER_CHAR);

                }

            }else{

                RSP_EXT_LABEL->Clear();

            }

            }
    }

    }

    CMD_CODE = "";
    CMD_EXT_LENGTH = "";
    CMD_PAR0 = "";
    CMD_PAR1 = "";
    CMD_EXT = "";

    CMD_CODE_Str = "";
    CMD_EXT_LENGTH_Str = "";
    CMD_PAR0_Str = "";
    CMD_PAR1_Str = "";
    CMD_EXT_Str = "";
}

uint32_t ConvertStringToInt(string str, int base) {

    char str1[32];
    char *ptr;

    strcpy(str1, str.c_str());

    return strtol(str1, &ptr, base);

}
void ufr_com_protocol_sending_commandFrame::OnbClearClick(wxCommandEvent& event)
{
    Connect_Status->Clear();

    Command_Status->Clear();

    CMD_CODE_LABEL->Clear();

    CMD_EXT_LABEL->Clear();

    CMD_EXT_LENGTH_LABEL->Clear();

    CMD_PAR0_LABEL->Clear();

    CMD_PAR1_LABEL->Clear();

    CMD_GRID->ClearGrid();

    RSP_GRID->ClearGrid();

    RSP_EXT_LABEL->Clear();

    ACKNOWLEDGE_GRID->ClearGrid();

    Connect_Status->SetBackgroundColour("white");

    Function_Label->Clear();

}

void ConvertStringToIntArray(string str, uint8_t *array, int base) {

    switch (base){
        case 16:
            for (unsigned int i = 0; i < str.length() / 2; i++) {

                string part = str.substr(i * 2, 2);

                array[i] = (uint8_t)ConvertStringToInt(part, 16);


            }
            break;

    }

}
