#include "SpinBoxDelegate.hpp"
#include "Student.hpp"

#include <QSpinBox>


SpinBoxDelegate::SpinBoxDelegate(QObject* parent)
    : QStyledItemDelegate{parent}
{
}

QWidget*
SpinBoxDelegate::createEditor(QWidget* parent,
                              const QStyleOptionViewItem&,
                              const QModelIndex&) const
{
    auto editor{ new QSpinBox{parent} };
    editor->setFrame(false);
    editor->setMinimum(Student::COURSE_MIN);
    editor->setMaximum(Student::COURSE_MAX);

    return editor;
}

void
SpinBoxDelegate::setEditorData(QWidget* editor,
                               const QModelIndex& idx) const
{
    auto modelCourseValue{ idx.model()->data(idx, Qt::EditRole).toInt() };
    auto spinBox{ static_cast<QSpinBox*>(editor) };

    spinBox->setValue(modelCourseValue);
}

void
SpinBoxDelegate::setModelData(QWidget* editor,
                              QAbstractItemModel* model,
                              const QModelIndex& idx) const
{
    auto spinBox{ static_cast<QSpinBox*>(editor) };
    spinBox->interpretText();

    auto editorCourseValue{ spinBox->value() };

    model->setData(idx, editorCourseValue); /* data is set using EditRole by default */
}

void
SpinBoxDelegate::updateEditorGeometry(QWidget* editor,
                                      const QStyleOptionViewItem& opt,
                                      const QModelIndex&) const
{
    editor->setGeometry(opt.rect);
}
