#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <limits>
#include <QComboBox>
#include <QCompleter>
#include <QFrame>
#include <QCloseEvent>
#include "Structures.h"
#include "PopIdeologyDialog.h"

class PopEditorDialog : public QWidget
{
	Q_OBJECT

public:
	PopEditorDialog(QWidget *parent = nullptr);
	~PopEditorDialog();

	virtual void setPopData(const PopData& pop);

	// One time usage, for initializing all available cultures
	void setCultures(const QList<QString>& cultures);
	// One time usage, for initializing all available types
	void setTypes(const QList<QString>& types);
	// One time usage, for initializing all available religions
	void setReligions(const QList<QString>& religions);
	// One time usage, for initializing all available ideologies
	virtual void setIdeologies(const QList<Ideology>& ideologies);
signals:
	void deletePop(int id);
	void backButtonPressed();
	void popChanged(PopData pop);
protected slots:
	void closeEvent(QCloseEvent* event) override;
	void saveChanges();
	virtual void setupStyle();
	void setupComboBox(QComboBox* combobox);
	void setupConnections();

protected:
	QComboBox* m_typeComboBox;
	QComboBox* m_cultureComboBox;
	QComboBox* m_religionComboBox;
	QLineEdit* m_sizeEdit;	

	QPushButton* backButton;
	QPushButton* deletePopButton;

	bool hasChanges;
	PopData m_popData;

	QHBoxLayout* topLayout;

};

