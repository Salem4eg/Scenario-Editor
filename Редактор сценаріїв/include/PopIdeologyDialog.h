#pragma once

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QValidator>
#include "Structures.h"

class PopIdeologyDialog : public QWidget
{
	Q_OBJECT

public:
	PopIdeologyDialog(QWidget *parent = nullptr);
	~PopIdeologyDialog();

	void updateIdeologies(const QList<Ideology>& ideologies);
	void createIdeologyWigdets(const QList<Ideology>& ideologies);

signals:
	void ideologiesChanged(QList<Ideology> ideologies);
	void backButtonPressed();

private:
	QVBoxLayout* leftIdeologiesContainer;
	QVBoxLayout* rightIdeologiesContainer;
	QLabel* totalPercentageWidget;

	QHash<int, Ideology> m_ideologies;
	QHash<int, QLineEdit*> m_ideologiesLineEdits;
	bool m_has_changes = false;

	void clearLayout(QLayout* layout);
	void checkTotalPercentage();
};

