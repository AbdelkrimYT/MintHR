#include "windows.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    actionExit = new QAction(tr("&Exit"), this);
    actionDep = new QAction(tr("New &Department"), this);
    actionJob = new QAction(tr("New &Jon"), this);
    actionEmploye = new QAction(tr("New E&mployee"), this);

    connect(actionDep, &QAction::triggered, this, &MainWindow::createNewDep);
    connect(actionJob, &QAction::triggered, this, &MainWindow::createNewJob);
    connect(actionEmploye, &QAction::triggered, this, &MainWindow::createNewEmploye);

    mainMenubar = menuBar();
    mainStatusBar = statusBar();


    menuFile = new QMenu(tr("File"), mainMenubar);
    menuEdit = new QMenu(tr("Edit"), mainMenubar);
    menuHelp = new QMenu(tr("Help"), mainMenubar);

    menuFile->addAction(actionExit);
    menuEdit->addAction(actionDep);
    menuEdit->addAction(actionJob);
    menuEdit->addAction(actionEmploye);

    mainMenubar->addMenu(menuFile);
    mainMenubar->addMenu(menuEdit);
    mainMenubar->addMenu(menuHelp);


    depModel = new DepartmentModel(this);
    jobModel = new JobModel(this);
    shfModel = new ShiftModel(this);
    empModel = new EmployeeModel(this);


    mainWidget = new QTabWidget(this);
    mainWidget->setObjectName("MainWidget");
    mainWidget->setDocumentMode(true);

    depWidget = new DepartmentWidget(mainWidget, depModel);
    jobWidget = new JobWidget(mainWidget, jobModel);
    shfWidget = new ShiftWidget(mainWidget, shfModel);
    empWidget = new EmployeeWidget(mainWidget, empModel);

    mainWidget->addTab(depWidget, QIcon(":/icons/assets/apartment.svg"), tr("Departments"));
    mainWidget->addTab(jobWidget, QIcon(":/icons/assets/work.svg"), tr("Jobs"));
    mainWidget->addTab(shfWidget, QIcon(":/icons/assets/schedule.svg"), tr("Shifts"));
    mainWidget->addTab(empWidget, QIcon(":/icons/assets/person.svg"), tr("Employees"));
    //mainWidget->tabBar()->setMinimumWidth(200);

    // Setup window
    setWindowTitle(tr("MintHR"));
    setMenuBar(mainMenubar);
    setCentralWidget(mainWidget);
    setStatusBar(mainStatusBar);

    searchEdit = new QLineEdit(this);
    searchEdit->setToolTip(tr("Search"));
    searchEdit->setPlaceholderText(tr("Type here to search"));
    searchEdit->setClearButtonEnabled(true);

    zkButton = new QToolButton(this);
    zkButton->setIcon(QIcon(":/icons/assets/fingerprint.svg"));
    zkButton->setPopupMode(QToolButton::ToolButtonPopupMode::MenuButtonPopup);
    zkButton->setToolButtonStyle(Qt::ToolButtonIconOnly);

    mainStatusBar->addPermanentWidget(searchEdit);
    mainStatusBar->addPermanentWidget(zkButton);
    mainStatusBar->setMinimumWidth(32);
}

MainWindow::~MainWindow()
{
    delete actionExit;
    delete actionDep;
    delete actionJob;
    delete actionEmploye;

    delete mainMenubar;
    delete mainStatusBar;
    delete menuFile;
    delete menuEdit;
    delete menuHelp;

    delete depModel;
    delete jobModel;
    delete shfModel;
    delete empModel;

    delete mainWidget;
    delete depWidget;
    delete jobWidget;

    delete shfWidget;
    delete empWidget;

    delete searchEdit;
    delete zkButton;
}

void MainWindow::createNewDep()
{
    const int row = depModel->rowCount();
    depModel->insertRow(row);
    empModel->select();
}

void MainWindow::createNewJob()
{
    const int row = jobModel->rowCount();
    jobModel->insertRow(row);
    empModel->select();
}

void MainWindow::createNewShift()
{
    const unsigned int row = shfModel->rowCount();
    shfModel->insertRow(row);
}

void MainWindow::deleteShift(int row)
{
    shfModel->removeShift(row);
    shfModel->select();
}

void MainWindow::createNewEmploye()
{
    const int row = empModel->rowCount();
    empModel->insertRow(row);
}

void MainWindow::deleteEmploye(int row)
{
    empModel->removeRow(row);
    empModel->select();
}



DepartmentWidget::DepartmentWidget(QWidget *parent, DepartmentModel *model) : QTableView(parent)
{
    m_model = model;

    setModel(m_model);
    hideColumn(0);
    setAlternatingRowColors(true);
    setShowGrid(false);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setToolTip(tr("Delete"));

    connect(verticalHeader(), &QHeaderView::sectionClicked, this, &DepartmentWidget::rowDelete);
}

DepartmentWidget::~DepartmentWidget()
{
}

void DepartmentWidget::rowDelete(int row)
{
    m_model->removeRow(row);
    m_model->select();
}




JobWidget::JobWidget(QWidget* parent, JobModel* model) : QTableView(parent)
{
    m_model = model;

    setModel(m_model);
    hideColumn(0);
    setAlternatingRowColors(true);
    setShowGrid(false);
    horizontalHeader()->setStretchLastSection(true);
    verticalHeader()->setToolTip(tr("Delete"));

    connect(verticalHeader(), &QHeaderView::sectionClicked, this, &JobWidget::rowDelete);
}

JobWidget::~JobWidget()
{
}

void JobWidget::rowDelete(int row)
{
    m_model->removeRow(row);
    m_model->select();
}




ShiftWidget::ShiftWidget(QWidget* parent, ShiftModel* model) : QWidget(parent)
{
    m_model = model;

    table = new QTableView(this);
    dayDelegate = new ShiftDayDelegate(table);

    table->setModel(m_model);
    table->hideColumn(0);
    table->setAlternatingRowColors(true);
    table->setShowGrid(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setToolTip(tr("Delete"));
    table->setItemDelegateForColumn(6, dayDelegate);

    mainBoard = new QTabWidget(this);
    mainBoard->setTabPosition(QTabWidget::South);
    //mainBoard->setMaximumHeight(250);
    //mainBoard->setDocumentMode(true);

    pgEdit = new QScrollArea(mainBoard);
    pgEdit->setWidgetResizable(true);
    pgEdit->setFrameShape(QFrame::NoFrame);

    pgEditWidget = new QWidget(pgEdit);
    pgEditLayout = new QVBoxLayout(pgEditWidget);

    lbl_e1 = new QLabel(tr("Name:"), pgEditWidget);
    lbl_e2 = new QLabel(tr("Start Time:"), pgEditWidget);
    lbl_e3 = new QLabel(tr("End Time:"), pgEditWidget);
    lbl_e4 = new QLabel(tr("Delay Time:"), pgEditWidget);
    lbl_e5 = new QLabel(tr("EarlyTime:"), pgEditWidget);
    lbl_e6 = new QLabel(tr("Active Days:"), pgEditWidget);
    spx_c7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    lbl_e1->setMinimumWidth(120);
    lbl_e2->setMinimumWidth(120);
    lbl_e3->setMinimumWidth(120);
    lbl_e4->setMinimumWidth(120);
    lbl_e5->setMinimumWidth(120);
    lbl_e6->setMinimumWidth(120);

    shf_e1 = new QLineEdit(pgEditWidget);
    shf_e2 = new QTimeEdit(pgEditWidget);
    shf_e3 = new QTimeEdit(pgEditWidget);;
    shf_e4 = new QTimeEdit(pgEditWidget);;
    shf_e5 = new QTimeEdit(pgEditWidget);;
    shf_e6 = new QListView(pgEditWidget);
    btn_e7 = new QPushButton(tr("Edit"), pgEditWidget);

    shf_e2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    shf_e3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    shf_e4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    shf_e5->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    hbl_e1 = new QHBoxLayout;
    hbl_e2 = new QHBoxLayout;
    hbl_e3 = new QHBoxLayout;
    hbl_e4 = new QHBoxLayout;
    hbl_e5 = new QHBoxLayout;
    hbl_e6 = new QHBoxLayout;
    hbl_e7 = new QHBoxLayout;

    hbl_e1->addWidget(lbl_e1);
    hbl_e2->addWidget(lbl_e2);
    hbl_e3->addWidget(lbl_e3);
    hbl_e4->addWidget(lbl_e4);
    hbl_e5->addWidget(lbl_e5);
    hbl_e6->addWidget(lbl_e6);
    hbl_e7->addItem(spx_c7);

    hbl_e1->addWidget(shf_e1);
    hbl_e2->addWidget(shf_e2);
    hbl_e3->addWidget(shf_e3);
    hbl_e4->addWidget(shf_e4);
    hbl_e5->addWidget(shf_e5);
    hbl_e6->addWidget(shf_e6);
    hbl_e7->addWidget(btn_e7);

    pgEditLayout->addItem(hbl_e1);
    pgEditLayout->addItem(hbl_e2);
    pgEditLayout->addItem(hbl_e3);
    pgEditLayout->addItem(hbl_e4);
    pgEditLayout->addItem(hbl_e5);
    pgEditLayout->addItem(hbl_e6);
    pgEditLayout->addItem(hbl_e7);

    m_shfd_model = new WorkDayModel(this);

    const QModelIndex idx = m_model->index(0, 0);

    if (idx.isValid())
        m_shfd_model->select(idx.data().toInt());

    shf_e6->setModel(m_shfd_model);

    pgEdit->setWidget(pgEditWidget);


    pgCreate = new QScrollArea(mainBoard);
    pgCreate->setWidgetResizable(true);
    pgCreate->setFrameShape(QFrame::NoFrame);

    pgCreateWidget = new QWidget(pgCreate);
    pgCreateLayout = new QVBoxLayout(pgCreateWidget);

    lbl_c1 = new QLabel(tr("Name:"), pgCreateWidget);
    lbl_c2 = new QLabel(tr("Start Time:"), pgCreateWidget);
    lbl_c3 = new QLabel(tr("End Time:"), pgCreateWidget);
    lbl_c4 = new QLabel(tr("Delay Time:"), pgCreateWidget);
    lbl_c5 = new QLabel(tr("EarlyTime:"), pgCreateWidget);
    lbl_c6 = new QLabel(tr("Active Days:"), pgCreateWidget);
    spx_c7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

    lbl_c1->setMinimumWidth(120);
    lbl_c2->setMinimumWidth(120);
    lbl_c3->setMinimumWidth(120);
    lbl_c4->setMinimumWidth(120);
    lbl_c5->setMinimumWidth(120);
    lbl_c6->setMinimumWidth(120);

    shf_c1 = new QLineEdit(pgCreateWidget);
    shf_c2 = new QTimeEdit(pgCreateWidget);
    shf_c3 = new QTimeEdit(pgCreateWidget);
    shf_c4 = new QTimeEdit(pgCreateWidget);
    shf_c5 = new QTimeEdit(pgCreateWidget);
    shf_c6 = new QListWidget(pgCreateWidget);
    btn_c7 = new QPushButton(tr("Create"), pgCreateWidget);

    shf_c2->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    shf_c3->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    shf_c4->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);
    shf_c5->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Minimum);

    shf_c2->setTime(QTime(8, 00));
    shf_c3->setTime(QTime(8, 10));
    shf_c4->setTime(QTime(16, 00));
    shf_c5->setTime(QTime(15, 50));

    QListWidgetItem* item1 = new QListWidgetItem(tr("Saturday"), shf_c6);
    QListWidgetItem* item2 = new QListWidgetItem(tr("Sunday"), shf_c6);
    QListWidgetItem* item3 = new QListWidgetItem(tr("Monday"), shf_c6);
    QListWidgetItem* item4 = new QListWidgetItem(tr("Tuesday"), shf_c6);
    QListWidgetItem* item5 = new QListWidgetItem(tr("Wednesday"), shf_c6);
    QListWidgetItem* item6 = new QListWidgetItem(tr("Thursday"), shf_c6);
    QListWidgetItem* item7 = new QListWidgetItem(tr("Friday"), shf_c6);

    item1->setCheckState(Qt::Unchecked);
    item2->setCheckState(Qt::Unchecked);
    item3->setCheckState(Qt::Unchecked);
    item4->setCheckState(Qt::Unchecked);
    item5->setCheckState(Qt::Unchecked);
    item6->setCheckState(Qt::Unchecked);
    item7->setCheckState(Qt::Unchecked);

    item1->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item2->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item3->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item4->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item5->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item6->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
    item7->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

    hbl_c1 = new QHBoxLayout;
    hbl_c2 = new QHBoxLayout;
    hbl_c3 = new QHBoxLayout;
    hbl_c4 = new QHBoxLayout;
    hbl_c5 = new QHBoxLayout;
    hbl_c6 = new QHBoxLayout;
    hbl_c7 = new QHBoxLayout;
    hbl_c7 = new QHBoxLayout;

    hbl_c1->addWidget(lbl_c1);
    hbl_c2->addWidget(lbl_c2);
    hbl_c3->addWidget(lbl_c3);
    hbl_c4->addWidget(lbl_c4);
    hbl_c5->addWidget(lbl_c5);
    hbl_c6->addWidget(lbl_c6);
    hbl_c7->addItem(spx_c7);

    hbl_c1->addWidget(shf_c1);
    hbl_c2->addWidget(shf_c2);
    hbl_c3->addWidget(shf_c3);
    hbl_c4->addWidget(shf_c4);
    hbl_c5->addWidget(shf_c5);
    hbl_c6->addWidget(shf_c6);
    hbl_c7->addWidget(btn_c7);

    pgCreateLayout->addItem(hbl_c1);
    pgCreateLayout->addItem(hbl_c2);
    pgCreateLayout->addItem(hbl_c3);
    pgCreateLayout->addItem(hbl_c4);
    pgCreateLayout->addItem(hbl_c5);
    pgCreateLayout->addItem(hbl_c6);
    pgCreateLayout->addItem(hbl_c7);

    pgCreate->setWidget(pgCreateWidget);

    mapper = new QDataWidgetMapper(this);
    mapper->setModel(m_model);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setOrientation(Qt::Horizontal);
    mapper->addMapping(shf_e1, 1);
    mapper->addMapping(shf_e2, 2);
    mapper->addMapping(shf_e3, 3);
    mapper->addMapping(shf_e4, 4);
    mapper->addMapping(shf_e5, 5);
    mapper->toFirst();

    pgEmp = new QTableView(mainBoard);
    m_emp_model = new QSqlQueryModel(this);
    empQuery = new QSqlQuery;

    empQuery->prepare("SELECT employee_name, job_title, employee_start_date, department_name FROM employees AS E INNER JOIN shifts AS S ON E.shift_id = S.shift_id AND S.shift_id = :ShiftId LEFT JOIN jobs AS J ON E.job_id= J.job_id LEFT JOIN departments AS D ON E.department_id = D.department_id");
    empQuery->bindValue(":ShiftId", 1);
    empQuery->exec();

    m_emp_model->setQuery(*empQuery);
    m_emp_model->setHeaderData(0, Qt::Horizontal, tr("Employee name"));
    m_emp_model->setHeaderData(1, Qt::Horizontal, tr("Job Title"));
    m_emp_model->setHeaderData(2, Qt::Horizontal, tr("Start Date"));
    m_emp_model->setHeaderData(3, Qt::Horizontal, tr("Department"));

    pgEmp->setModel(m_emp_model);

    mainBoard->addTab(pgEdit, QIcon(":/icons/assets/edit.svg"), tr("Edit"));
    mainBoard->addTab(pgCreate, QIcon(":/icons/assets/create.svg"), tr("Create"));
    mainBoard->addTab(pgEmp, QIcon(":/icons/assets/person.svg"), tr("Employees"));

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(table);
    mainLayout->addWidget(mainBoard);

    connect(table->verticalHeader(), &QHeaderView::sectionClicked, this, &ShiftWidget::rowDelete);
    connect(btn_c7, &QPushButton::clicked, this, &ShiftWidget::newShift);
    connect(btn_e7, &QPushButton::clicked, this, &ShiftWidget::editShift);
    connect(static_cast<QItemSelectionModel*>(table->selectionModel()), &QItemSelectionModel::currentRowChanged, this, &ShiftWidget::nextShift);
    connect(table->selectionModel(), &QItemSelectionModel::currentRowChanged,
            mapper, &QDataWidgetMapper::setCurrentModelIndex);
}

ShiftWidget::~ShiftWidget()
{
    delete table;
    delete mainLayout;
    delete dayDelegate;
    delete mainBoard;
    delete pgEdit;
    delete pgEditWidget;
    delete pgEditLayout;
    delete pgCreate;

    delete lbl_e1;
    delete lbl_e2;
    delete lbl_e3;
    delete lbl_e4;
    delete lbl_e5;
    delete lbl_e6;
    delete spx_e7;

    delete shf_e1;
    delete shf_e2;
    delete shf_e3;
    delete shf_e4;
    delete shf_e5;
    delete shf_e6;
    delete btn_e7;

    delete hbl_e1;
    delete hbl_e2;
    delete hbl_e3;
    delete hbl_e4;
    delete hbl_e5;
    delete hbl_e6;
    delete hbl_e7;

    delete lbl_c1;
    delete lbl_c2;
    delete lbl_c3;
    delete lbl_c4;
    delete lbl_c5;
    delete lbl_c6;
    delete spx_c7;

    delete shf_c1;
    delete shf_c2;
    delete shf_c3;
    delete shf_c4;
    delete shf_c5;
    delete shf_c6;
    delete btn_c7;

    delete hbl_c1;
    delete hbl_c2;
    delete hbl_c3;
    delete hbl_c4;
    delete hbl_c5;
    delete hbl_c6;
    delete hbl_c7;

    delete mapper;
    delete pgEmp;
    delete m_emp_model;
    delete empQuery;
    delete m_shfd_model;
}

void ShiftWidget::rowDelete(int row)
{

}

void ShiftWidget::newShift()
{
    QSqlQuery insertQuery;
    insertQuery.prepare("INSERT INTO shifts(shift_name, start_time, end_time, delay_time, early_time)"
                  "VALUES (:ShiftName, :StartTime, :EndTime, :DelayTime, :EarlyTime)");

    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE shift_days SET is_enabled = 1 WHERE  shift_day = :ShiftDay AND shift_id = last_insert_rowid();");

    const QString shift_name = shf_c1->text();
    const QTime start_time = shf_c2->time();
    const QTime end_time = shf_c3->time();
    const QTime delay_time = shf_c4->time();
    const QTime early_time = shf_c5->time();

    db.transaction();

    insertQuery.bindValue(":ShiftName", shift_name);
    insertQuery.bindValue(":StartTime", start_time);
    insertQuery.bindValue(":EndTime", end_time);
    insertQuery.bindValue(":DelayTime", delay_time);
    insertQuery.bindValue(":EarlyTime", early_time);
    insertQuery.exec();

    for (int i = 0; i < 7; i++)
    {
        const auto item = shf_c6->item(i);

        if (item->checkState() == Qt::Checked)
        {
            updateQuery.bindValue(":ShiftDay", item->data(Qt::DisplayRole).toString().toUpper());
            updateQuery.exec();
        }
    }

    db.commit();

    m_model->select();

    QMessageBox::information(this, nullptr, "New Shift created successfully.");

    shf_c1->clear();
    shf_c2->setTime(QTime(8, 00));
    shf_c3->setTime(QTime(8, 10));
    shf_c4->setTime(QTime(16, 00));
    shf_c5->setTime(QTime(15, 50));

    for (int i = 0; i < 7; i++)
        shf_c6->item(i)->setCheckState(Qt::Unchecked);
}

void ShiftWidget::editShift()
{
    QSqlQuery updateQuery;
    updateQuery.prepare("UPDATE shifts SET shift_name = :ShiftName, start_time = :StartTime, end_time = :EndTime, delay_time = :DelayTime, early_time = :EarlyTime WHERE shift_id = :ShiftId");

    const int row = mapper->currentIndex();

    const int shift_id = m_model->index(row, 0).data().toInt();
    const QString shift_name = shf_e1->text();
    const QTime start_time = shf_e2->time();
    const QTime end_time = shf_e3->time();
    const QTime delay_time = shf_e4->time();
    const QTime early_time = shf_e5->time();

    updateQuery.bindValue(":ShiftName", shift_name);
    updateQuery.bindValue(":StartTime", start_time);
    updateQuery.bindValue(":EndTime", end_time);
    updateQuery.bindValue(":DelayTime", delay_time);
    updateQuery.bindValue(":EarlyTime", early_time);
    updateQuery.bindValue(":ShiftId", shift_id);

    updateQuery.exec();

    m_shfd_model->update();
    m_model->select();

    QMessageBox::information(this, "", tr("Shift update successfully."));
}

void ShiftWidget::nextShift(const QModelIndex& current, const QModelIndex& previous)
{
    const int row = current.row();
    const int shift_id = m_model->index(row, 0).data().toInt();

    m_shfd_model->select(shift_id);

    emit m_emp_model->layoutAboutToBeChanged();

    empQuery->bindValue(":ShiftId", shift_id);
    empQuery->exec();

    m_emp_model->setQuery(*empQuery);

    emit m_emp_model->layoutChanged();
}




EmployeeWidget::EmployeeWidget(QWidget* parent, EmployeeModel* model) : QWidget(parent)
{
    m_model = model;

    table = new QTableView(this);
    dbRelational = new QSqlRelationalDelegate(this);

    table->setModel(m_model);
    table->hideColumn(0);
    table->setAlternatingRowColors(true);
    table->setShowGrid(false);
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setToolTip("Delete");
    table->setItemDelegateForColumn(2, dbRelational);
    table->setItemDelegateForColumn(3, dbRelational);
    table->setItemDelegateForColumn(4, dbRelational);

    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addWidget(table);

    connect(table->verticalHeader(), &QHeaderView::sectionClicked, this, &EmployeeWidget::rowDelete);
}

EmployeeWidget::~EmployeeWidget()
{
    delete table;
    delete dbRelational;
    delete mainLayout;
}

void EmployeeWidget::rowDelete(int row)
{
    m_model->removeRow(row);
    m_model->select();
}
