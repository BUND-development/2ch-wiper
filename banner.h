#ifndef BANNER_H
#define BANNER_H

#include <QDialog>

namespace Ui {
class Banner;
}

class Banner : public QDialog {
    Q_OBJECT

public:
    explicit Banner(QWidget * parent = nullptr);
    ~Banner();

protected:
    void changeEvent(QEvent * e);

private:
    Ui::Banner * ui;
};

#endif // BANNER_H
