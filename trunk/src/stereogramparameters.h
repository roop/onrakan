#ifndef STEREOGRAM_PARAMETERS_H
#define STEREOGRAM_PARAMETERS_H

#include <QMap>
#include <QString>
#include <QVariant>

#define OBSERVER_TO_SCREEN_IN "observerToScreenInches"
#define SCREEN_TO_BG_IN "screenToBackgroundInches"
#define BW_EYES_IN "betweenEyesInches"
#define H_RES_PPI "horizontalResolutionPpi"

class QSettings;

class StereogramParameters
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

    StereogramParameters();
    void initialize();
    QStringList parameterNames() const;
    void setValue(const QString &key, QVariant value);
    QVariant value(const QString &key, OptionType type = SpecifiedOption) const;
    QVariant value(const QString &key, Unit unit, OptionType type = SpecifiedOption) const;

    inline int observerToScreenPx() const { return value(OBSERVER_TO_SCREEN_IN, Pixel).toInt(); }
    inline int screenToBackgroundPx() const { return value(SCREEN_TO_BG_IN, Pixel).toInt(); }
    inline int betweenEyesPx() const { return value(BW_EYES_IN, Pixel).toInt(); }

    bool operator==(const StereogramParameters &other) { return (m_parameters == other.m_parameters); }
    bool operator!=(const StereogramParameters &other) { return (m_parameters != other.m_parameters); }

    static StereogramParameters fromSettings(const QSettings *settings);
    void saveToSettings(QSettings *settings);

private:
    QMap<QString, QVariant> m_parameters;
    QMap<QString, QVariant> m_defaultParameters;
};

#endif // STEREOGRAM_PARAMETERS_H
