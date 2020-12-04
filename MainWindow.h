#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QFileDialog>
#include <QStringList>
#include <QResource>
#include <QStandardItemModel>
#include <QModelIndex>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    void list_files(const QString &path);
    void list_rcc();
    bool extra_rcc(const QString &output, const QString &file, bool force=false);

private slots:
    void on_button_exit_clicked();
    void on_button_browse_rcc_clicked();
    void on_button_browse_exdir_clicked();
    void on_button_extra_all_clicked();
    void on_button_extra_selected_clicked();
    void on_list_view_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    QString m_rccfile;
    QString m_extra_dir;
    QString m_selected_file;
    QStringList m_children;
    QStandardItemModel *m_model;
};
#endif // MAINWINDOW_H
