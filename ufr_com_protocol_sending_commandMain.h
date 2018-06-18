/***************************************************************
 * Name:      ufr_com_protocol_sending_commandMain.h
 * Purpose:   Defines Application Frame
 * Author:    Aleksandar (Krstic)
 * Created:   2018-06-18
 * Copyright: Aleksandar (aleksandar.krstic@d-logic.rs)
 * License:
 **************************************************************/

#ifndef UFR_COM_PROTOCOL_SENDING_COMMANDMAIN_H
#define UFR_COM_PROTOCOL_SENDING_COMMANDMAIN_H

//(*Headers(ufr_com_protocol_sending_commandFrame)
#include <wx/button.h>
#include <wx/frame.h>
#include <wx/grid.h>
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/statusbr.h>
#include <wx/textctrl.h>
//*)

class ufr_com_protocol_sending_commandFrame: public wxFrame
{
    public:

        ufr_com_protocol_sending_commandFrame(wxWindow* parent,wxWindowID id = -1);
        virtual ~ufr_com_protocol_sending_commandFrame();

    private:

        //(*Handlers(ufr_com_protocol_sending_commandFrame)
        void OnQuit(wxCommandEvent& event);
        void OnAbout(wxCommandEvent& event);
        void OnbConnectClick(wxCommandEvent& event);
        void OnbDisconnectClick(wxCommandEvent& event);
        void OnbSendClick(wxCommandEvent& event);
        void OnbClearClick(wxCommandEvent& event);
        //*)

        //(*Identifiers(ufr_com_protocol_sending_commandFrame)
        static const long ID_BUTTON1;
        static const long ID_BUTTON2;
        static const long ID_TEXTCTRL1;
        static const long ID_STATICTEXT1;
        static const long ID_STATICTEXT2;
        static const long ID_STATICTEXT3;
        static const long ID_STATICTEXT4;
        static const long ID_STATICTEXT5;
        static const long ID_TEXTCTRL2;
        static const long ID_TEXTCTRL3;
        static const long ID_TEXTCTRL4;
        static const long ID_TEXTCTRL5;
        static const long ID_BUTTON3;
        static const long ID_STATICTEXT9;
        static const long ID_TEXTCTRL7;
        static const long ID_STATICTEXT6;
        static const long ID_TEXTCTRL6;
        static const long ID_BUTTON4;
        static const long ID_STATICTEXT7;
        static const long ID_GRID1;
        static const long ID_STATICTEXT11;
        static const long ID_GRID3;
        static const long ID_STATICTEXT8;
        static const long ID_GRID2;
        static const long ID_STATICTEXT10;
        static const long ID_TEXTCTRL8;
        static const long idMenuQuit;
        static const long idMenuAbout;
        static const long ID_STATUSBAR1;
        //*)

        //(*Declarations(ufr_com_protocol_sending_commandFrame)
        wxButton* bClear;
        wxButton* bConnect;
        wxButton* bDisconnect;
        wxButton* bSend;
        wxGrid* ACKNOWLEDGE_GRID;
        wxGrid* CMD_GRID;
        wxGrid* RSP_GRID;
        wxStaticText* StaticText10;
        wxStaticText* StaticText11;
        wxStaticText* StaticText1;
        wxStaticText* StaticText2;
        wxStaticText* StaticText3;
        wxStaticText* StaticText4;
        wxStaticText* StaticText5;
        wxStaticText* StaticText6;
        wxStaticText* StaticText7;
        wxStaticText* StaticText8;
        wxStaticText* StaticText9;
        wxStatusBar* StatusBar1;
        wxTextCtrl* CMD_CODE_LABEL;
        wxTextCtrl* CMD_EXT_LABEL;
        wxTextCtrl* CMD_EXT_LENGTH_LABEL;
        wxTextCtrl* CMD_PAR0_LABEL;
        wxTextCtrl* CMD_PAR1_LABEL;
        wxTextCtrl* Command_Status;
        wxTextCtrl* Connect_Status;
        wxTextCtrl* RSP_EXT_LABEL;
        //*)

        DECLARE_EVENT_TABLE()
};

#endif // UFR_COM_PROTOCOL_SENDING_COMMANDMAIN_H
