#include "mainform.h"
#include "./ui_mainform.h"
#include "QString"
#include "QClipboard"
#include "random"

unsigned int seed = static_cast<unsigned int>(time(nullptr));
std::mt19937 g_engine(seed);
std::uniform_int_distribution<int> g_distibution;

MainForm::MainForm(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainForm)
{
    ui->setupUi(this);
    on_generatePassword_button_clicked();
}

MainForm::~MainForm()
{
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

