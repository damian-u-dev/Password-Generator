#ifndef ABOUTPG_H
#define ABOUTPG_H

#include <QDialog>

namespace Ui {
class AboutPG;
}

class AboutPG : public QDialog
{
    Q_OBJECT

public:
    explicit AboutPG(QWidget *parent = nullptr);
    ~AboutPG();

private:
    Ui::AboutPG *ui;
};

#endif // ABOUTPG_H
