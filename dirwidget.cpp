#include "dirwidget.h"
#include <QPainter>
#include <QtGui>
#include "settings.h"
#include "fileheader.h"



QString str_selected(QLatin1String("  O  "));
QString str_unselected(QLatin1String("     "));
const uint text_margin = 5;


DirWidget::DirWidget(QWidget *parent) :
    QWidget(parent)
  , m_current_records(0)
  ,m_big5Codec(QTextCodec::codecForName("Big5"))
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
    action->setShortcut(QKeySequence(Qt::Key_Right));
    addAction(action);
    connect(action,SIGNAL(triggered()) , this, SLOT(enterArticle()));

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

void DirWidget::setBBSRecords(QList<BBSRecord>& records)
{
    m_records = records;
    m_current_records = m_records.size()-1;
    update();
}


void DirWidget::paintEvent(QPaintEvent *event)
{
    QRect widgetRect = rect();

    QPainter painter(this);
    QFont font = painter.font();
    int rows = ( widgetRect.height() / (font.pointSize()+text_margin) -  2);

//    qDebug() << "rows = "<<rows<<widgetRect.height()<<font.pointSize();

    int m_draw_start = int(m_current_records/rows) * rows;
//    qDebug() << "m_draw_start "<<m_draw_start<<rows<<m_current_records;

    for(  int pos_y = font.pointSize()+text_margin, idx = m_draw_start ; idx < m_draw_start+rows && idx < m_records.size() ; idx ++ ) {
//        qDebug() << idx << m_records[idx].title;
        if(idx == m_current_records) {
            painter.save();
            painter.setBrush(QBrush(Qt::black));
            painter.setPen(Qt::NoPen);
            painter.drawRect( 0, pos_y+text_margin/2, widgetRect.width(), -(font.pointSize()+text_margin) );
            painter.setPen(QPen(Qt::white));
            painter.drawText( 0, pos_y, getALineString(m_records[idx], idx == m_current_records)+ " " );
            painter.restore();
        } else {
            painter.drawText( 0, pos_y, getALineString(m_records[idx], idx == m_current_records)+ " " );
        }
        pos_y += font.pointSize() + text_margin;
    }
}

QString DirWidget::getALineString( BBSRecord record, bool current )
{

    return ((current) ?  str_selected : str_unselected ) +
            record.date + tr("  ") +
            record.owner + tr("  ") +
            record.title;
}

void DirWidget::cursorUp()
{
    m_current_records = (m_current_records-1 < 0 ) ? 0 : m_current_records-1;
    repaint();
}

void DirWidget::cursorDown()
{
    m_current_records = (m_current_records+1 >= m_records.size()) ? m_records.size()-1 : m_current_records+1;
    repaint();
}

void DirWidget::enterArticle()
{
    qDebug() <<" enterArticle ";
    emit sigReadFile( m_records[m_current_records].filename );
}


void DirWidget::processDotDir( const QString& dir )
{

#ifdef WIN32
    QFile file(dir+QLatin1String("\\.DIR"));
#else
    QFile file(dir+QLatin1String("/.DIR"));
#endif
    bool success =file.open(QIODevice::ReadOnly | QIODevice::Text);

    if(success) {
        qDebug() << "success";
    } else {
        qDebug() << "failed to open "<<file.fileName();
    }
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

            m_records.append(BBSRecord(
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
    m_current_records = m_records.size()-1;
}
