#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Utils.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_model = new QStandardItemModel(this);
    m_model->setHorizontalHeaderLabels(QStringList({"File", "FileAbsPath"}));

    ui->list_view->setModel(m_model);
}

MainWindow::~MainWindow()
{
    if (m_model) {
        m_model->clear();
        delete m_model;
        m_model = nullptr;
    }

    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        on_button_exit_clicked();
    }
}

void MainWindow::list_files(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        return;
    }
    dir.setFilter(QDir::Dirs | QDir::Files);
    dir.setSorting(QDir::DirsFirst);

    foreach (QFileInfo fi, dir.entryInfoList()) {
        if (fi.fileName() == "." || fi.fileName() == "..") {
            continue;
        }
        if (fi.isDir()) {
            list_files(fi.filePath());
        } else {
            m_children.append(fi.absoluteFilePath());
        }
    }
}

void MainWindow::list_rcc()
{
    m_model->clear();
    m_model->setHorizontalHeaderLabels(QStringList({"File", "FileAbsPath"}));

    list_files(":/");

    foreach (QString file, m_children) {
        // DEBUG() << "File:" << file;
        if (file.startsWith(":/")) {
            file = file.remove(0, 2);
        }
        m_model->appendRow(new QStandardItem(file));
    }
}

bool MainWindow::extra_rcc(const QString &output, const QString &file, bool force)
{
    Q_UNUSED(force)

    DEBUG() << "ExtraDir:" << output << "Extra:" << file;

    QFileInfo fi(QString("%1/%2").arg(output).arg(file));
    QDir dir(output);
    dir.mkpath(fi.absolutePath());

    QFile qfile(QString(":/%1").arg(file));
    if (qfile.exists()) {
        ui->statusbar->showMessage(QString("Extra: %1").arg(QDir::toNativeSeparators(file)));
        qfile.copy(fi.absoluteFilePath());
    }

    return true;
}

void MainWindow::on_button_exit_clicked()
{
    QResource::unregisterResource(m_rccfile);

    qApp->exit();
}

void MainWindow::on_button_browse_rcc_clicked()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setFileMode(QFileDialog::ExistingFile);
    dialog->setViewMode(QFileDialog::Detail);
    dialog->setDirectory(".");
    dialog->setWindowTitle(tr("Open a RCC file"));
    dialog->setNameFilter(tr("RCC File(*.rcc)"));

    QStringList rccfiles;
    if (dialog->exec()) {
        rccfiles = dialog->selectedFiles();
    }

    if (rccfiles.isEmpty()) {
        return;
    }

    QFileInfo fi(m_rccfile);
    if (!m_rccfile.isEmpty() && fi.exists(m_rccfile) && fi.isFile()) {
        QResource::unregisterResource(m_rccfile);
    }

    m_rccfile = rccfiles.at(0);
    DEBUG() << "RCC File:" << m_rccfile;
    ui->line_edit_rcc->setText(QDir::toNativeSeparators(m_rccfile));

    fi.setFile(m_rccfile);
    m_extra_dir = QString("%1/%2_RCC_Output").arg(fi.absolutePath()).arg(fi.baseName());
    ui->line_edit_exdir->setText(QDir::toNativeSeparators(m_extra_dir));

    QResource::registerResource(m_rccfile);

    list_rcc();
}

void MainWindow::on_button_browse_exdir_clicked()
{
    QFileDialog *dialog = new QFileDialog(this);
    dialog->setViewMode(QFileDialog::Detail);
    dialog->setFileMode(QFileDialog::DirectoryOnly);
    dialog->setDirectory(".");
    dialog->setWindowTitle(tr("Select a extra output dir"));

    QStringList exdirs;
    if (dialog->exec()) {
        exdirs = dialog->selectedFiles();
    }

    if (exdirs.isEmpty()) {
        return;
    }

    m_extra_dir = exdirs.at(0);
    ui->line_edit_exdir->setText(QDir::toNativeSeparators(m_extra_dir));
}

void MainWindow::on_button_extra_all_clicked()
{
    DEBUG();
    ui->list_view->clearSelection();
    foreach (QString file, m_children) {
        if (file.startsWith(":/")) {
            file = file.remove(0, 2);
        }
        if (!extra_rcc(m_extra_dir, file)) {
            return;
        }
    }

    ui->statusbar->showMessage(QString("Done! Output dir is [%1]").arg(m_extra_dir));
}

void MainWindow::on_button_extra_selected_clicked()
{
    if (m_selected_file.isEmpty()) {
        return;
    }

    extra_rcc(m_extra_dir, m_selected_file);
}

void MainWindow::on_list_view_clicked(const QModelIndex &index)
{
    m_selected_file = index.data().toString();
    DEBUG() << "Selected File:" << m_selected_file;
}

