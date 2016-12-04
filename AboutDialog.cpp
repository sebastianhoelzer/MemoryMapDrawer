#include "AboutDialog.h"
#include "ui_aboutdialog.h"
#include <QLayout>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    this->window()->setFixedSize(400, 180);

    Qt::WindowFlags flags = windowFlags();
    flags = flags & (~Qt::WindowContextHelpButtonHint);
    setWindowFlags(flags);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}

void AboutDialog::on_closeDialog_clicked()
{
    this->close();
    deleteLater();
}
