#include "ProvinceInfoToolTip.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

ProvinceInfoToolTip::ProvinceInfoToolTip(QWidget *parent)
	: QWidget(nullptr)
{
	auto* main_layout = new QVBoxLayout(this);

	auto* top_widget = new QWidget;
	auto* central_widget = new QWidget;
	// auto * bottom_widget = new QWidget; // space for province details

	auto* top_layout = new QHBoxLayout(top_widget);
	auto* central_layout = new QHBoxLayout(central_widget);

	m_name = new QLabel("Province name");
	auto* close_button = new QPushButton("Close"); // should cross image

	top_layout->addStretch(1);
	top_layout->addWidget(m_name, 2, Qt::AlignHCenter);
	top_layout->addWidget(close_button, 1, Qt::AlignRight);

	m_owner = new QLabel("Owner: ");
	m_cores = new QLabel("Cores: ");

	central_layout->addWidget(m_owner, 0, Qt::AlignLeft);
	central_layout->addWidget(m_cores, 0, Qt::AlignLeft);

	main_layout->addWidget(top_widget, 1);
	main_layout->addWidget(central_widget, 4);

	connect(close_button, &QPushButton::clicked, this, [this]()
		{
			emit hideToolTip();
		});
}

ProvinceInfoToolTip::~ProvinceInfoToolTip()
{}

void ProvinceInfoToolTip::setInfo(const ProvinceInfo& info)
{
	m_name->setText(info.name);
	m_owner->setText("Owner: " + info.owner);
	m_cores->setText("Cores: " + info.cores.join(", "));
}

