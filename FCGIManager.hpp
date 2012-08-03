/*
 * File:   FCGIInterface.hpp
 * Author: atom
 *
 * Created on 24 липня 2012, 15:47
 */

#ifndef FCGIINTERFACE_HPP
#define FCGIINTERFACE_HPP

#include <stdlib.h>
#include <syslog.h>
#include <fastcgi++/request.hpp>
#include <fastcgi++/manager.hpp>
#include <fastcgi++/fcgistream.hpp>
#include <fstream>
#include "PObject.h"
#include "PaperStatus.h"

class FCGIInterface:
    public Fastcgipp:: Request<char>,
    PObject
{
    public:
        bool response();

        FCGIInterface();

        FCGIInterface(const FCGIInterface & orig);

        virtual ~FCGIInterface();

    private:
        void showInfoPage();
        void showDebugManagePage();
        void openHtml();
        void closeHtml();
        void printHtmlHeader();
        void printBill(string billText);
        PaperStatus parsePaperStatus();
#define BR "<br>\n";
};
#endif   /* FCGIINTERFACE_HPP */


//~ Formatted by Jindent --- http://www.jindent.com
