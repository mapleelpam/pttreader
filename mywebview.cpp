#include "mywebview.h"
#include <QtGui>

MyWebView::MyWebView(QWidget* parent)
    : QWebView(parent)
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
}

void MyWebView::slotLeaveThisView()
{
    qDebug() << " left ";
    emit sigLeaveThisView();
}

void MyWebView::slotZoomIn()
{
    setZoomFactor( zoomFactor()*1.1 );
}

void MyWebView::slotZoomOut()
{
    setZoomFactor( zoomFactor()/1.1 );

}
//void MyWebView::keyPressEvent ( QKeyEvent * ev )
//{
//}
