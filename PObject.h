/* 
 * File:   PObject.h
 * Author: atom
 *
 * Created on 24 липня 2012, 14:32
 */
#include <iostream>
#include <syslog.h>
#include <stdexcept>
#include <fstream>
#include <ctime>
#include <sstream>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#ifndef POBJECT_H
#define	POBJECT_H

#define OUT_FILE        1
#define OUT_SYSLOG      2
#define OUT_BOTH        3

using namespace std;
using namespace boost::property_tree;

class PObject {
public:

    void log(string message, int _pri);
    void log(string message, int _pri, int outMode);
    PObject();
    PObject(const PObject& orig);
    virtual ~PObject();
    int getCurrentDebugLevel();
    string getCurrentDebugLevelText();
    string getDebugLevelText(int debugLevel);
    void setCurrentDebugLevel(int currentDebugLevel);
    int getCurrentOutMode();
    string getCurrentOutModeText();
    void setCurrentOutMode(int currentOutMode);
    bool isDebuggingEnabled();
    void setDebuggingEnabled(bool isDebuggingEnabled);
    string getLogFilename();
private:
    void initXmlConfig();
    string logFilename;
    int currentOutMode;
    int currentDebugLevel;
    bool debuggingEnabled;
    string formatMessage(string message);
    void printMessageToSelectedOutput(string message, int _pri, int outLevel);
    void logToFile(string message);
    void saveXmlConfig();
};

#endif	/* POBJECT_H */

