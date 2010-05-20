#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings : public QSettings
{
public:
    enum Unit {
        Inch,
        Pixel
    };
    enum OptionType {
        SpecifiedOption,
        DefaultOption
    };

    Settings(QObject *parent = 0);
    void initialize();
    QVariant option(const QString &key, OptionType type = SpecifiedOption);
    QVariant option(const QString &key, Unit unit, OptionType type = SpecifiedOption);

private:
    QMap<QString, QVariant> m_defaultOptions;
};

#endif // OPTIONS_H
