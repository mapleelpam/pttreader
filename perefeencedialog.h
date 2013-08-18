#ifndef PEREFEENCEDIALOG_H
#define PEREFEENCEDIALOG_H

#include <QDialog>

namespace Ui {
class PerefeenceDialog;
}

class PerefeenceDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PerefeenceDialog(QWidget *parent = 0);
    ~PerefeenceDialog();

public slots:
    virtual void accept ();
private slots:
    void on_pb_general_font_clicked();
    void on_pb_frontcolor_clicked();
    void on_pb_backcolor_clicked();

private:
    void initialSettings();
    Ui::PerefeenceDialog *ui;

    QFont m_general_font;
    QColor m_background_color;
    QColor m_foreground_color;
};

#endif // PEREFEENCEDIALOG_H
