#ifndef DIRWIDGET_H
#define DIRWIDGET_H

#include <QWidget>
#include "bbsrecord.h"

class DirWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DirWidget(QWidget *parent = 0);
    
    void setBBSRecords(QList<BBSRecord>& records);

    void loadFontSettings();
    void updateDirPath();

signals:
    void sigReadFile( const QString );
    
public slots:

private slots:
    void cursorUp();
    void cursorDown();
    void enterArticle();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString getALineString( BBSRecord, bool current = false);
    void processDotDir( const QString& fp );

private:
    QList<BBSRecord>    m_records;
    QTextCodec *m_big5Codec;
    int                m_current_records;
};

#endif // DIRWIDGET_H
