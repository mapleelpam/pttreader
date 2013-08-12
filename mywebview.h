#ifndef MYWEBVIEW_H
#define MYWEBVIEW_H


#define USE_TEXT_BROWSER

#ifdef USE_TEXT_BROWSER
#include <QTextBrowser>
class MyWebView : public QTextBrowser
#else
#include <QWebView>
class MyWebView : public QWebView        
#endif
{
    Q_OBJECT
public:
    MyWebView(QWidget* parent);

#ifdef USE_TEXT_BROWSER
    void load(QUrl url) {   this->setSource(url);   }
#endif
    void loadFontSettings();
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
