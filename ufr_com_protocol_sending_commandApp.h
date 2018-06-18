/***************************************************************
 * Name:      ufr_com_protocol_sending_commandApp.h
 * Purpose:   Defines Application Class
 * Author:    Aleksandar (Krstic)
 * Created:   2018-06-18
 * Copyright: Aleksandar (aleksandar.krstic@d-logic.rs)
 * License:
 **************************************************************/

#ifndef UFR_COM_PROTOCOL_SENDING_COMMANDAPP_H
#define UFR_COM_PROTOCOL_SENDING_COMMANDAPP_H

#include <wx/app.h>

class ufr_com_protocol_sending_commandApp : public wxApp
{
    public:
        virtual bool OnInit();
};

#endif // UFR_COM_PROTOCOL_SENDING_COMMANDAPP_H
