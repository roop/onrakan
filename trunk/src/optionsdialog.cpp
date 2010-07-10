#include "optionsdialog.h"
#include "ui_optionsdialog.h"
#include "stereogramparameters.h"

OptionsDialog::OptionsDialog(QSettings *settings, QWidget *parent) :
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

QSettings *OptionsDialog::settings() const
{
    return m_settings;
}

void OptionsDialog::loadDefaultValues()
{
    StereogramParameters parameters;
    ui->observerToScreen->setValue(parameters.value(OBSERVER_TO_SCREEN_IN, StereogramParameters::DefaultOption).toDouble());
    ui->screenToBackground->setValue(parameters.value(SCREEN_TO_BG_IN, StereogramParameters::DefaultOption).toDouble());
    ui->betweenEyes->setValue(parameters.value(BW_EYES_IN, StereogramParameters::DefaultOption).toDouble());
    ui->horizontalResolution->setValue(parameters.value(H_RES_PPI, StereogramParameters::DefaultOption).toInt());
}

void OptionsDialog::loadFromSettings()
{
    StereogramParameters parameters = StereogramParameters::fromSettings(m_settings);
    ui->observerToScreen->setValue(parameters.value(OBSERVER_TO_SCREEN_IN, StereogramParameters::SpecifiedOption).toDouble());
    ui->screenToBackground->setValue(parameters.value(SCREEN_TO_BG_IN, StereogramParameters::SpecifiedOption).toDouble());
    ui->betweenEyes->setValue(parameters.value(BW_EYES_IN, StereogramParameters::SpecifiedOption).toDouble());
    ui->horizontalResolution->setValue(parameters.value(H_RES_PPI, StereogramParameters::SpecifiedOption).toInt());
}

void OptionsDialog::saveToSettings()
{
    if (m_settings) {
        StereogramParameters sp;
        sp.setValue(OBSERVER_TO_SCREEN_IN, ui->observerToScreen->value());
        sp.setValue(SCREEN_TO_BG_IN, ui->screenToBackground->value());
        sp.setValue(BW_EYES_IN, ui->betweenEyes->value());
        sp.setValue(H_RES_PPI, ui->horizontalResolution->value());
        sp.saveToSettings(m_settings);
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
