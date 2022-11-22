#include "ClassNode.hpp"
#include "Edge.hpp"

#include <QPainter>
#include <QtMath>


Edge::Edge(ClassNode* src, ClassNode* dst)
    : m_sourceNode{src}
    , m_destinationNode{dst}
{
    setAcceptedMouseButtons(Qt::NoButton);

    m_sourceNode->addEdge(this);
    m_destinationNode->addEdge(this);

    adjust();
}

ClassNode*
Edge::sourceNode() const
{
    return m_sourceNode;
}

ClassNode*
Edge::destinationNode() const
{
    return m_destinationNode;
}

void
Edge::adjust()
{
    if (!m_sourceNode || !m_destinationNode)
    {
        return;
    }

    QLineF line
    {
        mapFromItem(m_sourceNode, 0, 0),
        mapFromItem(m_destinationNode, 0, 0)
    };

    auto len{ line.length() };

    prepareGeometryChange();

    if (len > qreal{20.})
    {
        QPointF edgeOffset
        {
            (line.dx() * 10) / len,
            (line.dy() * 10) / len
        };

        m_sourcePoint = line.p1() + edgeOffset;
        m_destinationPoint = line.p2() - edgeOffset;
    }
    else
    {
        m_sourcePoint = m_destinationPoint = line.p1();
    }
}

int
Edge::type() const
{
    return Type;
}

QRectF
Edge::boundingRect() const
{
    if (!m_sourceNode || !m_destinationNode)
    {
        return QRectF{};
    }

    qreal penWidth{ 1 };
    qreal extra{ (penWidth + arrowSize) / 2. };

    return QRectF
        {
            m_sourcePoint,
            QSizeF{m_destinationPoint.x() - m_sourcePoint.x(),
                   m_destinationPoint.y() - m_sourcePoint.y()}
        }
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void
Edge::paint(QPainter* painter,
            const QStyleOptionGraphicsItem*,
            QWidget*)
{
    if (!m_sourceNode || !m_destinationNode)
    {
        return;
    }

    QLineF line{ m_sourcePoint, m_destinationPoint };

    if (qFuzzyCompare(line.length(), qreal{0.}))
    {
        return;
    }

    /* draw line */
    painter->setPen(QPen{Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin});
    painter->drawLine(line);

    /* draw arrows */
    auto angle{ std::atan2(-line.dy(), line.dx()) };

    auto destArrowP1{ m_destinationPoint + QPointF{sin(angle - M_PI / 3) * arrowSize,
                                                   cos(angle - M_PI / 3) * arrowSize}};

    auto destArrowP2{ m_destinationPoint + QPointF{sin(angle - M_PI + M_PI / 3) * arrowSize,
                                                   cos(angle - M_PI + M_PI / 3) * arrowSize}};

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF{} << line.p2() << destArrowP1 << destArrowP2);
}
