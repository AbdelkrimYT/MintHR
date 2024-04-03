#include "windows.h"

#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QFile>
#include <QMessageBox>

QSqlDatabase db;

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    //application.setStyle("Fusion");

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("minthr.db");

    if (!db.open())
    {
        QMessageBox::warning(nullptr, "Database error", db.lastError().text());
        return application.exit();
    }

    QFile theme(":/themes/theme.qss");
    theme.open(QFile::ReadOnly);

    QString styleSheet(theme.readAll());

    MainWindow mainWindow;
    mainWindow.setStyleSheet(styleSheet);
    mainWindow.showMaximized();

    return application.exec();
}
