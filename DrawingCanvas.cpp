#include "DrawingCanvas.h"
#include <qdebug.h>
#include <QString>
#include <QFileInfo>
#include <QCoreApplication>

DrawingCanvas::DrawingCanvas(QWidget *parent) :
    QWidget(parent)
{
}

DrawingCanvas::~DrawingCanvas()
{

}

void DrawingCanvas::paint(QPainter &painter)
{
    QFont myFont("Arial", m_mapData.fontSize());
    QString str(QString("%1%2%3").arg(getPrefix()).arg(0, m_mapData.fieldWidth(), 16, QChar('0')).arg(getSuffix()));

    QFontMetrics fm(myFont);
    if(m_mapData.showAddressLabelLeft())
    {
        m_mapData.setBoxOffsetX(m_mapData.addressXOffset() + fm.width(str) + 10 );
    }
    else
    {
        m_mapData.setBoxOffsetX(m_mapData.addressXOffset() + 10);
    }


    painter.drawRect(QRect(m_mapData.boxOffset().x(), m_mapData.boxOffset().y(), m_mapData.width(), m_mapData.height()));
    painter.setFont(QFont("Arial", m_mapData.fontSize()));
    if(m_mapData.showAddressLabelLeft())
    {
        painter.drawText(QPoint(m_mapData.addressXOffset(), m_mapData.boxOffset().y()+m_mapData.fontSize()),
                         QString("%1%2%3").arg(getPrefix()).arg(0, m_mapData.fieldWidth(), 16, QChar('0')).arg(getSuffix()));
        painter.drawText(QPoint(m_mapData.addressXOffset(), m_mapData.height()+m_mapData.boxOffset().y()-m_mapData.fontSize()/2),
                         QString("%1%2%3").arg(getPrefix()).arg(m_mapData.maxAddress(), m_mapData.fieldWidth(), 16, QChar('0')).arg(getSuffix()));
    }
    if(m_mapData.showAddressLabelRight())
    {
        painter.drawText(QPoint(m_mapData.addressXOffset() + m_mapData.boxOffset().x()+ m_mapData.width(), m_mapData.boxOffset().y()+m_mapData.fontSize()),
                         QString("%1%2%3").arg(getPrefix()).arg(0, m_mapData.fieldWidth(), 16, QChar('0')).arg(getSuffix()));
        painter.drawText(QPoint(m_mapData.addressXOffset() + m_mapData.boxOffset().x()+ m_mapData.width(), m_mapData.height()+m_mapData.boxOffset().y()-m_mapData.fontSize()/2),
                         QString("%1%2%3").arg(getPrefix()).arg(m_mapData.maxAddress(), m_mapData.fieldWidth(), 16, QChar('0')).arg(getSuffix()));
    }

    m_edges.clear();
    m_edges.append(0);
    m_edges.append(m_mapData.maxAddress());

    foreach(MemoryRange memorySection, m_mapData.memorySections())
    {
        drawMemorySection(painter, memorySection.lowerLimit(), memorySection.upperLimit(), memorySection.name());
    }
}

QString DrawingCanvas::getPrefix()
{
    if (m_mapData.showPrefix())
        return m_mapData.prefix();
    else
        return QString();
}

QString DrawingCanvas::getSuffix()
{
    if (m_mapData.showSuffix())
        return m_mapData.suffix();
    else
        return QString();
}

QSize DrawingCanvas::calculateCanvasSize()
{
    QSize canvasSize;

    QFont myFont("Arial", m_mapData.fontSize());
    QString str(QString("%1%2%3").arg(getPrefix()).arg(0, m_mapData.fieldWidth(), 16, QChar('0')).arg(getSuffix()));
    QFontMetrics fm(myFont);

    int width = m_mapData.width()+m_mapData.boxOffset().x()+20;

    if(m_mapData.showAddressLabelRight())
    {
        width = width + fm.width(str) + 30;
    }

    canvasSize.setWidth(width);
    canvasSize.setHeight(m_mapData.height()+2*m_mapData.boxOffset().y());

    return canvasSize;
}

void DrawingCanvas::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    paint(painter);
}

void DrawingCanvas::setMapData(MapData &mapData)
{
    m_mapData = mapData;
    update();
}

void DrawingCanvas::drawMemorySection(QPainter &painter, float startRange, float endRange, QString name)
{
    int heightOfTopLine = startRange/m_mapData.maxAddress()*m_mapData.height()+m_mapData.boxOffset().y();
    int heightOfBottomLine = (endRange+1)/m_mapData.maxAddress()*m_mapData.height()+m_mapData.boxOffset().y();
    QPoint topLeftCorner(m_mapData.boxOffset().x(), heightOfTopLine);
    QPoint topRightCorner(m_mapData.boxOffset().x()+m_mapData.width()-1, heightOfTopLine);
    QPoint bottomLeftCorner(m_mapData.boxOffset().x(), heightOfBottomLine-1);
    QPoint bottomRightCorner(m_mapData.boxOffset().x()+m_mapData.width()-1, heightOfBottomLine-1);

    // draw address at the beginning of the range
    if (startRange != 0)
    {
        QString address = QString("%1%2%3").arg(getPrefix()).arg((int)startRange, m_mapData.fieldWidth(), 16, QChar('0')).arg(getSuffix());

        if(m_mapData.showAddressLabelLeft())
        {
            painter.drawText(QPoint(m_mapData.addressXOffset(), heightOfTopLine+m_mapData.fontSize()), address);
        }

        if(m_mapData.showAddressLabelRight())
        {
            painter.drawText(QPoint(m_mapData.addressXOffset() + m_mapData.boxOffset().x()+ m_mapData.width(), heightOfTopLine+m_mapData.fontSize()), address);
        }
    }

    if (!m_edges.contains(startRange) && !m_edges.contains(startRange-1))
    {
        m_edges.append(startRange);
        painter.drawLine(topLeftCorner, topRightCorner);
    }

    // draw address at the end of the range
    if (endRange != m_mapData.maxAddress())
    {
        QString address = QString("%1%2%3")
                .arg(getPrefix())
                .arg((int)endRange, m_mapData.fieldWidth(), 16, QChar('0'))
                .arg(getSuffix());

        if(m_mapData.showAddressLabelLeft())
        {
            painter.drawText(QPoint(m_mapData.addressXOffset(), heightOfBottomLine-m_mapData.fontSize()/2), address);
        }

        if(m_mapData.showAddressLabelRight())
        {
             painter.drawText(QPoint(m_mapData.addressXOffset() + m_mapData.boxOffset().x() + m_mapData.width(),
                                     heightOfBottomLine-m_mapData.fontSize()/2), address);
        }
    }

    if (!m_edges.contains(endRange))
    {
        m_edges.append(endRange);
        painter.drawLine(bottomLeftCorner, bottomRightCorner);
    }

    QRect section(topLeftCorner, bottomRightCorner);

    painter.drawText(section, Qt::AlignCenter, name);

}

void DrawingCanvas::exportToSvg(QString path)
{
    if (path.isEmpty())
        return;

    QSvgGenerator generator;
    generator.setFileName(path);
    QSize canvasSize = calculateCanvasSize();
    generator.setSize(canvasSize);
    generator.setViewBox(QRect(QPoint(0, 0), canvasSize));
    generator.setTitle(m_mapData.name());
    generator.setDescription(QString("Diagramm created with %1").arg(QFileInfo( QCoreApplication::applicationFilePath() ).fileName()));
    QPainter painter;
    painter.begin(&generator);
    paint(painter);
    painter.end();
}

void DrawingCanvas::exportToPng(QString path)
{
    if (path.isEmpty())
        return;

    QImage img(calculateCanvasSize(),
               QImage::Format_ARGB8565_Premultiplied);
    img.fill(QColor(255, 255, 255));

    QPainter painter;
    painter.begin(&img);
    paint(painter);
    painter.end();

    img.save(path);
}


