#include <QSettings>
#include <QStringList>
#include "stereogramparameters.h"

StereogramParameters::StereogramParameters()
{
    initialize();
}

void StereogramParameters::initialize()
{
    m_defaultParameters[OBSERVER_TO_SCREEN_IN] = 15;
    m_defaultParameters[SCREEN_TO_BG_IN] = 15;
    m_defaultParameters[BW_EYES_IN] = 2.5;
    m_defaultParameters[H_RES_PPI] = 72;
}

QStringList StereogramParameters::parameterNames() const
{
    return m_defaultParameters.keys();
}

void StereogramParameters::setValue(const QString &key, QVariant value)
{
    m_parameters[key] = value;
}

QVariant StereogramParameters::value(const QString &key, OptionType type) const
{
    if (type == DefaultOption)
        return m_defaultParameters.value(key);
    if (type == SpecifiedOption) {
        if (!m_parameters.contains(key))
            return value(key, DefaultOption);
        QVariant variantValue = m_parameters.value(key);
        if (key == OBSERVER_TO_SCREEN_IN ||
            key == SCREEN_TO_BG_IN ||
            key == BW_EYES_IN) {
            bool ok;
            double val = variantValue.toDouble(&ok);
            if (ok)
                return val;
            return value(key, DefaultOption);
        } else if (key == H_RES_PPI) {
            bool ok;
            int val = variantValue.toInt(&ok);
            if (ok)
                return val;
            return value(key, DefaultOption);
        }
        return variantValue;
    }
    return QVariant();
}

QVariant StereogramParameters::value(const QString &key, Unit unit, OptionType type) const
{
    if (key == OBSERVER_TO_SCREEN_IN ||
        key == SCREEN_TO_BG_IN ||
        key == BW_EYES_IN) {
        if (unit == Inch)
            return value(key, type);
        else if (unit == Pixel)
            return int(value(key, type).toDouble() * value(H_RES_PPI, type).toInt());
    }
    return QVariant();
}

// static
StereogramParameters StereogramParameters::fromSettings(const QSettings *settings)
{
    StereogramParameters sp;
    foreach(const QString &name, sp.parameterNames())
        if (settings->contains("Parameters/" + name))
            sp.setValue(name, settings->value("Parameters/" + name));
    return sp;
}

void StereogramParameters::saveToSettings(QSettings *settings)
{
    foreach(const QString &name, m_defaultParameters.keys())
        settings->setValue("Parameters/" + name, value(name));
}
