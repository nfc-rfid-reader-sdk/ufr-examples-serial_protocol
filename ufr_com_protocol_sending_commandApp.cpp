/***************************************************************
 * Name:      ufr_com_protocol_sending_commandApp.cpp
 * Purpose:   Code for Application Class
 * Author:    Aleksandar (Krstic)
 * Created:   2018-06-18
 * Copyright: Aleksandar (aleksandar.krstic@d-logic.rs)
 * License:
 **************************************************************/

#include "ufr_com_protocol_sending_commandApp.h"

//(*AppHeaders
#include "ufr_com_protocol_sending_commandMain.h"
#include <wx/image.h>
//*)

IMPLEMENT_APP(ufr_com_protocol_sending_commandApp);

bool ufr_com_protocol_sending_commandApp::OnInit()
{
    //(*AppInitialize
    bool wxsOK = true;
    wxInitAllImageHandlers();
    if ( wxsOK )
    {
    	ufr_com_protocol_sending_commandFrame* Frame = new ufr_com_protocol_sending_commandFrame(0);
    	Frame->Show();
    	SetTopWindow(Frame);
    }
    //*)
    return wxsOK;

}
