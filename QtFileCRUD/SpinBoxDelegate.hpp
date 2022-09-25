#ifndef SPINBOXDELEGATE_HPP
#define SPINBOXDELEGATE_HPP

#include <QStyledItemDelegate>


/*!
 * \brief Provides spin box editor for Student's fields (i.e. course)
 *
 * \sa QSPinBox, Student
 */
class SpinBoxDelegate
    : public QStyledItemDelegate
{
    Q_OBJECT

public:

    SpinBoxDelegate(QObject* parent = nullptr);

    /*!
     * \brief Provide editor for Student's course
     */
    QWidget*
    createEditor(QWidget* parent,
                 const QStyleOptionViewItem& option,
                 const QModelIndex& index) const override;

    void
    setEditorData(QWidget* editor,
                  const QModelIndex& index) const override;

    void
    setModelData(QWidget* editor,
                 QAbstractItemModel* model,
                 const QModelIndex& index) const override;

    void
    updateEditorGeometry(QWidget* editor,
                         const QStyleOptionViewItem& option,
                         const QModelIndex& index) const override;
};

#endif /* SPINBOXDELEGATE_HPP */
