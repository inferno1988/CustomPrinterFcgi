/*
 * File:   PrinterWrapper.h
 * Author: atom
 *
 * Created on 2 серпня 2012, 9:15
 */

#ifndef PRINTERWRAPPER_H
#define PRINTERWRAPPER_H

#include <iconv.h>
#include <termios.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>
#include "PObject.h"

using namespace std;

class PrinterWrapper:
    public PObject
{
    public:
        PrinterWrapper();

        PrinterWrapper(const PrinterWrapper & orig);

        virtual ~PrinterWrapper();

        void closePort();
        void initPort();
        void writeData(char * buf);
        unsigned int getPaperStatus();

    private:
        static const char GET_PRINTER_STATUS[2];
        static const char INIT[2];
        static const char HIQ[3];
        static const char TOTAL_CUT[2];
        int               wait_flag;
        int               fd;
        string            devPath;

        void signalHandlerIO(int status);
};



#endif   /* PRINTERWRAPPER_H */


//~ Formatted by Jindent --- http://www.jindent.com
