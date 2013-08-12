#include "mywebview.h"
#include <QtGui>
#include "settings.h"

MyWebView::MyWebView(QWidget* parent)
#ifdef USE_TEXT_BROWSER
    : QTextBrowser(parent)
#else
    : QWebView(parent)
#endif
{
    qDebug() << "in my web vew";
    QAction* action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::Key_Left));
    addAction(action);
    connect(action,SIGNAL(triggered()) , this, SLOT(slotLeaveThisView()));


    action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::Key_ZoomIn));
    addAction(action);
    connect(action,SIGNAL(triggered()) , this, SLOT(slotZoomIn()));


    action = new QAction(this);
    action->setShortcut(QKeySequence(Qt::Key_ZoomOut));
    addAction(action);
    connect(action,SIGNAL(triggered()) , this, SLOT(slotZoomOut()));

    this->setText("");
    loadFontSettings();
}

void MyWebView::loadFontSettings()
{
    QSettings settings;
    QFont font;
    font.fromString(settings.value(AppkeyFont,this->font().toString()).toString());

    setFont(font);
}

void MyWebView::slotLeaveThisView()
{
    qDebug() << " left ";
    emit sigLeaveThisView();
}

void MyWebView::slotZoomIn()
{
#ifndef USE_TEXT_BROWSER
    setZoomFactor( zoomFactor()*1.1 );
#endif
}

void MyWebView::slotZoomOut()
{
#ifndef USE_TEXT_BROWSER
    setZoomFactor( zoomFactor()/1.1 );
#endif

}
//void MyWebView::keyPressEvent ( QKeyEvent * ev )
//{
//}
