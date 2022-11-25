#include "ClassNode.hpp"
#include "Edge.hpp"
#include "QtFileGraph.hpp"

#include <math.h>

#include <QFileDialog>
#include <QHash>
#include <QKeyEvent>
#include <QMessageBox>
#include <QPair>
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
    setMinimumSize(1300, 730);
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
    case Qt::Key_Plus:
        zoomIn();
        break;
    case Qt::Key_Minus:
        zoomOut();
        break;
    default:
        QGraphicsView::keyPressEvent(event);
    }
}

void
QtFileGraph::timerEvent(QTimerEvent*)
{
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
}

#if QT_CONFIG(wheelevent)
void
QtFileGraph::wheelEvent(QWheelEvent* event)
{
    scaleView(pow(2., -event->angleDelta().y() / 240.0));
}
#endif

void
QtFileGraph::drawBackground(QPainter* painter,
                            const QRectF& rect)
{
    /* shadow */
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

    QHash<QString, QPair<ClassNode*, QStringList>> matches{};
    for (auto it{ rx.globalMatch(contents) }; it.hasNext();)
    {
        auto match{ it.next() };

        auto node{ new ClassNode{this, match.captured("cname")} };
        scene()->addItem(node);

        qDebug() << "Splits for class " << match.captured("cname");
        QStringList parents{};
        for (const auto& p : match.captured("inhspec").split(",", QString::SkipEmptyParts))
        {
            qDebug() << p.split(" ", QString::SkipEmptyParts);
            parents << p.split(" ", QString::SkipEmptyParts).last();
        }

        matches.insert(match.captured("cname"),
                       {node, parents});
    }

    /* create top-level */
    const auto keys{ matches.keys() };
    for (const auto& k : keys)
    {
        const auto parents{ matches[k].second };
        for (const auto& p : parents)
        {
            qDebug() << "Testing parent " << p << " of class " << k;
            if (!matches.contains(p))
            {
                qDebug() << "New top-level " << p;
                auto node{ new ClassNode{this, p} };
                matches.insert(p,
                               {node, {}});
                scene()->addItem(node);
            }
        }
    }

    /* create edges for all nodes */
    for (const auto& key : matches.keys())
    {
        for (const auto& parent : matches[key].second)
        {
            qDebug() << "Connecting " << key << " with parent " << parent;
            scene()->addItem(new Edge{matches[parent].first, matches[key].first});
        }
    }
}
