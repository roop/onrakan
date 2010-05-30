#include "settings.h"

Settings::Settings(QObject *parent)
    : QSettings("Onrakan", "onrakan", parent)
{
    initialize();
}

void Settings::initialize()
{
    m_defaultOptions[OBSERVER_TO_SCREEN_IN] = 15;
    m_defaultOptions[SCREEN_TO_BG_IN] = 15;
    m_defaultOptions[BW_EYES_IN] = 2.5;
    m_defaultOptions[H_RES_PPI] = 72;
    m_defaultOptions[V_RES_PPI] = 72;
    m_defaultOptions[V_RES_EQ_H_RES] = true;
}

QVariant Settings::option(const QString &key, OptionType type)
{
    if (type == DefaultOption)
        return m_defaultOptions.value(key);
    if (type == SpecifiedOption) {
        if (!contains("Options/" + key))
            return option(key, DefaultOption);
        QVariant variantValue = value("Options/" + key);
        if (key == OBSERVER_TO_SCREEN_IN ||
            key == SCREEN_TO_BG_IN ||
            key == BW_EYES_IN) {
            bool ok;
            double val = variantValue.toDouble(&ok);
            if (ok)
                return val;
            return option(key, DefaultOption);
        } else if (key == H_RES_PPI ||
                   key == V_RES_PPI) {
            bool ok;
            int val = variantValue.toInt(&ok);
            if (ok)
                return val;
            return option(key, DefaultOption);
        }
        return variantValue;
    }
    return QVariant();
}

QVariant Settings::option(const QString &key, Unit unit, OptionType type)
{
    if (key == OBSERVER_TO_SCREEN_IN ||
        key == SCREEN_TO_BG_IN ||
        key == BW_EYES_IN) {
        if (unit == Inch)
            return option(key, type);
        else if (unit == Pixel)
            return int(option(key, type).toDouble() * option(H_RES_PPI, type).toInt());
    }
    return QVariant();
}
