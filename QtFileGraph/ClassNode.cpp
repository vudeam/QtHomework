#include "ClassNode.hpp"

#include <QPainter>
#include <QRadialGradient>
#include <QStyle>
#include <QStyleOptionGraphicsItem>


ClassNode::ClassNode(GraphWidget* graphWidget)
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
    // edge->adjust(); TODO: declare Edge class
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

        // for (auto edge : qAsConst(m_edgeList)) edge->adjust(); TODO: declare Edge class

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
