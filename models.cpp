#include "models.h"

DepartmentModel::DepartmentModel(QObject* parent) : QSqlTableModel(parent, db)
{
    setTable("departments");
    setHeaderData(1, Qt::Horizontal, tr("Department Name"));
    select();
}

QVariant DepartmentModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DecorationRole)
        return QVariant(QIcon(":/icons/assets/delete.svg"));

    return QSqlTableModel::headerData(section, orientation, role);
}

JobModel::JobModel(QObject* parent) : QSqlTableModel(parent, db)
{
    setTable("jobs");
    setHeaderData(1, Qt::Horizontal, tr("Job Title"));
    select();
}

QVariant JobModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DecorationRole)
        return QVariant(QIcon(":/icons/assets/delete.svg"));

    return QSqlTableModel::headerData(section, orientation, role);
}

ShiftModel::ShiftModel(QObject* parent) : QSqlTableModel(parent, db)
{
    setTable("v_shifts");
    setHeaderData(1, Qt::Horizontal, tr("Shift Name"));
    setHeaderData(2, Qt::Horizontal, tr("Start Time"));
    setHeaderData(3, Qt::Horizontal, tr("End Time"));
    setHeaderData(4, Qt::Horizontal, tr("Delay Time"));
    setHeaderData(5, Qt::Horizontal, tr("Early Time"));
    setHeaderData(6, Qt::Horizontal, tr("Shift Days"));
    select();
}

QVariant ShiftModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DecorationRole)
        return QVariant(QIcon(":/icons/assets/delete.svg"));

    return QSqlTableModel::headerData(section, orientation, role);
}

void ShiftModel::removeShift(int row)
{
    const unsigned int shift_id = index(row, 0).data().toInt();

    QSqlQuery query;
    query.prepare("DELETE FROM shifts WHERE shift_id = :ShiftID");
    query.bindValue(":ShiftID", shift_id);
    query.exec();

    removeRow(row);
}

EmployeeModel::EmployeeModel(QObject* parent) : QSqlRelationalTableModel(parent, db)
{
    setTable("employees");

    setRelation(2, QSqlRelation("departments", "department_id", "department_name"));
    setRelation(3, QSqlRelation("jobs", "job_id", "job_title"));
    setRelation(4, QSqlRelation("shifts", "shift_id", "shift_name"));

    setHeaderData(1, Qt::Horizontal, tr("Employee Name"));
    setHeaderData(2, Qt::Horizontal, tr("Department"));
    setHeaderData(3, Qt::Horizontal, tr("Job"));
    setHeaderData(4, Qt::Horizontal, tr("Shift"));
    setHeaderData(5, Qt::Horizontal, tr("Start Date"));
    setHeaderData(6, Qt::Horizontal, tr("Salary"));
    setHeaderData(7, Qt::Horizontal, tr("Employee Fingerprint"));
    select();
}

QVariant EmployeeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DecorationRole)
        return QVariant(QIcon(":/icons/assets/delete.svg"));

    return QSqlRelationalTableModel::headerData(section, orientation, role);
}

ShiftDayModel::ShiftDayModel(QObject* parent) : QSqlQueryModel(parent)
{

}

QVariant ShiftDayModel::data(const QModelIndex& index, int role) const
{
    const int row = index.row();
    const int column = index.column();

    if (role == Qt::CheckStateRole && column == 0)
    {
        const bool is_enabled = index.model()->index(row, 1).data().toBool();

        if (is_enabled)
            return Qt::Checked;

        else
            return Qt::Unchecked;
    }

    return QSqlQueryModel::data(index, role);
}

Qt::ItemFlags ShiftDayModel::flags(const QModelIndex& index) const
{
    return Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
}

WorkDayModel::WorkDayModel(QObject* parent) : QStandardItemModel(parent)
{
    m_query = new QSqlQuery;
    m_query->prepare("SElECT shift_day, is_enabled, shift_id FROM shift_days WHERE shift_id = :ShiftId");

    setRowCount(7);
    setColumnCount(1);
}

WorkDayModel::~WorkDayModel()
{
    delete m_query;
    clear();
}

void WorkDayModel::select(int id)
{
    m_query->bindValue(":ShiftId", id);
    m_shift_id = id;

    if (m_query->exec())
    {
        const int c_shift_day = m_query->record().indexOf("shift_day");
        const int c_is_enabled = m_query->record().indexOf("is_enabled");

        m_query->first();

        for (int row = 0; row < 7; row++)
        {
            const QString day_name = m_query->value(c_shift_day).toString();
            const bool is_enabled = m_query->value(c_is_enabled).toBool();

            QStandardItem* day_name_item = new QStandardItem(day_name);

            day_name_item->setCheckable(true);
            day_name_item->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

            if (is_enabled)
                day_name_item->setCheckState(Qt::Checked);

            else
                day_name_item->setCheckState(Qt::Unchecked);

            setItem(row, 0, day_name_item);

            m_query->next();
        }
    }
}

void WorkDayModel::update()
{
    QSqlQuery query;
    query.prepare("UPDATE shift_days SET is_enabled = :IsEnabled WHERE shift_id = :ShiftId AND shift_day = :ShiftDay");
    query.bindValue(":ShiftId", m_shift_id);

    for (int row = 0; row < 7; row++)
    {
        const QString day_name = index(row, 0).data(Qt::DisplayRole).toString();
        const bool is_enabled = index(row, 0).data(Qt::CheckStateRole).toBool();

        query.bindValue(":ShiftDay", day_name);
        query.bindValue(":IsEnabled", is_enabled);
        query.exec();
    }
}
