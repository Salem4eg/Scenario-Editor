#include "ProvinceInfoToolTip.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>

ProvinceInfoToolTip::ProvinceInfoToolTip(QWidget *parent)
	: QWidget(parent)
{
	setObjectName("tooltip");
	auto* main_layout = new QVBoxLayout(this);

	auto* top_widget = new QWidget;
	auto* top_layout = new QHBoxLayout(top_widget);
	top_widget->setStyleSheet("background-color: red");

	m_name = new QLabel("Province name");
	m_name->setGraphicsEffect(getShadow());
	top_layout->addWidget(m_name, 0, Qt::AlignHCenter);

	auto* middle_widget = new QWidget;
	auto* middle_layout = new QHBoxLayout(middle_widget);
	middle_widget->setStyleSheet("background-color: red");

	m_owner = new QLabel("Owner: ");
	m_cores = new QLabel("Cores: ");
	m_owner->setGraphicsEffect(getShadow());
	m_cores->setGraphicsEffect(getShadow());

	middle_layout->addWidget(m_owner, 0, Qt::AlignLeft);
	middle_layout->addWidget(m_cores, 0, Qt::AlignRight);

	auto* bottom_widget = new QWidget;
	auto* bottom_layout = new QHBoxLayout(bottom_widget);
	bottom_widget->setStyleSheet("background-color: red");

	auto* close_button = new QPushButton("Close");
	close_button->setGraphicsEffect(getShadow());

	bottom_layout->addWidget(close_button, 0, Qt::AlignHCenter);

	main_layout->addWidget(top_widget);
	main_layout->addWidget(middle_widget);
	main_layout->addWidget(bottom_widget);

	connect(close_button, &QPushButton::clicked, this, [this]()
		{
			emit hideToolTip();
		});

	//this->setStyleSheet(
	//	R"(
	//		QWidget
	//		{
	//			font-family: Georgia;
	//			color: #f5cc93;
	//		}
	//		
	//		#tooltip
	//		{
	//			border-image: url(images/province_tooltip.png) 2 2 2 2 stretch;
	//			background-position: center;
	//			background-repeat: no-repeat;
	//		}
	//		
	//		QPushButton
	//		{
	//			border-image: url(images/button_background.png) 2 2 2 2 stretch;
	//			font-weight: bold;
	//		}
	//		
	//		
	//		
	//		
	//	)");
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
	event->accept();
}

void ProvinceInfoToolTip::mouseReleaseEvent(QMouseEvent* event)
{
	//qDebug() << "Event accepted in tooltip:releaseEvent";
	event->accept();
}

QGraphicsDropShadowEffect* ProvinceInfoToolTip::getShadow()
{
	return new QGraphicsDropShadowEffect(this);
}
