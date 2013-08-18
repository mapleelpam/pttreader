#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextCodec>
#include <QTextDocument>
#include "dirwidget.h"

namespace Ui {
class MainWindow;
}

class MyWebView;
class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
public slots:
//    void processDotDir( const QString& fp );
    void slotSwitchToOutline();
    void on_actionFont_triggered();
    void on_actionOpenPath_triggered();
    void on_actionSearchTitle_triggered();
    void on_actionPerefeence_triggered();
private slots:
    void slotReadFile(const QString);
private:
    Ui::MainWindow *ui;
//    QList<BBSRecord> records;
    DirWidget*  m_dirWidget;
    MyWebView*   m_webview;
//    QTextDocument* m_article;
};

#endif // MAINWINDOW_H
