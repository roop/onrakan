#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QSettings>
#include <QAbstractButton>

namespace Ui {
    class OptionsDialog;
}

class Settings;

class OptionsDialog : public QDialog {
    Q_OBJECT
public:
    explicit OptionsDialog(Settings *settings = 0, QWidget *parent = 0);
    ~OptionsDialog();

public slots:
    void loadDefaultValues();
    void loadFromSettings();
    void saveToSettings();

private slots:
    void handleButtonClicked(QAbstractButton *button);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::OptionsDialog *ui;
    Settings *m_settings;
};

#endif // OPTIONSDIALOG_H
