#ifndef QTFILEGRAPH_HPP
#define QTFILEGRAPH_HPP

#include <QGraphicsView>


/* forward declaration */
class ClassNode;
/***********************/

class QtFileGraph
    : public QGraphicsView
{
    Q_OBJECT

public:

    QtFileGraph(QWidget* parent = nullptr);

    void
    itemMoved();

public slots:

    void
    zoomIn();

    void
    zoomOut();

protected:

    void
    keyPressEvent(QKeyEvent* event) override;

    void
    timerEvent(QTimerEvent* event) override;

#if QT_CONFIG(wheelevent)
    void
    wheelEvent(QWheelEvent* event) override;
#endif

    void
    drawBackground(QPainter* painter,
                   const QRectF& rect) override;

    void
    scaleView(qreal scaleFactor);

private:

    int m_timerId;
    ClassNode* m_centerNode;
};
#endif /* QTFILEGRAPH_HPP */
