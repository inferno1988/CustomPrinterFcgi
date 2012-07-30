/* 
 * File:   PObject.cpp
 * Author: atom
 * 
 * Created on 24 липня 2012, 14:32
 */

#include "PObject.h"
#include <time.h>

PObject::PObject() {
    initXmlConfig();
    this->currentDebugLevel = getCurrentDebugLevel();
    this->currentOutMode = getCurrentOutMode();
    this->debuggingEnabled = isDebuggingEnabled();
}

PObject::PObject(const PObject& orig) {
}

PObject::~PObject() {
}

void PObject::log(std::string message, int _pri) {
    openlog("PrinterDriver", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_DAEMON);
    printMessageToSelectedOutput(message, _pri, getCurrentOutMode());
    closelog();
}

void PObject::log(std::string message, int _pri, int outMode) {
    openlog("PrinterDriver", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_DAEMON);
    printMessageToSelectedOutput(message, _pri, outMode);
    closelog();
}

void PObject::printMessageToSelectedOutput(std::string message, int _pri, int outLevel) {
    if (outLevel == OUT_FILE)
        logToFile(message);
    if (outLevel == OUT_SYSLOG)
        syslog(_pri, "%s", message.c_str());
    if (outLevel == OUT_BOTH) {
        logToFile(message);
        syslog(_pri, "%s", message.c_str());
    }
}

void PObject::logToFile(string message) {
    string formattedMessage = formatMessage(message);
    ofstream * fileToLog = new ofstream(getLogFilename().c_str(), ios::out | ios::app);
    fileToLog->write(formattedMessage.c_str(), formattedMessage.length());
    fileToLog->close();
    delete fileToLog;
}

string PObject::formatMessage(string message) {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    ostringstream outputString;
    outputString << "[" << 1900 + ltm->tm_year << "/" << 1 + ltm->tm_mon << "/"
            << ltm->tm_mday << " " << ltm->tm_hour << ":" << ltm->tm_min << ":"
            << ltm->tm_sec << "]: " << message << endl;
    message = outputString.str();
    return message;
}

string PObject::getLogFilename() {
    return logFilename;
}

void PObject::initXmlConfig() {
    ptree propertyTree;
    try {
        read_xml("printer.xml", propertyTree);
        logFilename = propertyTree.get<string>("debug.filename");
        currentDebugLevel = propertyTree.get<int>("debug.level");
        currentOutMode = propertyTree.get<int>("debug.out");
        debuggingEnabled = propertyTree.get<bool>("debug.enabled");
    } catch (xml_parser_error e) {
        log(string(e.what()), LOG_CRIT, OUT_SYSLOG);
        exit(EXIT_FAILURE);
    } catch (ptree_bad_data e) {
        log(string(e.what()), LOG_CRIT, OUT_SYSLOG);
        exit(EXIT_FAILURE);
    }

}

void PObject::saveXmlConfig() {
    try {
        ptree propertyTree;
        propertyTree.put("debug.level", currentDebugLevel);
        propertyTree.put("debug.out", currentOutMode);
        propertyTree.put("debug.enabled", debuggingEnabled);
        propertyTree.put("debug.filename", "/tmp/printer.log");
        write_xml("printer.xml", propertyTree);
    } catch (xml_parser_error e) {
        log(string(e.what()), LOG_CRIT, OUT_SYSLOG);
        exit(EXIT_FAILURE);
    }
}

int PObject::getCurrentDebugLevel() {
    return currentDebugLevel;
}

void PObject::setCurrentDebugLevel(int currentDebugLevel) {
    this->currentDebugLevel = currentDebugLevel;
    saveXmlConfig();
}

int PObject::getCurrentOutMode() {
    return currentOutMode;
}

void PObject::setCurrentOutMode(int currentOutMode) {
    this->currentOutMode = currentOutMode;
    saveXmlConfig();
}

bool PObject::isDebuggingEnabled() {
    return debuggingEnabled;
}

void PObject::setDebuggingEnabled(bool isDebuggingEnabled) {
    this->debuggingEnabled = isDebuggingEnabled;
    saveXmlConfig();
}

string PObject::getCurrentOutModeText() {
    int currentOutMode = getCurrentOutMode();
    if (currentOutMode == OUT_FILE)
        return "File";
    if (currentOutMode == OUT_SYSLOG)
        return "Syslog";
    if (currentOutMode == OUT_BOTH)
        return "Both";
}

string PObject::getCurrentDebugLevelText() {
    int currentDebugLevel = getCurrentDebugLevel();
    if (currentDebugLevel == LOG_INFO)
        return "Informational";
    if (currentDebugLevel == LOG_WARNING)
        return "Warning";
    if (currentDebugLevel == LOG_ERR)
        return "Error";
    if (currentDebugLevel == LOG_CRIT)
        return "Critical";
    return "Wrong debug level";     
}
