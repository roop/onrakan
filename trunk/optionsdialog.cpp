#include "optionsdialog.h"
#include "ui_optionsdialog.h"

OptionsDialog::OptionsDialog(QSettings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    m_settings = settings;
    loadDefaultValues();
    loadFromSettings();
    connect(this, SIGNAL(accepted()), this, SLOT(saveToSettings()));
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::loadDefaultValues()
{
    ui->observerToScreen->setValue(15);
    ui->screenToBackground->setValue(15);
    ui->betweenEyes->setValue(2.5);
    ui->horizontalResolution->setValue(72);
    ui->verticalResolution->setValue(72);
    ui->sameAsHorizontalResolution->setChecked(true);
}

void OptionsDialog::loadFromSettings()
{
    if (m_settings) {
        m_settings->beginGroup("Options");
        bool ok;
        double valueDouble;
        valueDouble = m_settings->value("observerToScreenDistance").toDouble(&ok);
        if (ok)
            ui->observerToScreen->setValue(valueDouble);
        valueDouble = m_settings->value("screenToBackgroundDistance").toDouble(&ok);
        if (ok)
            ui->screenToBackground->setValue(valueDouble);
        valueDouble = m_settings->value("betweenEyesDistance").toDouble(&ok);
        if (ok)
            ui->betweenEyes->setValue(valueDouble);
        int valueInt;
        valueInt = m_settings->value("horizontalResolution").toInt(&ok);
        if (ok)
            ui->horizontalResolution->setValue(valueInt);
        valueInt = m_settings->value("verticalResolution").toInt(&ok);
        if (ok)
            ui->verticalResolution->setValue(valueInt);
        bool checked;
        if (m_settings->contains("verticalResolutionSameAsHorizontalResolution")) {
            checked = m_settings->value("verticalResolutionSameAsHorizontalResolution").toBool();
            ui->sameAsHorizontalResolution->setChecked(checked);
        }
        m_settings->endGroup();
    }
}

void OptionsDialog::saveToSettings()
{
    if (m_settings) {
        m_settings->beginGroup("Options");
        m_settings->setValue("observerToScreenDistance", ui->observerToScreen->value());
        m_settings->setValue("screenToBackgroundDistance", ui->screenToBackground->value());
        m_settings->setValue("betweenEyesDistance", ui->betweenEyes->value());
        m_settings->setValue("horizontalResolution", ui->horizontalResolution->value());
        m_settings->setValue("verticalResolution", ui->horizontalResolution->value());
        m_settings->setValue("verticalResolutionSameAsHorizontalResolution", ui->sameAsHorizontalResolution->isChecked());
        m_settings->endGroup();
    }
}

void OptionsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
