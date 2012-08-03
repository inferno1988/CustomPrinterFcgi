/*
 * File:   PrinterWrapper.cpp
 * Author: atom
 *
 * Created on 2 серпня 2012, 9:15
 */

#include "PrinterWrapper.h"

PrinterWrapper::PrinterWrapper() {
    this->devPath = getDeviceFileDescriptor();
}

PrinterWrapper::PrinterWrapper(const PrinterWrapper & orig) {
}

PrinterWrapper::~PrinterWrapper() {
}

void PrinterWrapper::closePort() {
    log("Closing printer port", LOG_INFO);
    close(fd);
    log("Printer port has closed", LOG_INFO);
}

void PrinterWrapper::initPort() {
    struct termios oldtio, newtio;
    struct sigaction saio;    // definition of signal action
    log("Opening printer port", LOG_INFO);

    fd = open(devPath.c_str(), O_RDWR | O_NOCTTY | O_NONBLOCK);

    if (fd < 0) {
        string errorText(strerror(errno));
        log("Can't open printer port: " + devPath + " " + errorText, LOG_CRIT, OUT_BOTH);
        exit(0);
    }

    log("Printer port has opened", LOG_INFO);

    sigemptyset(&saio.sa_mask);    // saio.sa_mask = 0;

    saio.sa_flags    = 0;
    saio.sa_restorer = NULL;

    sigaction(SIGIO, &saio, NULL);

    // allow the process to receive SIGIO
    fcntl(fd, F_SETOWN, getpid());

    // Make the file descriptor asynchronous (the manual page says only
    // O_APPEND and O_NONBLOCK, will work with F_SETFL...)
    fcntl(fd, F_SETFL, FASYNC);
    tcgetattr(fd, &oldtio);

    newtio.c_cflag     = B9600 | CRTSCTS | CS8 | 0 | 0 | 0 | CLOCAL | CREAD;
    newtio.c_iflag     = IGNPAR;
    newtio.c_oflag     = 0;
    newtio.c_lflag     = 0;    // ICANON;
    newtio.c_cc[VMIN]  = 1;
    newtio.c_cc[VTIME] = 0;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &newtio);
}

string to_string(int val) {
 char buff[32];
 sprintf(buff,"%d",val);
 return string(buff);
}

void PrinterWrapper::writeData(char * buf) {
    iconv_t cd;
    size_t  k, f, t;

    string iconv_from = "CP1251//IGNORE";
    string iconv_to   = "UTF-8";

    char   buf1[MAXANSWERSIZE];
    char * out = buf1;
    char * msg = buf;

    f  = strlen(buf);
    t  = sizeof(buf1);
    log("Recoding from UTF-8 to CP1251", LOG_INFO);
    cd = iconv_open(iconv_from.c_str(), iconv_to.c_str());
    k  = iconv(cd, &msg, &f, &out, &t);
    log("Recoded from UFT-8 to CP1251", LOG_INFO);

    log("Sending INIT command to printer", LOG_INFO);
    write(fd, INIT, sizeof(INIT));
    log("Sending HIQ command to printer", LOG_INFO);
    write(fd, HIQ, sizeof(HIQ));
    log("Sending data to printer", LOG_INFO);
    int bc = write(fd, buf1, strlen(buf1));
    log("Bytes written to printer: " + to_string(bc), LOG_INFO);
    log("Sending TOTAL_CUT command to printer", LOG_INFO);
    write(fd, TOTAL_CUT, sizeof(TOTAL_CUT));

    int i;

    for (i = 0; i < MAXANSWERSIZE; i++) {
        buf1[i] = 0;
    }
    iconv_close(cd);
}

unsigned int PrinterWrapper::getPaperStatus() {
    unsigned char printerStatus = 0x00;
    log("Sending INIT command to printer", LOG_INFO);
    write(fd, INIT, strlen(INIT));
    log("Sending GET_PRINTER_STATUS command to printer", LOG_INFO);
    write(fd, GET_PRINTER_STATUS, strlen(GET_PRINTER_STATUS));
    read(fd, &printerStatus, 1);

    return (unsigned int) printerStatus;
}

const char PrinterWrapper::GET_PRINTER_STATUS[2] = { 0x1B, 0x76 };          // get printer status
const char PrinterWrapper::INIT[2]               = { 0x1B, 0X40 };          // get printer status
const char PrinterWrapper::HIQ[3]                = { 0x1B, 0x21, 0x01 };    // set hi quality mode
const char PrinterWrapper::TOTAL_CUT[2]          = { 0x1B, 0x69 };          // cut paper

//~ Formatted by Jindent --- http://www.jindent.com
