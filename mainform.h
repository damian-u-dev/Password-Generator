#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainForm;
}
QT_END_NAMESPACE

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    MainForm(QWidget *parent = nullptr);
    ~MainForm();

    QString GeneratePassword();
    char GenerateSymbol();
    char GenerateNumber();
    char GenerateSpecialSymbol();

private slots:
    void on_generatePassword_button_clicked();

    void on_copyPasswordClipboard_button_clicked();

private:
    Ui::MainForm *ui;
    void SetUpCountOfGeneration(int& countOfNumbers, int& countOfSpecialSymbols, int& countOfUsualSymbols, int passwordLength);
};
#endif // MAINFORM_H
