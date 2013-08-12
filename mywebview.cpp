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
}

void MyWebView::slotLeaveThisView()
{
    qDebug() << " left ";
    emit sigLeaveThisView();
}



//void MyWebView::keyPressEvent ( QKeyEvent * ev )
//{
//}
