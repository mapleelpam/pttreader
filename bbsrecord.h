#ifndef BBSRECORD_H
#define BBSRECORD_H

struct BBSRecord{
    QString filename;
    QString title;
    QString date;
    QString owner;

    BBSRecord( QString fn, QString tl, QString da, QString ow)
        : filename(fn)
        , title(tl)
        , date(da)
        , owner(ow)
    {

    }
};

#endif // BBSRECORD_H
