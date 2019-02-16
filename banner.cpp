#include "banner.h"
#include "ui_banner.h"

Banner::Banner (QWidget *parent) : QDialog(parent) , ui(new Ui::Banner) {
    ui->setupUi(this);
}

Banner::~Banner () {
    delete ui;
}

void Banner::changeEvent (QEvent * e) {
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
