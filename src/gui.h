#ifndef GUI_H
#define GUI_H
#include <QMainWindow>
#include <QPlainTextEdit>
#include <QFile>
#include <QDir>
#include <QSound>
#include <QFileDialog>
#include <QMovie>
#include <fstream>
#include "setup.h"
#include "wipechan.h"

namespace Ui {
class GUI;
}

class GUI : public QMainWindow {
    Q_OBJECT

public:
    explicit GUI(QWidget * parent = nullptr);
    ~GUI();

    void debug(const char &mode);

    Setup setup;

private slots:
    void on_startButton_clicked();
    void on_closeButton_clicked();
    void on_rollUpButton_clicked();
    void on_peedorBarButton_clicked();
    void on_noText_clicked();
    void on_quoting_clicked();
    void on_copying_clicked();
    void on_overload_clicked();
    void on_pastes_clicked();
    void on_random_clicked();
    void on_sage_clicked(bool checked);
    void on_serverKey_clicked(bool checked);
    void on_chaos_clicked(bool checked);
    void on_randChaos_clicked(bool checked);
    void on_musicButton_clicked();
    void on_images_clicked();
    void on_videos_clicked();
    void on_medias_clicked();
    void on_noMedia_clicked();
    void on_openButton_clicked();
    void on_saveButton_clicked();
    void on_mainMode_clicked();
    void on_shrapnelMode_clicked();
    void on_complain_clicked();
    void on_username_editingFinished();
    void on_helpButton_clicked();
    void on_anticaptcha_clicked();
    void on_x_captcha_clicked();
    void on_gurocaptcha_clicked();
    void on_reserved_clicked();

    bool eventFilter(QObject * object, QEvent * event);

private:
    Ui::GUI * ui;
    QFileDialog * fileDialog;

    void set_layout(std::string path);
    void install_filter();

    bool started;  // start/stop
    bool start();
    bool stop();
    void loadData();
    void updateData(const bool &chanIsCalled);
    void updateConfig();
    void delAll(std::ifstream &response);

    void closeEvent (QCloseEvent * event);
    void exposeEvent(QExposeEvent * event);

    QSound * music;
    bool isPlaying;
    bool isPidor;
    std::string username = "Аноним";
    std::string totalPosts = "0";
    std::string postsCount;
    std::string totalBans = "0";
    std::string proxiesCount;
    bool met = false;
    long relation = 5000;
    QMovie * anima;

    QPoint mpos;
    void mouseMoveEvent(QMouseEvent * event);
    void mousePressEvent(QMouseEvent * event);

    void check_main_mode();
    void check_shrapnel_mode();
    void check_solver();
    void check_server_key(bool checked);
    void check_mode(std::string box);
    void check_chaos(bool checked);
    void check_rand_chaos(bool checked);
    void check_sage(bool checked);
    void check_media(std::string type);
};




class Term : public QPlainTextEdit {
public:
    Term (QWidget * parent) {
        QPalette p = palette();
        p.setColor(QPalette::Base, Qt::black);
        p.setColor(QPalette::Text, Qt::green);
        setPalette(p);
    }

    void keyPressEvent(QKeyEvent *event) {
        if (event->key() >= 0x20 && event->key() <= 0x7E && (event->modifiers() == Qt::NoModifier || event->modifiers() == Qt::ShiftModifier))
            QPlainTextEdit::keyPressEvent(event);
        if (event->key() == Qt::Key_Backspace && event->modifiers() == Qt::NoModifier)
            QPlainTextEdit::keyPressEvent(event);
        if (event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier)
            ;
    }
    void mousePressEvent(QMouseEvent *) { setFocus(); }
    void mouseDoubleClickEvent(QMouseEvent *) {}
    void contentMenuEvent(QContextMenuEvent *) {}


};



#endif // GUI_H
