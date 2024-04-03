#ifndef WINDOWS_H
#define WINDOWS_H

#include "models.h"
#include "delegates.h"

#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QWidget>
#include <QTabWidget>
#include <QTableView>
#include <QFrame>
#include <QHeaderView>
#include <QIcon>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QTimeEdit>
#include <QDataWidgetMapper>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSpacerItem>
#include <QDialog>
#include <QMessageBox>
#include <QSplashScreen>
#include <QListWidget>
#include <QVariant>

QT_BEGIN_NAMESPACE

class MainWindow;
class DepartmentWidget;
class JobWidget;
class ShiftWidget;
class EmployeeWidget;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void createNewDep();
    void createNewJob();

    void createNewShift();
    void deleteShift(int row);

    void createNewEmploye();
    void deleteEmploye(int row);

private:
    QAction* actionExit;
    QAction* actionDep;
    QAction* actionJob;
    QAction* actionEmploye;

    QMenuBar* mainMenubar;
    QStatusBar* mainStatusBar;

    QMenu* menuFile;
    QMenu* menuEdit;
    QMenu* menuHelp;

    QTabWidget* mainWidget;

    DepartmentModel* depModel;
    JobModel* jobModel;
    ShiftModel* shfModel;
    EmployeeModel* empModel;

    DepartmentWidget* depWidget;
    JobWidget* jobWidget;
    ShiftWidget* shfWidget;
    EmployeeWidget* empWidget;

    QLineEdit* searchEdit;
    QToolButton* zkButton;
};


class DepartmentWidget : public QTableView
{
    Q_OBJECT

public:
    DepartmentWidget(QWidget* parent = nullptr, DepartmentModel* model = nullptr);
    ~DepartmentWidget();

private:
    DepartmentModel* m_model;

private slots:
    void rowDelete(int row);
};


class JobWidget : public QTableView
{
    Q_OBJECT

public:
    JobWidget(QWidget* parent = nullptr, JobModel* model = nullptr);
    ~JobWidget();

private:
    JobModel* m_model;

private slots:
    void rowDelete(int row);
};


class ShiftWidget : public QWidget
{
    Q_OBJECT

public:
    ShiftWidget(QWidget* parent = nullptr, ShiftModel* model = nullptr);
    ~ShiftWidget();

private:
    QTableView* table;
    ShiftModel* m_model;
    ShiftDayDelegate* dayDelegate;
    QDataWidgetMapper* mapper;

    QVBoxLayout* mainLayout;
    QTabWidget* mainBoard;

    QScrollArea* pgEdit;
    QWidget* pgEditWidget;
    QVBoxLayout* pgEditLayout;
    QLabel* lbl_e1;
    QLabel* lbl_e2;
    QLabel* lbl_e3;
    QLabel* lbl_e4;
    QLabel* lbl_e5;
    QLabel* lbl_e6;
    QSpacerItem* spx_e7;
    QHBoxLayout* hbl_e1;
    QHBoxLayout* hbl_e2;
    QHBoxLayout* hbl_e3;
    QHBoxLayout* hbl_e4;
    QHBoxLayout* hbl_e5;
    QHBoxLayout* hbl_e6;
    QHBoxLayout* hbl_e7;
    QLineEdit* shf_e1;
    QTimeEdit* shf_e2;
    QTimeEdit* shf_e3;
    QTimeEdit* shf_e4;
    QTimeEdit* shf_e5;
    QListView* shf_e6;
    QPushButton* btn_e7;

    QScrollArea* pgCreate;
    QWidget* pgCreateWidget;
    QVBoxLayout* pgCreateLayout;
    QLabel* lbl_c1;
    QLabel* lbl_c2;
    QLabel* lbl_c3;
    QLabel* lbl_c4;
    QLabel* lbl_c5;
    QLabel* lbl_c6;
    QSpacerItem* spx_c7;
    QHBoxLayout* hbl_c1;
    QHBoxLayout* hbl_c2;
    QHBoxLayout* hbl_c3;
    QHBoxLayout* hbl_c4;
    QHBoxLayout* hbl_c5;
    QHBoxLayout* hbl_c6;
    QHBoxLayout* hbl_c7;
    QLineEdit* shf_c1;
    QTimeEdit* shf_c2;
    QTimeEdit* shf_c3;
    QTimeEdit* shf_c4;
    QTimeEdit* shf_c5;
    QListWidget* shf_c6;
    QPushButton* btn_c7;

    QTableView* pgEmp;
    QSqlQueryModel* m_emp_model;
    QSqlQuery* empQuery;
    WorkDayModel* m_shfd_model;

private slots:
    void rowDelete(int row);
    void newShift();
    void editShift();
    void nextShift(const QModelIndex &current, const QModelIndex &previous);
};


class EmployeeWidget : public QWidget
{
    Q_OBJECT

public:
    EmployeeWidget(QWidget* parent = nullptr, EmployeeModel* model = nullptr);
    ~EmployeeWidget();

private:
    QTableView* table;
    EmployeeModel* m_model;

    QVBoxLayout* mainLayout;
    QSqlRelationalDelegate* dbRelational;

private slots:
    void rowDelete(int row);
};

QT_END_NAMESPACE

#endif // WINDOWS_H
