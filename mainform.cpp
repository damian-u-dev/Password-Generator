#include "mainform.h"
#include "aboutpg.h"
#include "./ui_mainform.h"
#include "QString"
#include "QClipboard"
#include "QDir"
#include "QFile"
#include <QKeyEvent>
#include "random"
#include <chrono>

std::mt19937 g_engine(std::chrono::steady_clock::now().time_since_epoch().count());
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

bool MainForm::ReadFile(const QString &Path) const
{
    QFile file(Path);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    char buf[10];
    file.readLine(buf, sizeof(buf));
    return static_cast<bool>(!strcmp(buf,"true"));
}

void MainForm::SaveSettings() const
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

void MainForm::SaveParameter(const QString &Path, const char* text) const
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


QString MainForm::GeneratePassword() const
{
    const int PASSWORD_LENGTH = ui->passwordLength_box->value();

    int countNumbers = 0;
    int countSpecialSymbols = 0;
    int countRegularSymbols = PASSWORD_LENGTH;
    SetUpCountOfGeneration(countNumbers, countSpecialSymbols, countRegularSymbols, PASSWORD_LENGTH);

    QString generatedPassword;
    bool wasOneUpper = false;
    int countGeneratedSymbols = 0;

    while (countGeneratedSymbols != PASSWORD_LENGTH)
    {
        GenerationType generatedType = static_cast<GenerationType>(g_distibution(g_engine) % 3);

        if (ui->checkBox_Numbers->isChecked() && generatedType == GenerationType::Number && countNumbers > 0)
        {
            generatedPassword += GenerateNumber();
            countGeneratedSymbols++;
            countNumbers--;
        }
        else if (ui->checkBox_SpecialSymbols->isChecked() && generatedType == GenerationType::SpecialSymbol && countSpecialSymbols > 0)
        {
            generatedPassword += GenerateSpecialSymbol();
            countGeneratedSymbols++;
            countSpecialSymbols--;
        }
        else if(generatedType == GenerationType::RegularSymbol && countRegularSymbols > 0)
        {
            char symbol = GenerateSymbol();
            if(isupper(symbol) && !wasOneUpper)
                wasOneUpper = true;

            if(ui->checkBox_UpperSymbols->isChecked() && (countRegularSymbols - 1) == 0 && !wasOneUpper)
            {
                symbol = toupper(symbol);
                wasOneUpper = true;
            }

            generatedPassword += symbol;
            countGeneratedSymbols++;
            countRegularSymbols--;
        }
    }
    return generatedPassword;
}

char MainForm::GenerateSymbol() const
{
    char randomSymbol = g_distibution(g_engine) % ('z' + 1 - 'a') + 'a';

    if(ui->checkBox_UpperSymbols->isChecked())
    {
        bool toUpper = g_distibution(g_engine) % 2;

        return (toUpper) ? toupper(randomSymbol) : randomSymbol;
    }
    return randomSymbol;
}

char MainForm::GenerateNumber() const
{
    char randomNumber = g_distibution(g_engine) % ('9' + 1 - '0') + '0';
    return randomNumber;
}

char MainForm::GenerateSpecialSymbol() const
{
    const char specialSymbols[] = { '!', '"', '#', '$', '%', '&', '(', ')', '*', '@', '_', '-', '=', '+' };
    int sizeArray = sizeof(specialSymbols);
    int randomIndex = g_distibution(g_engine) % sizeArray;

    return specialSymbols[randomIndex];
}

void MainForm::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_W)
    {
        close();
    }
    QMainWindow::keyPressEvent(event);
}

void MainForm::on_generatePassword_button_clicked()
{
    const QString generatedPassword = GeneratePassword();
    ui->generatedPassword_box->setText(generatedPassword);

    if(ui->copyAfterGeneration_Menu->isChecked())
        on_copyPasswordClipboard_button_clicked();
}


void MainForm::on_copyPasswordClipboard_button_clicked()
{
    QClipboard* clipboard = QGuiApplication::clipboard();
    clipboard->setText(ui->generatedPassword_box->toPlainText());

}

void MainForm::SetUpCountOfGeneration(int &countNumbers, int &countSpecialSymbols, int &countRegularSymbols, int passwordLength) const
{
    if (ui->checkBox_Numbers->isChecked())
    {
        if (passwordLength <= 12)
        {
            countNumbers = g_distibution(g_engine) % 2 + 1;
        }
        else if (passwordLength > 12 && passwordLength < 24)
        {
            countNumbers = g_distibution(g_engine) % 4 + 2;
        }
        else if (passwordLength > 24)
        {
            countNumbers = g_distibution(g_engine) % 7 + 4;
        }

        if(countNumbers == 0)
        {
            countNumbers = 2;
        }
    }
    if (ui->checkBox_SpecialSymbols->isChecked())
    {
        if (passwordLength <= 12)
        {
            countSpecialSymbols = g_distibution(g_engine) % 2 + 1;
        }
        else if (passwordLength > 12 && passwordLength < 24)
        {
            countSpecialSymbols = g_distibution(g_engine) % 4 + 2;
        }
        else if (passwordLength > 24)
        {
            countSpecialSymbols = g_distibution(g_engine) % 7 + 4;
        }

        if(countSpecialSymbols == 0)
        {
            countSpecialSymbols = 2;
        }
    }
    countRegularSymbols -= countNumbers + countSpecialSymbols;
}

void MainForm::on_actionAbout_triggered()
{
    AboutPG* form = new AboutPG(this);
    form->setAttribute(Qt::WA_DeleteOnClose);
    form->setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);
    form->exec();
}

void MainForm::on_actionExit_triggered()
{
    this->close();
}

