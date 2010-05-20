#include "settings.h"

Settings::Settings(QObject *parent)
    : QSettings("Onrakan", "onrakan", parent)
{
    initialize();
}

void Settings::initialize()
{
    m_defaultOptions["observerToScreenInches"] = 15;
    m_defaultOptions["screenToBackgroundInches"] = 15;
    m_defaultOptions["betweenEyesInches"] = 2.5;
    m_defaultOptions["horizontalResolutionPpi"] = 72;
    m_defaultOptions["verticalResolutionPpi"] = 72;
    m_defaultOptions["verticalResolutionSameAsHorizontalResolutionBool"] = true;
}

QVariant Settings::option(const QString &key, OptionType type)
{
    if (type == DefaultOption)
        return m_defaultOptions.value(key);
    if (type == SpecifiedOption) {
        if (!contains("Options/" + key))
            return option(key, DefaultOption);
        QVariant variantValue = value("Options/" + key);
        if (key == "observerToScreenInches" ||
            key == "screenToBackgroundInches" ||
            key == "betweenEyesInches") {
            bool ok;
            double val = variantValue.toDouble(&ok);
            if (ok)
                return val;
            return option(key, DefaultOption);
        } else if (key == "horizontalResolutionPpi" ||
                   key == "verticalResolutionPpi") {
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
    if (key == "observerToScreenInches" ||
        key == "screenToBackgroundInches" ||
        key == "betweenEyesInches") {
        if (unit == Inch)
            return option(key, type);
        else if (unit == Pixel)
            return int(option(key, type).toDouble() * option("horizontalResolutionPpi", type).toInt());
    }
    return QVariant();
}
