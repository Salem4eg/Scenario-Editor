#pragma once

#include <QWidget>
#include <QPixMap>


class ProgressBar : public QWidget
{
	Q_OBJECT

public:
	ProgressBar(QWidget *parent = nullptr);
	~ProgressBar();

public slots:
	void addProgress(int value);
	void resetProgress();

protected:
	void paintEvent(QPaintEvent* event) override;


private:
	QPixmap frame;
	QPixmap progress;

	int m_value = 0;
};

