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

#define OBSERVER_TO_SCREEN_IN "observerToScreenInches"
#define SCREEN_TO_BG_IN "screenToBackgroundInches"
#define BW_EYES_IN "betweenEyesInches"
#define H_RES_PPI "horizontalResolutionPpi"
#define V_RES_PPI "verticalResolutionPpi"
#define V_RES_EQ_H_RES "verticalResolutionSameAsHorizontalResolutionBool"

#endif // OPTIONS_H
