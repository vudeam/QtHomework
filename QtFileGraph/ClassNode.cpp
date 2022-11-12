#include "ClassNode.hpp"
#include "Edge.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>


ClassNode::ClassNode(QtFileGraph* graphWidget)
    : m_graph{graphWidget}
{
    setFlags(ItemIsMovable | ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
}

void
ClassNode::addEdge(Edge* edge)
{
    m_edgeList.append(edge);
    edge->adjust();
}

ClassNode::EdgeContainer
ClassNode::edges() const
{
    return m_edgeList;
}

int
ClassNode::type() const
{
    return Type;
}

void
ClassNode::calculateForces()
{
    if (!scene() || scene()->mouseGrabberItem() == this)
    {
        m_newPosition = pos();

        return;
    }

    /* sum up all forces pushing this item away */
    qreal xvel { 0 };
    qreal yvel { 0 };

    for (auto item : scene()->items())
    {
        auto node{ qgraphicsitem_cast<ClassNode*>(item) };

        if (!node)
        {
            continue;
        }

        auto vec{ mapToItem(node, 0, 0) };
        auto dx{ vec.x() };
        auto dy{ vec.y() };

        auto l { 2. * (dx * dx + dy * dy) };

        if (l > 0)
        {
            xvel += (dx * 150.0) / l;
            yvel += (dy * 150.0) / l;
        }
    }

    /* Now subtract all forces pulling items together */
    auto weight{ (m_edgeList.size() + 1) * 10 };

    for (const auto edge : qAsConst(m_edgeList))
    {
        QPointF vec{};
        if (edge->sourceNode() == this)
        {
            vec = mapToItem(edge->destinationNode(), 0, 0);
        }
        else
        {
            vec = mapToItem(edge->sourceNode(), 0, 0);
        }

        xvel -= vec.x() / weight;
        yvel -= vec.y() / weight;
    }

    if (qAbs(xvel) < .1 && qAbs(yvel) < .1)
    {
        xvel = yvel = 0;
    }

    auto sceneRect{ scene()->sceneRect() };

    m_newPosition = pos() + QPointF{ xvel, yvel };
    m_newPosition.setX(qMin(
                            qMax(m_newPosition.x(), sceneRect.left() + 10),
                            sceneRect.right() - 10));
    m_newPosition.setY(qMin(
                            qMax(m_newPosition.y(), sceneRect.top() + 10),
                            sceneRect.bottom() - 10));
}

bool
ClassNode::advancePosition()
{
    if (m_newPosition == pos())
    {
        return false;
    }

    setPos(m_newPosition);

    return true;
}

QRectF
ClassNode::boundingRect() const
{
    qreal adj{ 2 };

    /* mAgIc */
    return QRectF
    {
        -10 - adj,
        -10 - adj,
        23 + adj,
        23 + adj
    };
}

QPainterPath
ClassNode::shape() const
{
    QPainterPath path{};
    path.addRect(-10, -10, 20, 20);

    return path;
}

void
ClassNode::paint(QPainter* painter,
                 const QStyleOptionGraphicsItem* opt,
                 QWidget*)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::darkGray); // TODO: change colors?
    painter->drawRect(-7, -7, 20, 20);

    QRadialGradient grad{ -3, -3, 10 };
    if (QStyle::State_Sunken & opt->state)
    {
        grad.setCenter(3, 3);
        grad.setFocalPoint(3, 3);
        grad.setColorAt(1, QColor{Qt::yellow}.lighter(120));
        grad.setColorAt(0, QColor{Qt::darkYellow}.lighter(120));
    }
    else
    {
        grad.setColorAt(0, Qt::yellow);
        grad.setColorAt(1, Qt::darkYellow);
    }

    painter->setBrush(grad);

    painter->setPen(QPen{Qt::black, 0});
    painter->drawRect(-10, -10, 20, 20);
}

QVariant
ClassNode::itemChange(QGraphicsItem::GraphicsItemChange chg,
                      const QVariant& val)
{
    using GraphChange = QGraphicsItem::GraphicsItemChange;
    switch (chg)
    {
    case GraphChange::ItemPositionHasChanged:

        for (auto edge : qAsConst(m_edgeList))
        {
            edge->adjust();
        }

        // m_graph->itemMoved(); TODO
        break;

    default: break;
    }

    return QGraphicsItem::itemChange(chg, val);
}

void
ClassNode::mousePressEvent(QGraphicsSceneMouseEvent* evt)
{
    update();

    QGraphicsItem::mousePressEvent(evt);
}

void
ClassNode::mouseReleaseEvent(QGraphicsSceneMouseEvent* evt)
{
    update();

    QGraphicsItem::mouseReleaseEvent(evt);
}
