/* 
 * File:   FCGIInterface.cpp
 * Author: atom
 * 
 * Created on 24 липня 2012, 15:47
 */

#include <stdlib.h>
#include "FCGIInterface.hpp"


FCGIInterface::FCGIInterface() {

}

FCGIInterface::FCGIInterface(const FCGIInterface& orig) {
}

FCGIInterface::~FCGIInterface() {
}

bool FCGIInterface::response() {
    log("Request recieved", LOG_INFO, OUT_FILE);
    openHtml();
    out << "Welcome to Custom printer manager" << BR;
    out << "<a href=\"?info=1\">About</a>" << BR;
    out << "<a href=\"?debug=1\">Manage debugging</a>" << BR;
    if (environment().checkForGet("info"))
        showInfoPage();
    if (environment().checkForGet("debug")) {
        if (environment().checkForGet("debugState")) {
            string debugKey = environment().findGet("debugState");
            log("Enabling debugging: " + debugKey, LOG_INFO, OUT_FILE);
            setDebuggingEnabled((bool)atoi(debugKey.c_str()));
        }
        showDebugManagePage();
    }
    closeHtml();
    return true;
}

void FCGIInterface::showInfoPage() {
    out << "Custom printer daemon v0.3-rc2 fcgi" << BR;
    out << "Palamarchuk Maxim (gofl@meta.ua) ©2011" << BR;
    out << " ҉" << BR;
}

void FCGIInterface::showDebugManagePage() {
    if (isDebuggingEnabled())
       out << "Current debug state: Enabled" << BR
    else
        out << "Current debug state: Disabled" << BR;

    out << "Current log file is: " << getLogFilename() << BR;
    out << "Current debug level is: " << getCurrentDebugLevel() << BR;
    out << "Current output mode is: " << getCurrentOutMode() << BR;

    if (!isDebuggingEnabled())
        out << "<a href=\"?debug=1&debugState=1\">Enable debugging</a>" << BR
    else
        out << "<a href=\"?debug=1&debugState=0\">Disable debugging</a>" << BR;

    out << "Change debug output mode: ";
    out << "<a href=\"?debug=1&debugOutputMode=1\">To file only</a> | ";
    out << "<a href=\"?debug=1&debugOutputMode=2\">To syslog only</a> | ";
    out << "<a href=\"?debug=1&debugOutputMode=3\">Both</a>" << BR;
    out << "Change debug level: ";
    out << "<a href=\"?debug=1&debugLevel=1\">Informational</a> | ";
    out << "<a href=\"?debug=1&debugLevel=2\">Warning</a> | ";
    out << "<a href=\"?debug=1&debugLevel=3\">Error</a> | ";
    out << "<a href=\"?debug=1&debugLevel=4\">Critical</a>" << BR;
}

void FCGIInterface::openHtml() {
    out << "Content-Type: text/html; charset=utf-8\r\n\r\n";
    out << "<html><meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>";
    out << "<title>Custom printer manager</title>\n";
    out << "<body>";
}

void FCGIInterface::closeHtml() {
    out << "</body></html>\n";
    out.flush();
}

int main(int argc, char *argv[]) {
    try {
        Fastcgipp::Manager<FCGIInterface> fcgi;
        fcgi.handler();
    } catch (std::exception& e) {
        std::cout << e.what();
    }
}