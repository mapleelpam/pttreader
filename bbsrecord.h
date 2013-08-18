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


struct BBSRecordList{
    QList<BBSRecord>    records;
    int                 current_records;
    int                 num_of_raws;

    void clear()
    {
        num_of_raws = 1;
        records.clear();
        current_records = 0;
    }

    void gotoLastItem()
    {
        current_records = records.size() - 1;
    }

    void cursorUp()
    {
        current_records = (current_records-1 < 0 ) ? 0 : current_records-1;
    }

    void cursorDown()
    {
        current_records = (current_records+1 >= records.size()) ? records.size()-1 : current_records+1;
    }

    void cursorPageUp()
    {
        current_records = (current_records-num_of_raws < 0 ) ? 0 : current_records-num_of_raws;
    }

    void cursorPageDown()
    {
        current_records = (current_records+num_of_raws >= records.size()) ? records.size()-1 : current_records+num_of_raws;
    }

    void cursorHome()
    {
        current_records = 0;
    }

    void cursorEnd()
    {
        current_records = records.size() - 1;
    }
};

#endif // BBSRECORD_H
