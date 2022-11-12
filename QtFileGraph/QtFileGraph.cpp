#include "ClassNode.hpp"
#include "Edge.hpp"
#include "QtFileGraph.hpp"

#include <math.h>

#include <QKeyEvent>
#include <QRandomGenerator>


QtFileGraph::QtFileGraph(QWidget* parent)
    : QGraphicsView{parent}
{
    auto scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-200, -200, 400, 400);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    scale(qreal{0.8}, qreal{0.8});
    setMinimumSize(400, 400);
    setWindowTitle(tr("Header Graph"));

    m_centerNode = new ClassNode(this);

    auto node1 = new ClassNode{this},
         node2 = new ClassNode{this};

    scene->addItem(node1);
    scene->addItem(node2);
    scene->addItem(new Edge{ node1, node2 });
    scene->addItem(new Edge{m_centerNode, node1});

    node1->setPos(-50, 50);
    node2->setPos(0, -50);
    m_centerNode->setPos(0, 0);
}

void
QtFileGraph::itemMoved()
{
    if (!m_timerId)
    {
        m_timerId = startTimer(1000 / 25);
    }
}

void
QtFileGraph::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Up:
        m_centerNode->moveBy(0, -20);
        break;
    case Qt::Key_Down:
        m_centerNode->moveBy(0, 20);
        break;
    case Qt::Key_Left:
        m_centerNode->moveBy(-20, 0);
        break;
    case Qt::Key_Right:
        m_centerNode->moveBy(20, 0);
        break;
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    /*
    case Qt::Key_Space:
    case Qt::Key_Enter:
        m_shuffle();
        break;
    */
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void
QtFileGraph::timerEvent(QTimerEvent*)
{
    QVector<ClassNode*> nodes{};

    for (auto item : scene()->items())
    {
        if (auto node = qgraphicsitem_cast<ClassNode*>(item))
        {
            nodes.append(node);
        }
    }

    for (auto node : qAsConst(nodes))
    {
        node->calculateForces();
    }

    bool itemsMoved{ false };
    for (auto node : qAsConst(nodes))
    {
        if (node->advancePosition())
        {
            itemsMoved = true;
            break; /* ? */
        }
    }

    if (!itemsMoved)
    {
        killTimer(m_timerId);
        m_timerId = 0;
    }

}

#if QT_CONFIG(wheelevent)
void
QtFileGraph::wheelEvent(QWheelEvent* event)
{
    scaleView(pow(2., -event->angleDelta().y() / 240.0));
    // scaleView(pow(2., -event->delta() / 240.));
}
#endif

void
QtFileGraph::drawBackground(QPainter* painter,
                            const QRectF& rect)
{
    // Shadow
    auto sceneRect = this->sceneRect();

    QRectF rightShadow{sceneRect.right(),
                       sceneRect.top() + 5, 5,
                       sceneRect.height()};

    QRectF bottomShadow{sceneRect.left() + 5,
                        sceneRect.bottom(),
                        sceneRect.width(), 5};

    if (rightShadow.intersects(rect) || rightShadow.contains(rect))
    {
        painter->fillRect(rightShadow, Qt::darkGray);
    }

    if (bottomShadow.intersects(rect) || bottomShadow.contains(rect))
    {
        painter->fillRect(bottomShadow, Qt::darkGray);
    }

    // Fill
    QLinearGradient gradient{sceneRect.topLeft(), sceneRect.bottomRight()};
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);

    // Text
    QRectF textRect{sceneRect.left() + 4,
                    sceneRect.top() + 4,
                    sceneRect.width() - 4,
                    sceneRect.height() - 4};
    QString message{tr("Click and drag the nodes around, and zoom with the mouse "
                       "wheel or the '+' and '-' keys")};

    auto font = painter->font();
    font.setBold(true);
    font.setPointSize(14);
    painter->setFont(font);
    painter->setPen(Qt::lightGray);
    painter->drawText(textRect.translated(2, 2), message);
    painter->setPen(Qt::black);
    painter->drawText(textRect, message);
}

void
QtFileGraph::scaleView(qreal scaleFactor)
{
    auto factor{transform()
                .scale(scaleFactor, scaleFactor)
                 .mapRect(QRectF(0, 0, 1, 1))
                 .width()};

    if (factor < .07 || factor > 100)
    {
        return;
    }

    scale(scaleFactor, scaleFactor);
}

/*
void
QtFileGraph::shuffle()
{
    for (auto item : scene()->items())
        if (qgraphicsitem_cast<ClassNode*>(item))
        {
            item->setPos(-150 + QRandomGenerator::global()->bounded(300),
                         -150 + QRandomGenerator::global()->bounded(300));
        }
    }
}
*/

void
QtFileGraph::zoomIn()
{
    scaleView(qreal(1.2));
}

void
QtFileGraph::zoomOut()
{
    scaleView(1 / qreal(1.2));
}
