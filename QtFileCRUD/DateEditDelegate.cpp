#include "DateEditDelegate.hpp"
#include "StudentModel.hpp"

#include <QDateEdit>


DateEditDelegate::DateEditDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
}

QWidget*
DateEditDelegate::createEditor(QWidget* parent,
                               const QStyleOptionViewItem&,
                               const QModelIndex&) const
{
    auto editor{ new QDateEdit{parent} };
    editor->setFrame(false);
    editor->setDisplayFormat(JSON_DATE_FORMAT);

    return editor;
}

void
DateEditDelegate::setEditorData(QWidget* editor,
                                const QModelIndex& idx) const
{
    auto modelEnrollValue{ idx.model()->data(idx, Qt::EditRole).toDate() };
    auto dateEdit{ static_cast<QDateEdit*>(editor) };

    dateEdit->setDate(modelEnrollValue);
}

void
DateEditDelegate::setModelData(QWidget* editor,
                               QAbstractItemModel* model,
                               const QModelIndex& idx) const
{
    auto dateEdit{ static_cast<QDateEdit*>(editor) };
    dateEdit->interpretText();

    auto editorEnrollValue{ dateEdit->date() };

    model->setData(idx, editorEnrollValue); /* data is set using EditRole by default */
}

void
DateEditDelegate::updateEditorGeometry(QWidget* editor,
                                       const QStyleOptionViewItem& opt,
                                       const QModelIndex&) const
{
    editor->setGeometry(opt.rect);
}
