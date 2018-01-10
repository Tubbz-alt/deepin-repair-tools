#include "grubrepairwidget.h"
#include "grubrepairthread.h"

#include <QVBoxLayout>
#include <QDebug>
#include <QTimer>
#include <QApplication>

GrubRepairWidget::GrubRepairWidget(QWidget *parent)
    : QWidget(parent)

    , m_icon(new QLabel)
    , m_centerTips(new QLabel)
    , m_statusTips(new QLabel)
    , m_spinner(new DSpinner)
    , m_output(new QTextEdit)
    , m_showDetail(new DLinkButton)
    , m_hideDetail(new DLinkButton)
    , m_repairButton(new QPushButton)
    , m_okButton(new QPushButton)
{
    m_output->setStyleSheet("QTextEdit {"
                            "color: #1077d7;"
                            "}");

    m_showDetail->setText(tr("Show Detail"));
    m_hideDetail->setText(tr("Hide Detail"));
    m_icon->setPixmap(QIcon(":/resources/repair_grub.svg").pixmap(128, 128));
    m_icon->setAlignment(Qt::AlignHCenter);
    m_repairButton->setText(tr("Repair"));
    m_centerTips->setText(tr("Cannot find boot menu"));
    m_centerTips->setAlignment(Qt::AlignHCenter);
    m_statusTips->setAlignment(Qt::AlignHCenter);
    m_okButton->setText(tr("Finish"));
    m_spinner->setFixedSize(32, 32);

    QHBoxLayout *btnsLayout = new QHBoxLayout;
    btnsLayout->addStretch();
    btnsLayout->addWidget(m_repairButton);
    btnsLayout->addWidget(m_okButton);
    btnsLayout->addStretch();

    QVBoxLayout *centralLayout = new QVBoxLayout;
    centralLayout->addWidget(m_output);
    centralLayout->addWidget(m_icon);
    centralLayout->addWidget(m_centerTips);
    centralLayout->addWidget(m_showDetail);
    centralLayout->setAlignment(m_showDetail, Qt::AlignHCenter);
    centralLayout->addWidget(m_hideDetail);
    centralLayout->setAlignment(m_hideDetail, Qt::AlignHCenter);
    centralLayout->addStretch();
    centralLayout->addWidget(m_statusTips);
    centralLayout->addSpacing(5);
    centralLayout->addLayout(btnsLayout);
    centralLayout->addWidget(m_spinner);
    centralLayout->setAlignment(m_spinner, Qt::AlignHCenter);
    centralLayout->setSpacing(0);
    centralLayout->setContentsMargins(0, 60, 0, 40);

    setLayout(centralLayout);

    connect(m_repairButton, &QPushButton::clicked, this, &GrubRepairWidget::onRepairClicked);
    connect(m_okButton, &QPushButton::clicked, this, &GrubRepairWidget::resetUi);
    connect(m_showDetail, &QPushButton::clicked, this, &GrubRepairWidget::showDetail);
    connect(m_hideDetail, &QPushButton::clicked, this, &GrubRepairWidget::hideDetail);

    QTimer::singleShot(1, this, &GrubRepairWidget::resetUi);
}

void GrubRepairWidget::onRepairClicked()
{
    m_repairButton->setVisible(false);
    m_showDetail->setVisible(true);
    m_spinner->setVisible(true);
    m_spinner->start();

    GrubRepairThread *thrd = new GrubRepairThread;
    thrd->setToolsProxy(m_toolsProxy);

    connect(thrd, &GrubRepairThread::finished, thrd, &GrubRepairThread::deleteLater, Qt::QueuedConnection);
    connect(thrd, &GrubRepairThread::finished, this, &GrubRepairWidget::onRepairFinished);
    connect(thrd, &GrubRepairThread::outputPrinted, this, &GrubRepairWidget::appendOutput);

    thrd->start();
}

void GrubRepairWidget::resetUi()
{
    m_output->setVisible(false);
    m_output->clear();
    m_showDetail->setVisible(false);
    m_okButton->setVisible(false);
    m_statusTips->setVisible(false);
    m_repairButton->setVisible(true);
    m_showDetail->setVisible(false);
    m_hideDetail->setVisible(false);
    m_spinner->setVisible(false);
}

void GrubRepairWidget::showDetail()
{
    m_output->setVisible(true);
    m_icon->setVisible(false);
    m_centerTips->setVisible(false);
    m_showDetail->setVisible(false);
    m_hideDetail->setVisible(true);
}

void GrubRepairWidget::hideDetail()
{
    m_output->setVisible(false);
    m_icon->setVisible(true);
    m_centerTips->setVisible(true);
    m_showDetail->setVisible(true);
    m_hideDetail->setVisible(false);
}

void GrubRepairWidget::appendOutput(const QString &output)
{
    m_output->append(output);
}

void GrubRepairWidget::onRepairFinished()
{
    m_spinner->stop();
    m_spinner->setVisible(false);
    m_statusTips->setStyleSheet("color: green;");
    m_statusTips->setText(tr("Repair success"));
    m_statusTips->setVisible(true);
    m_okButton->setVisible(true);
}