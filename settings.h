#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QFont>
#include <QSettings>

const static QString AppKeyBoardDir = QLatin1String("AppKeyBoardDir");
const static QString AppkeyFont = QLatin1String("AppKeyFont");
const static QString AppKeyFrontgroundColor = QLatin1String("AppKeyFrontgroundColor");
const static QString AppKeyBackgroundColor = QLatin1String("AppKeyBackgroundColor");
const static QString AppKeyTextMargin = QLatin1String("AppKeyTextMargin");

struct GlobalSettings
{
    static QFont systemFont()
    {
        QFont font;
        return systemFont(font);
    }

    static QFont systemFont( const QFont & defaultFont )
    {
        QSettings settings;
        QFont font;
        font.fromString(settings.value(AppkeyFont,defaultFont.toString()).toString());

        return font;
    }
    static void saveSsytemFont( const QFont & font )
    {
        QSettings settings;
        settings.setValue( AppkeyFont, font.toString() );
    }

    static QColor MenuForegroundColor()
    {
        QSettings settings;
        return settings.value( AppKeyFrontgroundColor, Qt::white).toString();
    }
    static void saveMenuForegroundColor( const QColor& c)
    {
        QSettings settings;
        settings.setValue( AppKeyFrontgroundColor, c);
    }
    static QColor MenuBackgroundColor()
    {
        QSettings settings;
        return settings.value( AppKeyBackgroundColor, Qt::black).toString();
    }
    static void saveMenuBackgroundColor( const QColor& c)
    {
        QSettings settings;
        settings.setValue( AppKeyBackgroundColor, c);
    }
    static uint MenuTextMargin()
    {
        QSettings settings;
        return settings.value( AppKeyTextMargin, 5).toUInt();
    }
    static void saveMenuTextMargin( uint margin )
    {
        QSettings settings;
        settings.setValue( AppKeyTextMargin, margin);
    }

private:


};

#endif // SETTINGS_H
