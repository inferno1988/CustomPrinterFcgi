/* 
 * File:   FCGIInterface.cpp
 * Author: atom
 * 
 * Created on 24 липня 2012, 15:47
 */

#include "FCGIManager.hpp"
#include "PrinterWrapper.h"

FCGIInterface::FCGIInterface() {
}

FCGIInterface::FCGIInterface(const FCGIInterface& orig) {
}

FCGIInterface::~FCGIInterface() {
}

bool FCGIInterface::response() {
    using namespace Fastcgipp;

    log("Received request: " + environment().requestUri, LOG_INFO);

    if (environment().checkForGet("command") && environment().checkForGet("callback")) {
        if (environment().findGet("command") == "paper_status") {
            printHtmlHeader();
            PaperStatus paperStatus = parsePaperStatus();
            out << environment().findGet("callback") << "({ \"paperEnd\" : " << 
                    paperStatus.IsPaperEnd() << ", \"nearPaperEnd\" : " << 
                    paperStatus.IsNearPaperEnd() << " })" << endl;
        }
        return true;
    }

    if (environment().checkForGet("command")) {
        if (environment().findGet("command") == "paper_status") {
            printHtmlHeader();
            PaperStatus paperStatus = parsePaperStatus();
            out << "{ \"paperEnd\" : " << paperStatus.IsPaperEnd() << ", \"nearPaperEnd\" : " << 
                    paperStatus.IsNearPaperEnd() << " }" << endl;
        }
        return true;
    }

    openHtml();
    out << "Welcome to Custom printer manager" << BR;
    out << "<a href=\"?info=1\">About</a>" << BR;
    out << "<a href=\"?debug=1\">Debug settings</a>" << BR;

    if (environment().checkForPost("toprint")) {
        if (environment().posts.size()) {
            if (environment().posts.size()) {
                Http::Post<char> post = environment().findPost("toprint");
                log("Printing bill brint", LOG_INFO);
                printBill(post.value);
                log("Printing bill brint finished", LOG_INFO);
            }
        }
    }
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

void FCGIInterface::printBill(string billText) {
    PrinterWrapper * printerWrapper = new PrinterWrapper();
    printerWrapper->initPort();
    printerWrapper->writeData((char *) billText.c_str());
    printerWrapper->closePort();
    delete printerWrapper;
}

PaperStatus FCGIInterface::parsePaperStatus() {
    PaperStatus paperStatus;
    PrinterWrapper * printerWrapper = new PrinterWrapper();
    printerWrapper->initPort();
    unsigned int printerStatus = printerWrapper->getPaperStatus();
    printerWrapper->closePort();
    delete printerWrapper;
    
    switch (printerStatus) {
        case 0:
            paperStatus.SetNearPaperEnd(false);
            paperStatus.SetPaperEnd(false);
            break;
        case 3:
            paperStatus.SetNearPaperEnd(true);
            paperStatus.SetPaperEnd(false);
            break;
        case 12:
            paperStatus.SetNearPaperEnd(false);
            paperStatus.SetPaperEnd(true);
            break;
        case 15:
            paperStatus.SetNearPaperEnd(true);
            paperStatus.SetPaperEnd(true);
            break;
        default:
            paperStatus.SetNearPaperEnd(true);
            paperStatus.SetPaperEnd(true);
    }
    return paperStatus;
}

void FCGIInterface::showInfoPage() {
    out << "Custom printer daemon v0.3-rc2 fcgi" << BR;
    out << "Palamarchuk Maxim (gofl@meta.ua) ©2011" << BR;
    out << "¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤¤" << BR;
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

void FCGIInterface::printHtmlHeader() {
    out << "Content-Type: text/html; charset=utf-8\r\n\r\n";
}

void FCGIInterface::openHtml() {
    printHtmlHeader();
    out << " <!DOCTYPE html>\n<html>\n<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\"/>";
    out << "<title>Custom printer manager</title>\n";
    out << "<body>\n";
}

void FCGIInterface::closeHtml() {
    out << "</body>\n</html>\n";
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