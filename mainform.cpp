#include "mainform.h"
#include "./ui_mainform.h"
#include "QString"
#include "QClipboard"
#include "QDir"
#include "QFile"
#include "random"

unsigned int seed = static_cast<unsigned int>(time(nullptr));
std::mt19937 g_engine(seed);
std::uniform_int_distribution<int> g_distibution;

MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainForm)
{
    ui->setupUi(this);
    SetUpCheckboxes();
    SetLastLength();

    on_generatePassword_button_clicked();
}

void MainForm::SetUpCheckboxes()
{
    QDir settingsDirectory(MAIN_DIR);
    if (!settingsDirectory.exists())
        settingsDirectory.mkdir(MAIN_DIR);

    ui->copyAfterGeneration_Menu->setChecked(ReadFile(PATH_COPY_AFTER_GENERATION));
    ui->checkBox_UpperSymbols->setChecked(ReadFile(PATH_CHBOX_UPPER_SYMBOL));
    ui->checkBox_SpecialSymbols->setChecked(ReadFile(PATH_CHBOX_SPECIAL_SYMBOL));
    ui->checkBox_Numbers->setChecked(ReadFile(PATH_CHBOX_NUMBERS));
}

void MainForm::SetLastLength()
{
    QFile file(PATH_LAST_LENGTH_PASS);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        ui->passwordLength_box->setValue(8);

    char buf[10];
    file.readLine(buf,sizeof(buf));
    QString temp(buf);
    ui->passwordLength_box->setValue(temp.toInt());

}

bool MainForm::ReadFile(const QString &Path)
{
    QFile file(Path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    char buf[10];
    file.readLine(buf, sizeof(buf));
    return static_cast<bool>(!strcmp(buf,"true"));
}

void MainForm::SaveSettings()
{
    QDir settingsDirectory(MAIN_DIR);
    if (!settingsDirectory.exists())
        settingsDirectory.mkdir(MAIN_DIR);

    SaveParameter(PATH_COPY_AFTER_GENERATION, (ui->copyAfterGeneration_Menu->isChecked() ? "true" : "false"));
    SaveParameter(PATH_CHBOX_UPPER_SYMBOL, (ui->checkBox_UpperSymbols->isChecked() ? "true" : "false"));
    SaveParameter(PATH_CHBOX_SPECIAL_SYMBOL, (ui->checkBox_SpecialSymbols->isChecked() ? "true" : "false"));
    SaveParameter(PATH_CHBOX_NUMBERS, (ui->checkBox_Numbers->isChecked() ? "true" : "false"));

    QString strNum = QString::number(ui->passwordLength_box->value());
    SaveParameter(PATH_LAST_LENGTH_PASS, strNum.toUtf8().constData());
}

void MainForm::SaveParameter(const QString &Path, const char* text)
{
    QFile file(Path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(text);
    file.close();
}

MainForm::~MainForm()
{
    SaveSettings();
    delete ui;
}


QString MainForm::GeneratePassword()
{
    int countGeneratedSymbols = 0;
    const int passwordLength = ui->passwordLength_box->value();
    QString randomPassword;

    int countOfNumbers = 0;
    int countOfSpecialSymbols = 0;
    int countOfUsualSymbols = passwordLength;

    SetUpCountOfGeneration(countOfNumbers,countOfSpecialSymbols,countOfUsualSymbols,passwordLength);

    while (countGeneratedSymbols != passwordLength)
    {
        int randomChoice = g_distibution(g_engine) % 3;

        if (ui->checkBox_Numbers->isChecked() && randomChoice == 0 && countOfNumbers > 0)
        {
            randomPassword += GenerateNumber();
            countGeneratedSymbols++;
            countOfNumbers--;
        }
        else if (ui->checkBox_SpecialSymbols->isChecked() && randomChoice == 1 && countOfSpecialSymbols > 0)
        {
            randomPassword += GenerateSpecialSymbol();
            countGeneratedSymbols++;
            countOfSpecialSymbols--;
        }
        else if(randomChoice == 2 && countOfUsualSymbols > 0)
        {
            randomPassword += GenerateSymbol();
            countGeneratedSymbols++;
            countOfUsualSymbols--;
        }
    }
    return randomPassword;
}

char MainForm::GenerateSymbol()
{
    char randomSymbol = g_distibution(g_engine) % ('z' + 1 - 'a') + 'a';

    if(ui->checkBox_UpperSymbols->isChecked())
    {
        bool toUpper = g_distibution(g_engine) % 2;

        return (toUpper) ? toupper(randomSymbol) : randomSymbol;
    }
    return randomSymbol;
}

char MainForm::GenerateNumber()
{
    char randomNumber = g_distibution(g_engine) % ('9' + 1 - '0') + '0';

    return randomNumber;
}

char MainForm::GenerateSpecialSymbol()
{
    char specialSymbols[] = { '!', '"', '#', '$', '%', '&', '(', ')', '*', '@', '_', '-', '=', '+' };

    int sizeArray = sizeof(specialSymbols);
    int randomIndex = g_distibution(g_engine) % sizeArray;

    return specialSymbols[randomIndex];
}

void MainForm::on_generatePassword_button_clicked()
{
    const QString generatedPassword = GeneratePassword();
    ui->generatedPassword_box->setText(generatedPassword);

    if(ui->copyAfterGeneration_Menu->isChecked())
    {
        on_copyPasswordClipboard_button_clicked();
    }
}


void MainForm::on_copyPasswordClipboard_button_clicked()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->generatedPassword_box->toPlainText());

}

void MainForm::SetUpCountOfGeneration(int &countOfNumbers, int &countOfSpecialSymbols, int &countOfUsualSymbols, int passwordLength)
{
    if (ui->checkBox_Numbers->isChecked())
    {
        if (passwordLength <= 12)
        {
            countOfNumbers = g_distibution(g_engine) % 2 + 1;
        }
        else if (passwordLength > 12 && passwordLength < 24)
        {
            countOfNumbers = g_distibution(g_engine) % 4 + 2;
        }
        else if (passwordLength > 24)
        {
            countOfNumbers = g_distibution(g_engine) % 7 + 4;
        }
    }
    if (ui->checkBox_SpecialSymbols->isChecked())
    {
        if (passwordLength <= 12)
        {
            countOfSpecialSymbols = g_distibution(g_engine) % 2 + 1;
        }
        else if (passwordLength > 12 && passwordLength < 24)
        {
            countOfSpecialSymbols = g_distibution(g_engine) % 4 + 2;
        }
        else if (passwordLength > 24)
        {
            countOfSpecialSymbols = g_distibution(g_engine) % 7 + 4;
        }
    }

    countOfUsualSymbols -= countOfNumbers + countOfSpecialSymbols;
}

