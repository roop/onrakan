#include "optionsdialog.h"
#include "ui_optionsdialog.h"

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

void OptionsDialog::loadDefaultValues()
{
    ui->observerToScreen->setValue(m_settings->option("observerToScreenInches", Settings::DefaultOption).toDouble());
    ui->screenToBackground->setValue(m_settings->option("screenToBackgroundInches", Settings::DefaultOption).toDouble());
    ui->betweenEyes->setValue(m_settings->option("betweenEyesInches", Settings::DefaultOption).toDouble());
    ui->horizontalResolution->setValue(m_settings->option("horizontalResolutionPpi", Settings::DefaultOption).toInt());
    ui->verticalResolution->setValue(m_settings->option("verticalResolutionPpi", Settings::DefaultOption).toInt());
    ui->sameAsHorizontalResolution->setChecked(m_settings->option("verticalResolutionSameAsHorizontalResolutionBool", Settings::DefaultOption).toBool());
}

void OptionsDialog::loadFromSettings()
{
    ui->observerToScreen->setValue(m_settings->option("observerToScreenInches", Settings::SpecifiedOption).toDouble());
    ui->screenToBackground->setValue(m_settings->option("screenToBackgroundInches", Settings::SpecifiedOption).toDouble());
    ui->betweenEyes->setValue(m_settings->option("betweenEyesInches", Settings::SpecifiedOption).toDouble());
    ui->horizontalResolution->setValue(m_settings->option("horizontalResolutionPpi", Settings::SpecifiedOption).toInt());
    ui->verticalResolution->setValue(m_settings->option("verticalResolutionPpi", Settings::SpecifiedOption).toInt());
    ui->sameAsHorizontalResolution->setChecked(m_settings->option("verticalResolutionSameAsHorizontalResolutionBool", Settings::SpecifiedOption).toBool());
}

void OptionsDialog::saveToSettings()
{
    if (m_settings) {
        m_settings->beginGroup("Options");
        m_settings->setValue("observerToScreenInches", ui->observerToScreen->value());
        m_settings->setValue("screenToBackgroundInches", ui->screenToBackground->value());
        m_settings->setValue("betweenEyesInches", ui->betweenEyes->value());
        m_settings->setValue("horizontalResolutionPpi", ui->horizontalResolution->value());
        m_settings->setValue("verticalResolutionPpi", ui->verticalResolution->value());
        m_settings->setValue("verticalResolutionSameAsHorizontalResolutionBool", ui->sameAsHorizontalResolution->isChecked());
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
