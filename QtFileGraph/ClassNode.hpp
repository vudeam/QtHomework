#ifndef CLASSNODE_HPP
#define CLASSNODE_HPP

#include <QGraphicsItem>
#include <QVector>


/* forward declaration */
class Edge;
class QtFileGraph;
/***********************/

class ClassNode
    : public QGraphicsItem
{
public:

    using EdgeContainer = QVector<Edge*>;

    ClassNode(QtFileGraph* widget, const QString& cname = QString{});

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

    void
    calculateForces();

    bool
    advancePosition();

    QRectF
    boundingRect() const override;

    QPainterPath
    shape() const override;

    void
    paint(QPainter* painter,
          const QStyleOptionGraphicsItem* option,
          QWidget* widget) override;

    void
    setCname(const QString& cname);

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
    QtFileGraph* m_graph;
    QString m_cname;
};

#endif /* CLASSNODE_HPP */
