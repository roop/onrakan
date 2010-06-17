#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "settings.h"

OptionsDialog::OptionsDialog(Settings *settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    Q_ASSERT(m_settings);
    m_settings = settings;
    loadFromSettings();
    connect(ui->buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(handleButtonClicked(QAbstractButton*)));
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

Settings *OptionsDialog::settings() const
{
    return m_settings;
}

void OptionsDialog::loadDefaultValues()
{
    ui->observerToScreen->setValue(m_settings->option(OBSERVER_TO_SCREEN_IN, Settings::DefaultOption).toDouble());
    ui->screenToBackground->setValue(m_settings->option(SCREEN_TO_BG_IN, Settings::DefaultOption).toDouble());
    ui->betweenEyes->setValue(m_settings->option(BW_EYES_IN, Settings::DefaultOption).toDouble());
    ui->horizontalResolution->setValue(m_settings->option(H_RES_PPI, Settings::DefaultOption).toInt());
}

void OptionsDialog::loadFromSettings()
{
    ui->observerToScreen->setValue(m_settings->option(OBSERVER_TO_SCREEN_IN, Settings::SpecifiedOption).toDouble());
    ui->screenToBackground->setValue(m_settings->option(SCREEN_TO_BG_IN, Settings::SpecifiedOption).toDouble());
    ui->betweenEyes->setValue(m_settings->option(BW_EYES_IN, Settings::SpecifiedOption).toDouble());
    ui->horizontalResolution->setValue(m_settings->option(H_RES_PPI, Settings::SpecifiedOption).toInt());
}

void OptionsDialog::saveToSettings()
{
    if (m_settings) {
        m_settings->beginGroup("Options");
        m_settings->setValue(OBSERVER_TO_SCREEN_IN, ui->observerToScreen->value());
        m_settings->setValue(SCREEN_TO_BG_IN, ui->screenToBackground->value());
        m_settings->setValue(BW_EYES_IN, ui->betweenEyes->value());
        m_settings->setValue(H_RES_PPI, ui->horizontalResolution->value());
        m_settings->endGroup();
    }
}

void OptionsDialog::handleButtonClicked(QAbstractButton *button)
{
    if (ui->buttonBox->standardButton(button) == QDialogButtonBox::RestoreDefaults) {
        loadDefaultValues();
    } else if (ui->buttonBox->standardButton(button) == QDialogButtonBox::Ok) {
        saveToSettings();
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
