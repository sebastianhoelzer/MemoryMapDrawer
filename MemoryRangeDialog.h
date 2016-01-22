#ifndef MEMORYRANGEDIALOG_H
#define MEMORYRANGEDIALOG_H

#include <QDialog>
#include <QSpinBox>

class MemoryRangeDialog : public QDialog
{
    Q_OBJECT

public:
    MemoryRangeDialog(QWidget *parent = 0);
    ~MemoryRangeDialog();

    QString name();
    int startRange();
    int endRange();
signals:
    void approved();

private:
    QPushButton *m_okButton;
    QLineEdit *m_nameEdit;
    QSpinBox *m_startRangeEdit;
    QSpinBox *m_endRangeEdit;
};

#endif // MEMORYRANGEDIALOG_H
