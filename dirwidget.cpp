#include "dirwidget.h"
#include <QPainter>
#include <QtGui>
#include "settings.h"
#include "fileheader.h"



QString str_selected(QLatin1String("  O  "));
QString str_unselected(QLatin1String("     "));
const uint text_margin = 3;


DirWidget::DirWidget(QWidget *parent) :
    QWidget(parent)
  , m_big5Codec(QTextCodec::codecForName("Big5"))
{
    QAction* action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::Key_Up));
    addAction(action);
    connect(action,SIGNAL(triggered()) , this, SLOT(cursorUp()));

    action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::Key_Down));
    addAction(action);
    connect(action,SIGNAL(triggered()) , this, SLOT(cursorDown()));

    action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::Key_PageDown));
    addAction(action);
    connect(action,SIGNAL(triggered()) , this, SLOT(cursorPageDown()));

    action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::Key_PageUp));
    addAction(action);
    connect(action,SIGNAL(triggered()) , this, SLOT(cursorPageUp()));

    action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::Key_Right));
    addAction(action);
    connect(action,SIGNAL(triggered()) , this, SLOT(enterArticle()));


    action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::Key_Left));
    addAction(action);
    connect(action,SIGNAL(triggered()) , this, SLOT(leaveSearch()));


    UiModeSwitchToNormal();

    setFocus();

    loadFontSettings();
}

void DirWidget::loadFontSettings()
{
    QSettings settings;
    QFont font;
    font.fromString(settings.value(AppkeyFont,this->font().toString()).toString());

    setFont(font);
}

/*
void DirWidget::setBBSRecords(QList<BBSRecord>& records)
{
    m_records.records = records;
    m_records.gotoLastItem();
    update();
}*/


void DirWidget::paintEvent(QPaintEvent *event)
{
    QRect widgetRect = rect();

    QPainter painter(this);
    QFont font = painter.font();
    int num_of_raws = ( widgetRect.height() / (font.pointSize()+text_margin) -  2);
    m_cp->num_of_raws = num_of_raws;

//    qDebug() << "rows = "<<rows<<widgetRect.height()<<font.pointSize();

    int draw_start = int(m_cp->current_records/num_of_raws) * num_of_raws;
//    qDebug() << "m_draw_start "<<m_draw_start<<rows<<m_current_records;

    for(  int pos_y = font.pointSize()+text_margin, idx = draw_start
          ; idx < draw_start+num_of_raws && idx < m_cp->records.size() ; idx ++ ) {
//        qDebug() << idx << m_records[idx].title;
        if(idx == m_cp->current_records) {
            painter.save();
            painter.setBrush(QBrush(Qt::black));
            painter.setPen(QPen(Qt::white));
            painter.drawRect( 0, pos_y+text_margin, widgetRect.width(), -(font.pointSize()+text_margin) );
            painter.drawText( 0, pos_y, getALineString(m_cp->records[idx], idx == m_cp->current_records)+ " " );
            painter.restore();
        } else {
            painter.drawText( 0, pos_y, getALineString(m_cp->records[idx], idx == m_cp->current_records)+ " " );
        }
        pos_y += font.pointSize() + text_margin;
    }
}

QString DirWidget::getALineString( const BBSRecord& record, bool current )
{

    return ((current) ?  str_selected : str_unselected ) +
            record.date + tr("  ") +
            record.owner + tr("  ") +
            record.title;
}

void DirWidget::cursorUp()
{
    m_cp->cursorUp();
    repaint();
}

void DirWidget::cursorDown()
{
    m_cp->cursorDown();
    repaint();
}


void DirWidget::cursorPageUp()
{
    m_cp->cursorPageUp();
    repaint();
}

void DirWidget::cursorPageDown()
{
    m_cp->cursorPageDown();
    repaint();
}

void DirWidget::enterArticle()
{
    qDebug() <<" enterArticle ";
    emit sigReadFile( m_records.records[m_records.current_records].filename );
}

void DirWidget::leaveSearch()
{
    UiModeSwitchToNormal();
}

void DirWidget::processDotDir( const QString& dir )
{

#ifdef WIN32
    QFile file(dir+QLatin1String("\.DIR"));
#else
    QFile file(dir+QLatin1String("/.DIR"));
#endif
    file.open(QIODevice::ReadOnly | QIODevice::Text);

//    qDebug() << "fileheader size "<<sizeof(fileheader_t)<<" filesize = "<<file.size()<<dir;

    m_records.clear();
    while(1) {
        QByteArray ba = file.read(sizeof(fileheader_t));
        if( ba.size() == sizeof(fileheader_t )) {
            fileheader_t* fh = (fileheader_t*)(void*)(ba.constData());

            QString s_owner = m_big5Codec->toUnicode(fh->owner);
            QString s_title = m_big5Codec->toUnicode(fh->title);
            QString s_filename = m_big5Codec->toUnicode(fh->filename);
            QString s_date = QLatin1String(fh->date);

            m_records.records.append(BBSRecord(
                               s_filename,
                               s_title,
                               s_date,
                               s_owner
                               ) );



            if(fh->pad3[2] == 'M') {
                file.seek(file.pos()-1);
            }

        } else {
            qDebug() << "false"<<ba.size();
            break;
        }

    }

    file.close();
}

void DirWidget::updateDirPath()
{
    QSettings settings;
    processDotDir(settings.value(AppKeyBoardDir).toString());
    repaint();
    m_records.gotoLastItem();
}

void DirWidget::searchTitle( const QString& title)
{
//    QList<BBSRecord> after_search;
    m_searched_records.clear();

    foreach( const BBSRecord record, m_records.records ){
        if(record.title.contains(title)) {
            m_searched_records.records << record;
            qDebug() << " -> "<<record.title;
        }
    }

    if( m_searched_records.records.size() == 0 ) {
        m_searched_records.records << BBSRecord(tr(""),tr("nothing"),tr(""),tr(""));
    }
    m_searched_records.gotoLastItem();
    UiModeSwitchToSearch();
}

DirWidget::UI_MODE DirWidget::UiMode()
{
    return ui_mode;
}

void    DirWidget::UiModeSwitchToSearch()
{
    ui_mode = Search;
    m_cp = &m_searched_records;
    repaint();
}

void    DirWidget::UiModeSwitchToNormal()
{
    ui_mode = Normal;
    m_cp = &m_records;
    repaint();
}
