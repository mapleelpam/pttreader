#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "bbs2html.h"
#include "mywebview.h"
#include "settings.h"

#include <QFile>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_dirWidget = new DirWidget(this);
    ui->stackedWidget->addWidget(m_dirWidget);

//    m_article = new QTextDocument(this);
//    ui->stackedWidget->addWidget(m_article);

    m_webview = new MyWebView(ui->stackedWidget);
    ui->stackedWidget->addWidget(m_webview);

//    processDotDir();

//    m_dirWidget->setBBSRecords(records);
    QSettings settings;
    if(settings.value(AppKeyBoardDir).toString() == QLatin1String("")) {
        on_actionOpenPath_triggered();
    } else {
        m_dirWidget->updateDirPath();
    }
    ui->stackedWidget->setCurrentWidget(m_dirWidget);

    connect( m_dirWidget, SIGNAL(sigReadFile(const QString)) , SLOT(slotReadFile(const QString)) );
    connect( m_webview, SIGNAL(sigLeaveThisView()), this, SLOT(slotSwitchToOutline()) );

    connect( ui->actionZoomIn, SIGNAL(triggered()), m_webview, SLOT(slotZoomIn()) );
    connect( ui->actionZoomOut, SIGNAL(triggered()), m_webview, SLOT(slotZoomOut()) );
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString get_repeat_string( QString space, uint times )
{
    QString ret;
    for( int idx = 0 ; idx < times ; idx ++ ) {
        ret = ret+space;
    }
    return ret;
}

void MainWindow::slotReadFile(const QString filename)
{

    QString full_path = QLatin1String("/Users/maple/Downloads/command/")+filename;
    QString full_html_path = full_path+tr(".html");
    QString full_html;

    BBS2Html::ann2html( full_html, full_path.toLocal8Bit().constData(), "hello");
//    m_webview->load(QUrl(full_html_path));

    m_webview->setHtml( full_html );
    m_webview->show();
    ui->stackedWidget->setCurrentWidget (m_webview);
}

void MainWindow::slotSwitchToOutline()
{
    ui->stackedWidget->setCurrentWidget(m_dirWidget);
}

void MainWindow::on_actionFont_triggered()
{
    QSettings settings;
    bool ok;
    QFont font;

    font.fromString(settings.value(AppkeyFont,this->font().toString()).toString());

//    qDebug() << " font size " << font.pointSize() << font.pixelSize();
    font = QFontDialog::getFont(&ok, this);
//    qDebug() << "  -> " << settings.value(AppkeyFont,this->font().toString()).toString() << font.pointSize();

    settings.setValue(AppkeyFont, font.toString());

    if( ok ) {
//        m_dirWidget->setFont(font);
        m_dirWidget->loadFontSettings();
        m_webview->loadFontSettings();
    }
}

void MainWindow::on_actionOpenPath_triggered()
{
    QSettings settings;
    QString dir = settings.value(AppKeyBoardDir).toString();
    dir = QFileDialog::getExistingDirectory(this,tr("BBS File Path"));
//    qDebug() << "dir "<<dir;
    if( dir != "" ) {
        settings.setValue(AppKeyBoardDir, dir);
        m_dirWidget->updateDirPath();
    }
}

void MainWindow::on_actionSearchTitle_triggered()
{
    bool ok = false;
    QString title = QInputDialog::getText(this, tr("Search - "),
                                         tr("Include  - "), QLineEdit::Normal,
                                          tr(""), &ok);
    if (ok && !title.isEmpty())
        m_dirWidget->searchTitle(title);
}
