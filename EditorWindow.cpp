 #include "EditorWindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
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
}

void EditorWindow::handleNewMemorySection()
{
    qDebug() << "handleNewMemorySection";
    m_dialog->hide();
    MemoryRange range;
    range.setName(m_dialog->name());
    range.setLowerLimit(m_dialog->startRange());
    range.setUpperLimit(m_dialog->endRange());

    m_mapData.addMemorySection(range);
    m_memoryRangesModel.addMemoryRange(range);
}

void EditorWindow::handleSizeChange(int newSize)
{
    m_mapData.setMaxAddress(newSize -1);
    ui->drawingWidget->setMapData(m_mapData);
}

void EditorWindow::handleExportPngAction()
{
    QString path = QFileDialog::getSaveFileName(this, "Export as PNG");
    ui->drawingWidget->exportToPng(path);
}

void EditorWindow::handleExportSvgAction()
{
    QString defaultPath = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "", QStandardPaths::LocateDirectory);
    QString path = QFileDialog::getSaveFileName(this, "Export as SVG", defaultPath, tr("Image File (*.svg)"));
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
}

void EditorWindow::handleShowAddressLabelLeftChanged(bool showAddressLabelLeft)
{
    m_mapData.setShowAddressLabelLeft(showAddressLabelLeft);
    ui->drawingWidget->setMapData(m_mapData);
}

void EditorWindow::handelShowAddressLabelRightChanged(bool showAddressLabelRight)
{
    m_mapData.setShowAddressLabelRight(showAddressLabelRight);
    ui->drawingWidget->setMapData(m_mapData);
}

void EditorWindow::handleShowAddressPrefixChanged(bool showAddressPrefix)
{
    m_mapData.setShowPrefix(showAddressPrefix);
    ui->drawingWidget->setMapData(m_mapData);
}

void EditorWindow::handleShowAddressSuffixChanged(bool showAddressSuffix)
{
    m_mapData.setShowSuffix(showAddressSuffix);
    ui->drawingWidget->setMapData(m_mapData);
}

void EditorWindow::handlePrefixChanged(QString prefix)
{
    m_mapData.setPrefix(prefix);
    ui->drawingWidget->setMapData(m_mapData);
}

void EditorWindow::handleSuffixChanged(QString suffix)
{
    m_mapData.setSuffix(suffix);
    ui->drawingWidget->setMapData(m_mapData);
}

void EditorWindow::handleWidthChanged(int width)
{
    m_mapData.setWidth(width);
    ui->drawingWidget->setMapData(m_mapData);
}

void EditorWindow::handleHeightChanged(int height)
{
    m_mapData.setHeight(height);
    ui->drawingWidget->setMapData(m_mapData);
}

void EditorWindow::updateDrawing()
{
    m_mapData.setMemorySections(m_memoryRangesModel.getMemorySections());
    ui->drawingWidget->setMapData(m_mapData);

}

void EditorWindow::updateMapData()
{
    m_mapData.setMemorySections(m_memoryRangesModel.getMemorySections());
    if(!(m_mapData == m_originalData))
    {
        this->setWindowTitle(this->windowTitle() + "*");
    }

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
//    QString defaultPath = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "", QStandardPaths::LocateDirectory);
//    QString path = QFileDialog::getSaveFileName(this, "Save as ...", defaultPath, tr("Memory Map (*.mmp)"));
    //ui->drawingWidget->exportToSvg(path);
}

void EditorWindow::on_actionOpen_triggered()
{
    QString defaultPath = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "", QStandardPaths::LocateDirectory);
    QString filePath = QFileDialog::getOpenFileName(this, "Open", defaultPath, tr("Memory Map (*.mmp)"));

    QFile loadFile(filePath);

    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QByteArray loadData = loadFile.readAll();

    QJsonDocument loadDocument(QJsonDocument::fromJson(loadData));

    QJsonObject rootObject = loadDocument.object();

    MapData loadedMapData;
    loadedMapData.setName(rootObject["name"].toString());
    loadedMapData.setWidth(rootObject["width"].toInt());
    loadedMapData.setHeight(rootObject["height"].toInt());
    loadedMapData.setPrefix(rootObject["prefix"].toString());
    loadedMapData.setSuffix(rootObject["suffix"].toString());
    loadedMapData.setShowPrefix(rootObject["showPrefix"].toBool());
    loadedMapData.setShowSuffix(rootObject["showSuffix"].toBool());
    loadedMapData.setShowAddressLabelLeft(rootObject["showAddressLabelLeft"].toBool());
    loadedMapData.setShowAddressLabelRight(rootObject["showAddressLabelRight"].toBool());
    loadedMapData.setAddressXOffset(rootObject["addressXOffset"].toInt());
    loadedMapData.setBoxOffsetX(rootObject["boxOffsetX"].toInt());
    loadedMapData.setBoxOffsetY(rootObject["boxOffsetY"].toInt());
    loadedMapData.setFieldWidth(rootObject["fieldWidth"].toInt());
    loadedMapData.setFontSize(rootObject["fontSize"].toInt());
    loadedMapData.setMaxAddress(rootObject["maxAddress"].toInt());

    QList<MemoryRange> loadedSections;
    QJsonArray sectionsArray = rootObject["memorySections"].toArray();

    for (int index = 0; index < sectionsArray.size(); ++index)
    {
        QJsonObject sectionObject = sectionsArray[index].toObject();
        MemoryRange memorySection;
        memorySection.readFromJsonObject(sectionObject);
        loadedSections.append(memorySection);
    }

    loadedMapData.setMemorySections(loadedSections);
    qDebug() << m_mapData.toString();
    m_mapData = loadedMapData;
    m_memoryRangesModel.setMemorySections(m_mapData.memorySections());
    ui->drawingWidget->setMapData(m_mapData);
}

void EditorWindow::on_actionSaveAs_triggered()
{
    QString defaultPath = QStandardPaths::locate(QStandardPaths::DocumentsLocation, "", QStandardPaths::LocateDirectory);
    QString path = QFileDialog::getSaveFileName(this, "Save as ...", defaultPath, tr("Memory Map (*.mmp)"));

    MapData mapData = m_mapData;

    QFile file(path);

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return;
    }

    QJsonObject mapDataObject;
    mapDataObject["name"] = mapData.name();
    mapDataObject["width"] = mapData.width();
    mapDataObject["height"] = mapData.height();
    mapDataObject["showPrefix"] = mapData.showPrefix();
    mapDataObject["showSuffix"] = mapData.showSuffix();
    mapDataObject["prefix"] = mapData.prefix();
    mapDataObject["suffix"] = mapData.suffix();
    mapDataObject["showAddressLabelLeft"] = mapData.showAddressLabelLeft();
    mapDataObject["showAddressLabelRight"] = mapData.showAddressLabelRight();
    mapDataObject["addressXOffset"] = mapData.addressXOffset();
    mapDataObject["boxOffsetX"] = mapData.boxOffset().x();
    mapDataObject["boxOffsetY"] = mapData.boxOffset().y();
    mapDataObject["fieldWidth"] = mapData.fieldWidth();
    mapDataObject["fontSize"] = mapData.fontSize();
    mapDataObject["maxAddress"] = mapData.maxAddress();

    QJsonArray memorySectionsArray;
    foreach (const MemoryRange section, mapData.memorySections()) {
        QJsonObject sectionObject;
        section.writeToJsonObject(sectionObject);
        memorySectionsArray.append(sectionObject);
    }
    mapDataObject["memorySections"] = memorySectionsArray;

    QJsonDocument mapDataDocument(mapDataObject);
    file.write(mapDataDocument.toJson());

    file.close();
}

void EditorWindow::on_actionNew_triggered()
{
    MapData newMapData;
    //newMapData.clearMemorySections();

    m_mapData = newMapData;
    m_memoryRangesModel.setMemorySections(m_mapData.memorySections());
    ui->drawingWidget->setMapData(m_mapData);
}
