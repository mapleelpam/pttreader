#include "perefeencedialog.h"
#include "ui_perefeencedialog.h"
#include <QtGui>
#include "settings.h"

PerefeenceDialog::PerefeenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PerefeenceDialog)
{
    ui->setupUi(this);
    initialSettings();
}

void PerefeenceDialog::initialSettings()
{
    // General
    m_general_font = GlobalSettings::systemFont();
    ui->label_system_font->setFont(m_general_font);

    // Menu
    QPalette palette = ui->pb_backcolor->palette();
    m_background_color = GlobalSettings::MenuBackgroundColor();
    palette.setColor(QPalette::ButtonText, m_background_color);
    ui->pb_backcolor->setPalette( palette );

    palette = ui->pb_frontcolor->palette();
    m_foreground_color = GlobalSettings::MenuForegroundColor();
    palette.setColor(QPalette::ButtonText, m_foreground_color);
    ui->pb_frontcolor->setPalette( palette );

    uint text_margin = GlobalSettings::MenuTextMargin();
    ui->sb_text_margin->setValue( text_margin );
}

PerefeenceDialog::~PerefeenceDialog()
{
    delete ui;
}

void PerefeenceDialog::accept ()
{
    GlobalSettings::saveMenuBackgroundColor(m_background_color);
    GlobalSettings::saveMenuForegroundColor(m_foreground_color);
    GlobalSettings::saveMenuTextMargin(ui->sb_text_margin->value());
    GlobalSettings::saveSsytemFont(m_general_font);

    QDialog::accept();
}

void PerefeenceDialog::on_pb_general_font_clicked()
{
    bool ok;
    QFont font = m_general_font;
    font = QFontDialog::getFont(&ok,font, this);
    if( ok ) {
        ui->label_system_font->setFont(font);
        m_general_font = font;
    }
}

void PerefeenceDialog::on_pb_frontcolor_clicked()
{
    QColor color = QColorDialog::getColor( m_background_color );

    QPalette palette = ui->pb_frontcolor->palette();
    palette.setColor(QPalette::ButtonText, color);
    ui->pb_frontcolor->setPalette( palette );

    m_background_color = color;
}

void PerefeenceDialog::on_pb_backcolor_clicked()
{
    QColor color = QColorDialog::getColor( m_foreground_color );

    QPalette palette = ui->pb_backcolor->palette();
    palette.setColor(QPalette::ButtonText, color);
    ui->pb_backcolor->setPalette( palette );

    m_foreground_color = color;

}
