#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_VTKContour.h"

#include <QVTKOpenGLNativeWidget.h>
#include <qdebug.h>

class VTKContour : public QMainWindow
{
    Q_OBJECT

public:
    VTKContour(QWidget *parent = nullptr);
    ~VTKContour();

private:
    Ui::VTKContourClass ui;

    QVTKOpenGLNativeWidget* _pVTKWidget = Q_NULLPTR;
};
