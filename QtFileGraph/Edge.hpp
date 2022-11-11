#ifndef EDGE_HPP
#define EDGE_HPP

#include <QGraphicsItem>


/* forward declaration */
class ClassNode;
/***********************/

class Edge
    : public QGraphicsItem
{
public:

    Edge(ClassNode* source, ClassNode* destinaion);

    ClassNode*
    sourceNode() const;

    ClassNode*
    destinationNode() const;

    void
    adjust();

    enum
    {
        Type = UserType + 1
    };

    int
    type() const override;

protected:

    QRectF
    boundingRect() const override;

    void
    paint(QPainter* painter,
          const QStyleOptionGraphicsItem* option,
          QWidget* widget) override;

private:

    static constexpr qreal arrowSize{ 10 };

    ClassNode* m_sourceNode;
    ClassNode* m_destinationNode;
    QPointF m_sourcePoint;
    QPointF m_destinationPoint;
};

#endif /* EDGE_HPP */
