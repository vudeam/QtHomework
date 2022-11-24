#include "ClassNode.hpp"
#include "Edge.hpp"
#include "QtFileGraph.hpp"

#include <math.h>

#include <QFileDialog>
#include <QHash>
#include <QKeyEvent>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QTextStream>

#include <QDebug> // DELETE


static const QString classPattern
{
    R"RE(\b(class|struct)\s+(?<cname>[A-Za-z_]\w*)\s*:?(?<inhspec>\s*(public|private|protected)?\s*([A-Za-z_]+\w*)(,\s*(public\s|private\s|protected\s)?\s*([A-Za-z_]+\w*))*)?\s*{(?Us:.*)}\s*;)RE"
};

QtFileGraph::QtFileGraph(QWidget* parent)
    : QGraphicsView{parent}
    , m_timerId{0}
{
    auto scene = new QGraphicsScene(this);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    scene->setSceneRect(-640, -480, 1280, 720);
    setScene(scene);
    setCacheMode(CacheBackground);
    setViewportUpdateMode(BoundingRectViewportUpdate);
    setRenderHint(QPainter::Antialiasing);
    setTransformationAnchor(AnchorUnderMouse);
    // scale(qreal{.8}, qreal{.8});
    setMinimumSize(1300, 730);

    m_centerNode = new ClassNode{ this };

    auto node1 = new ClassNode{ this };
    auto node2 = new ClassNode{ this };

    m_centerNode->setCname("Parent" "\n0123456789");
    node1->setCname("Child1" "\n0123456789");
    node2->setCname("Child2" "\n0123456789");

    scene->addItem(m_centerNode);
    scene->addItem(node1);
    scene->addItem(node2);
    scene->addItem(new Edge{ m_centerNode, node1 });
    scene->addItem(new Edge{ m_centerNode, node2 });

    m_centerNode->setPos(0, 0);
    node1->setPos(0, 50);
}

void
QtFileGraph::itemMoved()
{
    if (!m_timerId)
    {
        m_timerId = startTimer(1000 / 250);
    }
}

void
QtFileGraph::keyPressEvent(QKeyEvent* event)
{
    switch (event->key())
    {
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
    /*
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
        }
    }

    if (!itemsMoved)
    {
        killTimer(m_timerId);
        m_timerId = 0;
    }
    */
    QList<ClassNode*> nodes{};

    for (auto item : scene()->items())
    {
        if (auto node = qgraphicsitem_cast<ClassNode*>(item))
        {
            // nodes << node;
            nodes.append(node);
        }
    }

    for (auto n : qAsConst(nodes))
    {
        n->calculateForces();
    }

    auto itemsMoved{ false };
    for (auto n : nodes)
    {
        if (n->advancePosition())
        {
            itemsMoved = true;
        }
    }

    if (!itemsMoved)
    {
        killTimer(m_timerId);
        m_timerId = 0;
    }


    /*
    QList<ClassNode *> nodes;
        foreach (QGraphicsItem *item, scene()->items()) {
            if (ClassNode *node = qgraphicsitem_cast<ClassNode *>(item))
                nodes << node;
        }

        foreach (ClassNode *node, nodes)
            node->calculateForces();

        bool itemsMoved = false;
        foreach (ClassNode *node, nodes) {
            if (node->advancePosition())
                itemsMoved = true;
        }

        if (!itemsMoved) {
            killTimer(m_timerId);
            m_timerId = 0;
        }
        */

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

    /* fill */
    QLinearGradient gradient{sceneRect.topLeft(), sceneRect.bottomRight()};
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1, Qt::lightGray);
    painter->fillRect(rect.intersected(sceneRect), gradient);
    painter->setBrush(Qt::NoBrush);
    painter->drawRect(sceneRect);
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

void
QtFileGraph::openFile()
{
    auto fileName
    {
        QFileDialog::getOpenFileName(this,
                                     tr("Open Header"),
                                     QDir::currentPath(),
                                     "Header (*.h *.H *.hh *.HH *.hpp *.HPP *.hxx *.HXX *.h++ *.H++)")
    };

    if (fileName.isEmpty())
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("No file selected"));

        return;
    }

    QRegularExpression rx
    {
        classPattern,
        QRegularExpression::MultilineOption
    };

    QFile inFile{ fileName };
    if (!inFile.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::critical(this,
                              tr("Error"),
                              tr("Cannot read file"));

        return;
    }

    QTextStream inStream{ &inFile };
    const auto contents{ inStream.readAll() };
    inFile.close();

    /* reset scene */
    scene()->clear();

    QFileInfo info{ fileName };
    m_centerNode = new ClassNode{ this, info.fileName() };
    scene()->addItem(m_centerNode);

    QHash<QString, ClassNode*> matches{};
    for (auto it{ rx.globalMatch(contents) }; it.hasNext();)
    {
        auto match{ it.next() };
        qDebug() << match.captured("inhspec");

        auto node{ new ClassNode{this, match.captured("cname")} };

        matches.insert(match.captured("cname"), node);
        scene()->addItem(node);
    }
}
