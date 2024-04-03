#ifndef MODELS_H
#define MODELS_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QSqlQueryModel>

#include <QItemSelectionModel>

#include <QSqlQuery>
#include <QSqlError>

#include <QStandardItemModel>
#include <QStandardItem>


extern QSqlDatabase db;

namespace Model
{

class Department : public QObject
{
    Q_OBJECT

    //Q_PROPERTY(int id MEMBER m_department_id);

public:

private:
    unsigned int m_department_id;
    QString m_department_name;
};

}

class DepartmentModel : public QSqlTableModel
{
    Q_OBJECT

public:
    DepartmentModel(QObject* parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

class JobModel : public QSqlTableModel
{
    Q_OBJECT

public:
    JobModel(QObject* parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

class ShiftModel : public QSqlTableModel
{
    Q_OBJECT

public:
    ShiftModel(QObject* parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void removeShift(int row);
};

class EmployeeModel : public QSqlRelationalTableModel
{
    Q_OBJECT

public:
    EmployeeModel(QObject* parent = nullptr);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
};

class WorkDayModel : public QStandardItemModel
{
    Q_OBJECT

public:
    WorkDayModel(QObject* parent = nullptr);
    ~WorkDayModel();

    void select(int id);
    void update();

private:
    QSqlQuery* m_query;
    unsigned int m_shift_id = 0;
};

class ShiftDayModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    ShiftDayModel(QObject* parent = nullptr);
    //~ShiftDayModel();

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
};

#endif // MODELS_H
