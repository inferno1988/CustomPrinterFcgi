/* 
 * File:   PaperStatus.cpp
 * Author: atom
 * 
 * Created on 3 серпня 2012, 14:54
 */

#include "PaperStatus.h"

PaperStatus::PaperStatus() {
}

PaperStatus::PaperStatus(const PaperStatus& orig) {
}

PaperStatus::~PaperStatus() {
}

void PaperStatus::SetNearPaperEnd(bool nearPaperEnd) {
    this->nearPaperEnd = nearPaperEnd;
}

bool PaperStatus::IsNearPaperEnd() const {
    return nearPaperEnd;
}

void PaperStatus::SetPaperEnd(bool paperEnd) {
    this->paperEnd = paperEnd;
}

bool PaperStatus::IsPaperEnd() const {
    return paperEnd;
}

