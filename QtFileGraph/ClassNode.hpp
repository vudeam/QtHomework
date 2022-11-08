#ifndef CLASSNODE_HPP
#define CLASSNODE_HPP

#include <QGraphicsItem>


/* forward declaration */
class Edge;
class GraphWidget;
/***********************/

class ClassNode
    : public QGraphicsItem
{
public:

    using EdgeContainer = QVector<Edge*>;

    ClassNode(GraphWidget* widget);

    void
    addEdge(Edge* edge);

    EdgeContainer
    edges() const;

    enum
    {
        Type = UserType + 1
    };

    int
    type() const override;

    /*
    void
    calculateForces();

    bool
    advancePosition();
    */

    QRectF
    boundingRect() const override;

    QPainterPath
    shape() const override;

    void
    paint(QPainter* painter,
          const QStyleOptionGraphicsItem* option,
          QWidget* widget) override;

protected:

    QVariant
    itemChange(GraphicsItemChange change,
               const QVariant& value) override;

    void
    mousePressEvent(QGraphicsSceneMouseEvent* event) override;

    void
    mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override;

private:

    EdgeContainer m_edgeList;
    QPointF m_newPosition;
    GraphWidget* m_graph;
};

#endif /* CLASSNODE_HPP */
