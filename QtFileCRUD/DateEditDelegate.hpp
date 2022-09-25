#ifndef DATEEDITDELEGATE_HPP
#define DATEEDITDELEGATE_HPP

#include <QStyledItemDelegate>


/*!
 * \brief Provides QDateEdit fot Student's fields containing date
 */
class DateEditDelegate
    : public QStyledItemDelegate
{
    Q_OBJECT

public:

    DateEditDelegate(QObject* parent = nullptr);

    /*!
     * \brief Provide editor for Student's enroll date
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

#endif /* DATEEDITDELEGATE_HPP */
