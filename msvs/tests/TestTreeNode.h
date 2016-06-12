#pragma once

class TestTreeNode : public QObject
{
	Q_OBJECT
private slots:
	void emptyConstructor();
	void tree();
	void splitChild();
};
