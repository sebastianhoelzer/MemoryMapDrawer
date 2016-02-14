 #include "EditorWindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStandardPaths>
#include <QMessageBox>
#include <QCloseEvent>
#include "MapDataFileReadWriter.h"
#include "MemoryRange.h"
#include "MemoryRangeDialog.h"
#include "AboutDialog.h"

EditorWindow::EditorWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditorWindow),
    m_dialog(0)
{
    ui->setupUi(this);

    MemoryRange range1;
    range1.setLowerLimit(0);
    range1.setUpperLimit(127);
    range1.setName("Bootloader");

    m_mapData.setName("Untitled");
    m_mapData.addMemorySection(range1);
    m_memoryRangesModel.addMemoryRange(range1);
    ui->addressPrefix->setText("0x");
    ui->addressSuffix->setText("h");

    ui->memoryRanges->setModel(&m_memoryRangesModel);
    ui->drawingWidget->setMapData(m_mapData);
    m_memoryRangesModel.setMemorySections(m_mapData.memorySections());
    ui->memoryRanges->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_originalData = m_mapData;

    connect(&m_memoryRangesModel, SIGNAL(sectionsChanged()), this, SLOT(updateDrawing()));
    connect(&m_memoryRangesModel, SIGNAL(sectionsChanged()), this, SLOT(updateMapData()));

    connect(ui->memorySize, SIGNAL(valueChanged(int)), this, SLOT(handleSizeChange(int)));

    connect(ui->actionExport_to_PNG, SIGNAL(triggered()), this, SLOT(handleExportPngAction()));
    connect(ui->actionExport_to_SVG, SIGNAL(triggered()), this, SLOT(handleExportSvgAction()));

    connect(ui->busWidthSelector, SIGNAL(currentIndexChanged(int)), this, SLOT(handleBusWidthChanged()));

    connect(ui->showAddressLabelLeftCheckBox, SIGNAL(clicked(bool)), this, SLOT(handleShowAddressLabelLeftChanged(bool)));
    connect(ui->showAddressLabelRightCheckBox, SIGNAL(clicked(bool)), this, SLOT(handelShowAddressLabelRightChanged(bool)));

    connect(ui->showAddressPrefix, SIGNAL(clicked(bool)),this, SLOT(handleShowAddressPrefixChanged(bool)));
    connect(ui->showAddressSuffix, SIGNAL(clicked(bool)), this, SLOT(handleShowAddressSuffixChanged(bool)));

    connect(ui->addressPrefix, SIGNAL(textEdited(QString)), this, SLOT(handlePrefixChanged(QString)));
    connect(ui->addressSuffix, SIGNAL(textEdited(QString)), this, SLOT(handleSuffixChanged(QString)));

    connect(ui->pixelWidthChooser, SIGNAL(valueChanged(int)), this, SLOT(handleWidthChanged(int)));
    connect(ui->pixelHeightChooser, SIGNAL(valueChanged(int)), this, SLOT(handleHeightChanged(int)));

    updateDrawing();
    updateMapData();
}

EditorWindow::~EditorWindow()
{
    delete ui;
}

void EditorWindow::on_addMemoryRange_clicked()
{
    if(!m_dialog)
    {
        m_dialog = new MemoryRangeDialog(this);
        connect(m_dialog, &MemoryRangeDialog::approved, this, &EditorWindow::handleNewMemorySection);
    }
    m_dialog->show();
    m_dialog->activateWindow();
}

void EditorWindow::on_removeRange_clicked()
{
    QItemSelectionModel *select = ui->memoryRanges->selectionModel();

    if (select->hasSelection())
    {
        foreach(QModelIndex index, select->selectedRows())
        {
            m_mapData.removeMemorySection(index.row());
            m_memoryRangesModel.removeMemoryRange(index.row());
        }
    }
    updateSaveState();
}

void EditorWindow::handleNewMemorySection()
{
    m_dialog->hide();
    MemoryRange range;
    range.setName(m_dialog->name());
    range.setLowerLimit(m_dialog->startRange());
    range.setUpperLimit(m_dialog->endRange());

    m_mapData.addMemorySection(range);
    m_memoryRangesModel.addMemoryRange(range);
    updateSaveState();
}

void EditorWindow::handleSizeChange(int newSize)
{
    m_mapData.setMaxAddress(newSize -1);
    ui->drawingWidget->setMapData(m_mapData);
    updateSaveState();
}

void EditorWindow::handleExportPngAction()
{
    QString defaultPath = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "", QStandardPaths::LocateDirectory) + m_mapData.name();
    QString path = QFileDialog::getSaveFileName(this, "Export as PNG", defaultPath, tr("Portable Network Graphics (*.png)"));
    if (path.isNull())
        return;
    ui->drawingWidget->exportToPng(path);
}

void EditorWindow::handleExportSvgAction()
{
    QString defaultPath = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "", QStandardPaths::LocateDirectory) + m_mapData.name();
    QString path = QFileDialog::getSaveFileName(this, "Export as SVG", defaultPath, tr("Scalable Vector Graphics (*.svg)"));
    if (path.isNull())
        return;
    ui->drawingWidget->exportToSvg(path);
}

void EditorWindow::handleBusWidthChanged()
{
    switch(ui->busWidthSelector->currentIndex())
    {
    case 0:
        m_mapData.setFieldWidth(2);
        break;
    case 1:
         m_mapData.setFieldWidth(4);
        break;
    case 2:
         m_mapData.setFieldWidth(8);
        break;
    case 3:
         m_mapData.setFieldWidth(16);
        break;
    }
    ui->drawingWidget->setMapData(m_mapData);
    updateSaveState();
}

void EditorWindow::handleShowAddressLabelLeftChanged(bool showAddressLabelLeft)
{
    m_mapData.setShowAddressLabelLeft(showAddressLabelLeft);
    ui->drawingWidget->setMapData(m_mapData);
    updateSaveState();
}

void EditorWindow::handelShowAddressLabelRightChanged(bool showAddressLabelRight)
{
    m_mapData.setShowAddressLabelRight(showAddressLabelRight);
    ui->drawingWidget->setMapData(m_mapData);
    updateSaveState();
}

void EditorWindow::handleShowAddressPrefixChanged(bool showAddressPrefix)
{
    m_mapData.setShowPrefix(showAddressPrefix);
    ui->drawingWidget->setMapData(m_mapData);
    updateSaveState();
}

void EditorWindow::handleShowAddressSuffixChanged(bool showAddressSuffix)
{
    m_mapData.setShowSuffix(showAddressSuffix);
    ui->drawingWidget->setMapData(m_mapData);
    updateSaveState();
}

void EditorWindow::handlePrefixChanged(QString prefix)
{
    m_mapData.setPrefix(prefix);
    ui->drawingWidget->setMapData(m_mapData);
    updateSaveState();
}

void EditorWindow::handleSuffixChanged(QString suffix)
{
    m_mapData.setSuffix(suffix);
    ui->drawingWidget->setMapData(m_mapData);
    updateSaveState();
}

void EditorWindow::handleWidthChanged(int width)
{
    m_mapData.setWidth(width);
    ui->drawingWidget->setMapData(m_mapData);
    updateSaveState();
}

void EditorWindow::handleHeightChanged(int height)
{
    m_mapData.setHeight(height);
    ui->drawingWidget->setMapData(m_mapData);
    updateSaveState();
}

void EditorWindow::updateSaveState()
{
    bool hasMemoryMapChanged = (m_mapData != m_originalData);

    ui->actionSave->setEnabled(hasMemoryMapChanged);

    QString nameOfApplication = QFileInfo( QCoreApplication::applicationFilePath() ).fileName().split(".",QString::SkipEmptyParts).at(0);

    setWindowTitle(QString("%1 - %2%3")
                   .arg(nameOfApplication)
                   .arg(m_mapData.name())
                   .arg(hasMemoryMapChanged ? "*" : ""));
}

void EditorWindow::updateDrawing()
{
    m_mapData.setMemorySections(m_memoryRangesModel.getMemorySections());
    ui->drawingWidget->setMapData(m_mapData);

}

void EditorWindow::updateMapData()
{
    m_mapData.setMemorySections(m_memoryRangesModel.getMemorySections());

    if(m_mapData.fieldWidth() == 2)
        ui->busWidthSelector->setCurrentIndex(0);
    if(m_mapData.fieldWidth() == 4)
        ui->busWidthSelector->setCurrentIndex(1);
    if(m_mapData.fieldWidth() == 8)
        ui->busWidthSelector->setCurrentIndex(2);
    if(m_mapData.fieldWidth() == 16)
        ui->busWidthSelector->setCurrentIndex(3);

    ui->showAddressPrefix->setChecked(m_mapData.showPrefix());
    ui->showAddressSuffix->setChecked(m_mapData.showSuffix());
    ui->addressPrefix->setText(m_mapData.prefix());
    ui->addressSuffix->setText(m_mapData.suffix());

    ui->showAddressLabelLeftCheckBox->setChecked(m_mapData.showAddressLabelLeft());
    ui->showAddressLabelRightCheckBox->setChecked(m_mapData.showAddressLabelRight());

    ui->memorySize->setValue(m_mapData.maxAddress() + 1);
    ui->pixelHeightChooser->setValue(m_mapData.height());
    ui->pixelWidthChooser->setValue(m_mapData.width());
}

void EditorWindow::on_actionAbout_triggered()
{
    AboutDialog *aboutDialog;
    aboutDialog = new AboutDialog(this);
    aboutDialog->show();
}

void EditorWindow::on_actionSave_triggered()
{
    QString path = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "", QStandardPaths::LocateDirectory) + m_mapData.name() + ".mmp";
    m_originalData = m_mapData;
    updateSaveState();

    MapDataFileReadWriter writer;
    writer.writeFile(path, m_mapData);
}

void EditorWindow::on_actionOpen_triggered()
{
    QString defaultPath = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "", QStandardPaths::LocateDirectory);
    QString filePath = QFileDialog::getOpenFileName(this, "Open", defaultPath, tr("Memory Map (*.mmp)"));

    if (filePath.isNull())
    {
        return;
    }

    MapDataFileReadWriter reader;
    m_mapData = reader.readFile(filePath);
    m_originalData = m_mapData;
    updateSaveState();

    m_memoryRangesModel.setMemorySections(m_mapData.memorySections());
    ui->drawingWidget->setMapData(m_mapData);
}

void EditorWindow::on_actionSaveAs_triggered()
{
    QString defaultPath = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "", QStandardPaths::LocateDirectory) + m_mapData.name();
    QString path = QFileDialog::getSaveFileName(this, "Save as ...", defaultPath, tr("Memory Map (*.mmp)"));

    qDebug() << path;
    //end the function if cancel has been pressed
    if (path.isNull())
    {
        return;
    }

    QString name = QFileInfo(path).fileName().split(".",QString::SkipEmptyParts).at(0);
    m_mapData.setName(name);
    m_originalData = m_mapData;
    updateSaveState();

    MapDataFileReadWriter writer;
    writer.writeFile(path, m_mapData);
}

void EditorWindow::on_actionNew_triggered()
{
    MapData newMapData;

    m_mapData = newMapData;
    m_memoryRangesModel.setMemorySections(m_mapData.memorySections());
    ui->drawingWidget->setMapData(m_mapData);
}


void EditorWindow::closeEvent (QCloseEvent *event)
{
    on_actionQuit_triggered();
    event->ignore();
}

void EditorWindow::on_actionQuit_triggered()
{
    if(m_mapData != m_originalData)
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this,
                                      QFileInfo(QCoreApplication::applicationFilePath()).fileName().split(".",QString::SkipEmptyParts).at(0),
                                      tr("Do you want to save changes?"),
                                      QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if (reply == QMessageBox::Yes)
        {
            on_actionSaveAs_triggered();
            QApplication::quit();
        }
        else if (reply == QMessageBox::No)
        {
            QApplication::quit();
        }
        else if (reply == QMessageBox::Cancel)
        {
            return;
        }
    }
    else
    {
        QApplication::quit();
    }
}
