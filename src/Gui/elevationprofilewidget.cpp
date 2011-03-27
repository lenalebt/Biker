#include "elevationprofilewidget.hpp"
#include "ui_elevationprofilewidget.h"

ElevationProfileWidget::ElevationProfileWidget(OSMDatabaseReader* dbreader, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ElevationProfileWidget),
    dbreader(dbreader)
{
    ui->setupUi(this);
}

ElevationProfileWidget::~ElevationProfileWidget()
{
    delete ui;
}

void ElevationProfileWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
