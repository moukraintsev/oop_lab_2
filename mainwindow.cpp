#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _table()
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels({"Кодон", "Аминокислота"});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Введите данные");
    auto key_edit = new QLineEdit(&dlg);
    key_edit->setText(QString("кодон"));
    auto val_edit = new QLineEdit(&dlg);
    val_edit->setText(QString("аминокислота"));
    auto btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    auto layout = new QFormLayout();
    layout->addRow(tr("Ключ"), key_edit);
    layout->addRow(tr("Значение"), val_edit);
    layout->addWidget(btn_box);
    dlg.setLayout(layout);
    if(dlg.exec() == QDialog::Accepted) {
        auto key = key_edit->text().toStdString();
        auto val = val_edit->text().toStdString();
        if (_table.has(key)){
            QMessageBox::warning(this, tr("Ошибка"),
                                 tr("Такой ключ уже есть. Если хотите изменить значение, нажмите изменить"));
            return;
        }
        _table.add(key, val);
        _show_table();
    }
}

void MainWindow::_show_table()
{
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    auto pairs = _table.getAllValues();
    for (auto p: pairs){
        auto key = QString::fromStdString(p.second);
        auto val = QString::fromStdString(p.first);
        ui->tableWidget->setRowCount(ui->tableWidget->rowCount()+1);
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 0, new QTableWidgetItem(key));
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-1, 1, new QTableWidgetItem(val));
    }
    ui->tableWidget->setHorizontalHeaderLabels({"Кодон", "Аминокислота"});
}

void MainWindow::on_pushButton_2_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Введите данные");
    auto key_edit = new QLineEdit(&dlg);
    key_edit->setText(QString("кодон"));
    auto val_edit = new QLineEdit(&dlg);
    val_edit->setText(QString("аминокислота"));
    auto btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    auto layout = new QFormLayout();
    layout->addRow(tr("Ключ"), key_edit);
    layout->addRow(tr("Значение"), val_edit);
    layout->addWidget(btn_box);
    dlg.setLayout(layout);
    if(dlg.exec() == QDialog::Accepted) {
        auto key = key_edit->text().toStdString();
        auto val = val_edit->text().toStdString();
        if (not _table.has(key)){
            QMessageBox::warning(this, tr("Ошибка"),
                                 tr("Такого ключа нет"));
            return;
        }
        _table.at(key) = val;
        _show_table();
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Введите данные");
    auto key_edit = new QLineEdit(&dlg);
    key_edit->setText(QString("кодон"));
    auto btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    auto layout = new QFormLayout();
    layout->addRow(tr("Ключ"), key_edit);
    layout->addWidget(btn_box);
    dlg.setLayout(layout);
    if(dlg.exec() == QDialog::Accepted) {
        auto key = key_edit->text().toStdString();
        if (not _table.has(key)){
            QMessageBox::warning(this, tr("Ошибка"),
                                 tr("Такого ключа нет"));
            return;
        }
        _table.pop(key);
        _show_table();
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    _table.clear();
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setHorizontalHeaderLabels({"Кодон", "Аминокислота"});
}

void MainWindow::on_pushButton_5_clicked()
{
    QDialog dlg(this);
    dlg.setWindowTitle("Введите данные");
    auto key_edit = new QLineEdit(&dlg);
    key_edit->setText(QString("кодон"));
    auto btn_box = new QDialogButtonBox(&dlg);
    btn_box->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(btn_box, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
    connect(btn_box, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);
    auto layout = new QFormLayout();
    layout->addRow(tr("Ключ"), key_edit);
    layout->addWidget(btn_box);
    dlg.setLayout(layout);
    if(dlg.exec() == QDialog::Accepted) {
        auto key = key_edit->text().toStdString();
        if (_table.has(key)){
            QMessageBox::information(this, tr("Значение по ключу"),
                                     QString::fromStdString("Ключ: " + key + "\nЗначение: " + _table.at(key)));
            return;
        }
        else{
            QMessageBox::warning(this, tr("Ошибка"),
                                 tr("Такого ключа нет"));
        }
    }
}

void MainWindow::on_pushButton_6_clicked()
{
    //from file
    std::string filename = QFileDialog::getOpenFileName(this, tr("Имя файла"), "/").toStdString();
    _table.fromFile(filename);
    _show_table();
}

void MainWindow::on_pushButton_7_clicked()
{
    //to file
    std::string filename = QFileDialog::getSaveFileName(this, tr("Имя файла"), "/").toStdString();
    _table.toFile(filename);
}
