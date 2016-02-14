#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QMainWindow>
#include <QFileDialog>

#include "MemoryRangesModel.h"
#include "MemoryRangeDialog.h"
#include "MapData.h"

namespace Ui {
class EditorWindow;
}

class EditorWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditorWindow(QWidget *parent = 0);
    ~EditorWindow();

private slots:

    void on_addMemoryRange_clicked();

    void on_removeRange_clicked();

    void handleNewMemorySection();

    void handleSizeChange(int newSize);

    void handleExportPngAction();

    void handleExportSvgAction();

    void handleBusWidthChanged();


    void handleShowAddressLabelLeftChanged(bool showAddressLabelLeft);
    void handelShowAddressLabelRightChanged(bool showAddressLabelRight);

    void handleShowAddressPrefixChanged(bool showAddressPrefix);
    void handleShowAddressSuffixChanged(bool showAddressSuffix);

    void handlePrefixChanged(QString prefix);
    void handleSuffixChanged(QString suffix);

    void handleWidthChanged(int width);
    void handleHeightChanged(int height);

    void updateSaveState();

    void updateDrawing();

    void on_actionAbout_triggered();

    void on_actionSave_triggered();

    void on_actionOpen_triggered();

    void on_actionSaveAs_triggered();

    void updateMapData();
    void on_actionNew_triggered();

    void on_actionQuit_triggered();

    void closeEvent(QCloseEvent *event);

private:
    void loadMemoryMap();

    Ui::EditorWindow *ui;
    MemoryRangesModel m_memoryRangesModel;
    MemoryRangeDialog *m_dialog;
    QFileDialog *m_fileDialog;
    MapData m_originalData;
    MapData m_mapData;

};

#endif // __MAINWINDOW_H__
