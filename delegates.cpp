#include "delegates.h"

ShiftDayDelegate::ShiftDayDelegate(QObject* parent) : QStyledItemDelegate(parent)
{
    m_model = new WorkDayModel(this);
}

ShiftDayDelegate::~ShiftDayDelegate()
{
    delete m_model;
}

QWidget* ShiftDayDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QListView* workDayList = new QListView(parent);
    workDayList->setModel(m_model);
    workDayList->setFrameShape(QFrame::NoFrame);
    //workDayList->setMinimumHeight(150);
    //workDayList->setAlternatingRowColors(true);
    return workDayList;
}

void ShiftDayDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    const int row = index.row();
    const int shift_id = index.model()->index(row, 0).data().toInt();

    m_model->select(shift_id);
}

void ShiftDayDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    m_model->update();
    static_cast<QSqlTableModel*>(model)->select();
}

void ShiftDayDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    //editor->setGeometry(option.rect);
    editor->setGeometry(editor->parentWidget()->parentWidget()->parentWidget()->geometry());
}
