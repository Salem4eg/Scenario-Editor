#include "ProvinceInfoToolTip.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

ProvinceInfoToolTip::ProvinceInfoToolTip(QWidget *parent)
	: QWidget(parent)
{
	QVBoxLayout* main_layout = new QVBoxLayout(this);
	main_layout->setContentsMargins(0, 0, 0, 0);

	QWidget* widget = new QWidget;
	widget->setFixedSize(300, 150);
	main_layout->addWidget(widget);

	auto* layout = new QVBoxLayout(widget);
	layout->setContentsMargins(0, 0, 0, 0);

	auto* top_widget = new QWidget;
	auto* top_layout = new QHBoxLayout(top_widget);

	m_name = new QLabel("Province name");
	m_name->setObjectName("province_name");
	top_layout->addWidget(m_name, 0, Qt::AlignHCenter);

	auto* middle_widget = new QWidget;
	auto* middle_layout = new QHBoxLayout(middle_widget);

	m_owner = new QLabel("Owner: ");
	m_cores = new QLabel("Cores: ");
	m_cores->setWordWrap(true);
	m_cores->setFixedWidth(120);

	middle_layout->addStretch(1);
	middle_layout->addWidget(m_owner, 0, Qt::AlignLeft);
	middle_layout->addStretch(1);
	middle_layout->addWidget(m_cores, 0, Qt::AlignRight);

	auto* bottom_widget = new QWidget;
	auto* bottom_layout = new QHBoxLayout(bottom_widget);

	auto* close_button = new QPushButton("Close");
	close_button->setFixedSize(80, 30);
	bottom_layout->addWidget(close_button, 0, Qt::AlignHCenter);

	layout->addWidget(top_widget);
	layout->addWidget(middle_widget);
	layout->addWidget(bottom_widget);


	connect(close_button, &QPushButton::clicked, this, [this]()
		{
			emit hideToolTip();
		});

	widget->setObjectName("tooltip");

	setStyleSheet(R"(
		QWidget
		{
			font-family: Georgia;
			color: #f5cc93;
			font-size: 14px;
		}

		QPushButton
		{
			border-image: url(images/small_button.png) 2 2 2 2 stretch;
			font-weight: bold;
			font-size: 18px;
		}

		QPushButton:hover
		{
			border-image: url(images/small_button_hovered.png) 2 2 2 2 stretch;
		}

		#tooltip
		{
			background-image: url(images/tooltip.png) 2 2 2 2 stretch;
			background-position: center;
			background-repeat: no-repeat;
		}
		
		#province_name
		{
			font-size: 18px;
			font-weight: bold;
		}
		
		
	)");
}

ProvinceInfoToolTip::~ProvinceInfoToolTip()
{}

void ProvinceInfoToolTip::setInfo(const ProvinceInfo& info)
{
	m_name->setText(info.name);
	m_owner->setText("Owner: " + info.owner);
	m_cores->setText("Cores: " + info.cores.join(", "));
}

void ProvinceInfoToolTip::mousePressEvent(QMouseEvent* event)
{
	//qDebug() << "Event accepted in tooltip";
	//event->accept();
	
}

void ProvinceInfoToolTip::mouseReleaseEvent(QMouseEvent* event)
{
	//qDebug() << "Event accepted in tooltip:releaseEvent";
	//event->accept();
}

void ProvinceInfoToolTip::mouseMoveEvent(QMouseEvent* event)
{
	
}
