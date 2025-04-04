#include "aboutpg.h"
#include "ui_aboutpg.h"

AboutPG::AboutPG(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AboutPG)
{
    ui->setupUi(this);

    ui->link_label->setText("<a href='https://github.com/damian-u-dev'>https://github.com/damian-u-dev</a>");
    ui->link_label->setTextFormat(Qt::RichText);
    ui->link_label->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->link_label->setOpenExternalLinks(true);
}

AboutPG::~AboutPG()
{
    delete ui;
}
