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

private:
    const QString MAIN_DIR = R"(C:\ProgramData\PG)";
    const QString PATH_COPY_AFTER_GENERATION = R"(C:\ProgramData\PG\CopyAfterGeneratePassword.txt)";
    const QString PATH_CHBOX_UPPER_SYMBOL = R"(C:\ProgramData\PG\CheckBoxUpperSymbol.txt)";
    const QString PATH_CHBOX_SPECIAL_SYMBOL = R"(C:\ProgramData\PG\CheckBoxSpecialSymbol.txt)";
    const QString PATH_CHBOX_NUMBERS = R"(C:\ProgramData\PG\CheckBoxNumbers.txt)";
    const QString PATH_LAST_LENGTH_PASS = R"(C:\ProgramData\PG\LastLengthPasswords.txt)";
public:
    MainForm(QWidget *parent = nullptr);
    void SaveSettings();
    void SaveParameter(const QString& Path,const char* text);
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
