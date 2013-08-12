#ifndef MYWEBVIEW_H
#define MYWEBVIEW_H

#include <QWebView>

class MyWebView : public QWebView
{
    Q_OBJECT
public:
    MyWebView(QWidget* parent);

signals:
    void sigLeaveThisView();
private slots:
    void slotLeaveThisView();

    void slotZoomIn();
    void slotZoomOut();
protected:
//    void keyPressEvent ( QKeyEvent * ev );
};

#endif // MYWEBVIEW_H
