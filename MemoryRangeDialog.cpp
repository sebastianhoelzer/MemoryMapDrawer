#include "MemoryRangeDialog.h"

#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

MemoryRangeDialog::MemoryRangeDialog(QWidget *parent) :
    QDialog(parent)
{

    QLabel *nameLabel = new QLabel("Name of memory section:", this);
    m_nameEdit = new QLineEdit(this);
    QLabel *startRangeLabel = new QLabel("Start of memory section:", this);
    m_startRangeEdit = new QSpinBox(this);
    QLabel *endRangeLabel = new QLabel("End of memory section:", this);
    m_endRangeEdit = new QSpinBox(this);

    m_startRangeEdit->setMaximum(65535);
    m_endRangeEdit->setMaximum(65535);

    QGridLayout *gridLayout = new QGridLayout();
    QVBoxLayout *mainLayout = new QVBoxLayout();
    m_okButton = new QPushButton("Ok", this);
    m_okButton->setAutoDefault(true);

    gridLayout->addWidget(nameLabel, 0,1);
    gridLayout->addWidget(m_nameEdit, 0,2);
    gridLayout->addWidget(startRangeLabel, 1, 1);
    gridLayout->addWidget(m_startRangeEdit, 1, 2);
    gridLayout->addWidget(endRangeLabel, 2, 1);
    gridLayout->addWidget(m_endRangeEdit, 2, 2);
    gridLayout->addWidget(m_okButton, 3, 2);

    mainLayout->addLayout(gridLayout);
    setLayout(mainLayout);

    setFixedSize(sizeHint());

    setWindowTitle(tr("Enter Memory Range"));

    connect(m_okButton, &QPushButton::clicked, this, &MemoryRangeDialog::approved);
}

MemoryRangeDialog::~MemoryRangeDialog()
{

}

QString MemoryRangeDialog::name()
{
    return m_nameEdit->text();
}

int MemoryRangeDialog::startRange()
{
    return m_startRangeEdit->value();
}

int MemoryRangeDialog::endRange()
{
    return m_endRangeEdit->value();
}


