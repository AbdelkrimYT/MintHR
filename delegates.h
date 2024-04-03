#ifndef DELEGATES_H
#define DELEGATES_H

#include "models.h"

#include <QObject>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QSqlRelationalDelegate>

#include <QWidget>
#include <QPainter>
#include <QSize>
#include <QCheckBox>
#include <QListView>
#include <QMessageBox>

class ShiftDayDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ShiftDayDelegate(QObject* parent = nullptr);
    ~ShiftDayDelegate();

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override;
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override;

private:
    QListView* dayList;
    WorkDayModel* m_model;
};

#endif // DELEGATES_H
