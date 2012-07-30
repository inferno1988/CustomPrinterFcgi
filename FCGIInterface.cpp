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
    openHtml();
    out << "Welcome to Custom printer manager" << BR;
    out << "<a href=\"?info=1\">About</a>" << BR;
    out << "<a href=\"?debug=1\">Manage debugging</a>" << BR;
    if (environment().checkForGet("info"))
        showInfoPage();
    if (environment().checkForGet("debug")) {
        if (environment().checkForGet("debugState")) {
            string debugKey = environment().findGet("debugState");
            setDebuggingEnabled((bool)atoi(debugKey.c_str()));
        }
        if (environment().checkForGet("debugOutputMode")) {
            string debugOutMode = environment().findGet("debugOutputMode");
            setCurrentOutMode(atoi(debugOutMode.c_str()));
        }
        if (environment().checkForGet("debugLevel")) {
            string debugLevel = environment().findGet("debugLevel");
            setCurrentDebugLevel(atoi(debugLevel.c_str()));
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
    out << "Current debug level is: " << getCurrentDebugLevelText() << BR;
    out << "Current output mode is: " << getCurrentOutModeText() << BR;

    if (!isDebuggingEnabled())
        out << "<a href=\"?debug=1&debugState=1\">Enable debugging</a>" << BR
    else
        out << "<a href=\"?debug=1&debugState=0\">Disable debugging</a>" << BR;

    out << "Change debug output mode: ";
    out << "<a href=\"?debug=1&debugOutputMode=" << OUT_FILE << "\">To file only</a> | ";
    out << "<a href=\"?debug=1&debugOutputMode=" << OUT_SYSLOG << "\">To syslog only</a> | ";
    out << "<a href=\"?debug=1&debugOutputMode=" << OUT_BOTH << "\">Both</a>" << BR;
    out << "Change debug level: ";
    out << "<a href=\"?debug=1&debugLevel=" << LOG_INFO << "\">Informational</a> | ";
    out << "<a href=\"?debug=1&debugLevel=" << LOG_WARNING << "\">Warning</a> | ";
    out << "<a href=\"?debug=1&debugLevel=" << LOG_ERR << "\">Error</a> | ";
    out << "<a href=\"?debug=1&debugLevel=" << LOG_CRIT << "\">Critical</a>" << BR;
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