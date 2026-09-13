#pragma once

#include "PopEditorDialog.h"

class PopSaveEditorDialog : public PopEditorDialog
{
	Q_OBJECT;
public:
	PopSaveEditorDialog(QWidget* parent = nullptr);

	void setPopData(const PopData& pop) override;

	void setIdeologies(const QList<Ideology>& ideologies) override;

private slots:
	void setupStyle() override;
	void setupConnections();
private:
	QLineEdit* m_militancyEdit;
	QLineEdit* m_literacyEdit;
	QLineEdit* m_consciousnessEdit;
	QPushButton* m_edit_ideologies;
	PopIdeologyDialog* m_ideologyWidget;
};