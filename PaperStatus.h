/* 
 * File:   PaperStatus.h
 * Author: atom
 *
 * Created on 3 серпня 2012, 14:54
 */

#ifndef PAPERSTATUS_H
#define	PAPERSTATUS_H

class PaperStatus {
public:
    PaperStatus();
    PaperStatus(const PaperStatus& orig);
    virtual ~PaperStatus();
    void SetNearPaperEnd(bool nearPaperEnd);
    bool IsNearPaperEnd() const;
    void SetPaperEnd(bool paperEnd);
    bool IsPaperEnd() const;
private:
    bool paperEnd;
    bool nearPaperEnd;
};

#endif	/* PAPERSTATUS_H */

