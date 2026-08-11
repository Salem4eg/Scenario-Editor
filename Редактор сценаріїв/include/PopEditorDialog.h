#pragma once

#include <QWidget>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <limits>
#include <QComboBox>
#include <QCompleter>
#include "Structures.h"
#include "PopIdeologyDialog.h"


class PopEditorDialog : public QWidget
{
	Q_OBJECT

public:
	PopEditorDialog(QWidget *parent = nullptr);
	~PopEditorDialog();

	void setPopData(const PopData& pop);
	void setCultures(const QList<QString>& cultures);
	void setTypes(const QList<QString>& types);
	void setReligions(const QList<QString>& religions);

	// One time usage, for initializing all available ideologies
	void setIdeologies(const QList<Ideology>& ideologies);
signals:
	void deletePop(int id);
	void backButtonPressed();
	void popChanged(PopData pop);
private:
	QComboBox* m_typeComboBox;
	QComboBox* m_cultureComboBox;
	QComboBox* m_religionComboBox;

	QLineEdit* m_sizeEdit;
	QLineEdit* m_militancyEdit;
	QLineEdit* m_literacyEdit;
	QLineEdit* m_consciousnessEdit;
	QPushButton* m_edit_ideologies;

	bool hasChanges;
	PopData m_popData;

	PopIdeologyDialog* m_ideologyWidget;

	void setupComboBox(QComboBox* combobox);
};

