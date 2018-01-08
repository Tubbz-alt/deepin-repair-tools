#ifndef SCANNINGPAGE_H
#define SCANNINGPAGE_H

#include <QWidget>
#include <QLabel>

#include <DSpinner>

DWIDGET_USE_NAMESPACE

class ScanningPage : public QWidget
{
    Q_OBJECT

public:
    explicit ScanningPage(QWidget *parent = nullptr);

private:
    DSpinner *m_spinner;
    QLabel *m_icon;
    QLabel *m_centerTips;
    QLabel *m_bottomTips;
};

#endif // SCANNINGPAGE_H
