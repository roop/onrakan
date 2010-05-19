#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui {
    class OptionsDialog;
}

class OptionsDialog : public QDialog {
    Q_OBJECT
public:
    explicit OptionsDialog(QSettings *settings = 0, QWidget *parent = 0);
    ~OptionsDialog();

public slots:
    void loadDefaultValues();
    void loadFromSettings();
    void saveToSettings();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::OptionsDialog *ui;
    QSettings *m_settings;
};

#endif // OPTIONSDIALOG_H
