#ifndef DIRWIDGET_H
#define DIRWIDGET_H

#include <QWidget>
#include "bbsrecord.h"

class DirWidget : public QWidget
{
    Q_OBJECT
    enum UI_MODE{
        Normal,
        Search
    } ui_mode;

public:
    explicit DirWidget(QWidget *parent = 0);

//    void setBBSRecords(QList<BBSRecord>& records);

    void loadSettings();
    void updateDirPath();

    void searchTitle( const QString& );


signals:
    void sigReadFile( const QString );

public slots:

private slots:
    void cursorUp();
    void cursorDown();
    void cursorPageUp();
    void cursorPageDown();
    void cursorHome();
    void cursorEnd();
    void enterArticle();
    void leaveSearch();

protected:
    void paintEvent(QPaintEvent *event);

private:
    QString getALineString( const BBSRecord& , bool current = false);
    void processDotDir( const QString& fp );
    UI_MODE UiMode();
    void    UiModeSwitchToSearch();
    void    UiModeSwitchToNormal();
private:
    BBSRecordList   m_records;
    BBSRecordList   m_searched_records;
    BBSRecordList   *m_cp; //Current Processing

    QTextCodec *m_big5Codec;
    uint m_text_margin;
    QColor m_foreground_color;
    QColor m_background_color;
//    uint        m_num_of_raws;
};

#endif // DIRWIDGET_H
