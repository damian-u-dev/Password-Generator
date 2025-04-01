#include "mainform.h"
#include "./ui_mainform.h"
#include "QString"
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
    int countRandomSymbols = 0;
    const int passwordLength = ui->passwordLength_box->value();
    QString randomPassword;

    while (countRandomSymbols != passwordLength)
    {
        int randomChoice = g_distibution(g_engine) % 3;

        if (randomChoice == 0)
        {
            randomPassword += GenerateNumber();
        }
        else if (randomChoice == 1)
        {
            randomPassword += GenerateSymbol();
        }
        else
        {
            randomPassword += GenerateSpecialSymbol();
        }

        countRandomSymbols++;
    }
    return randomPassword;
}

char MainForm::GenerateSymbol()
{
    char randomSymbol = g_distibution(g_engine) % ('z' + 1 - 'a') + 'a';

    bool toUpper = g_distibution(g_engine) % 2;

    return (toUpper) ? toupper(randomSymbol) : randomSymbol;
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

