#pragma once

#include <QWidget>
#include "ui_LidarNewPro.h"
#include "LidarMaster.h"
class LidarMaster;

class LidarNewPro : public QWidget
{
	Q_OBJECT

public:
	LidarNewPro(QWidget *parent = nullptr);
	~LidarNewPro();
	void setLidarMatser(LidarMaster* Matser);
	void addTrees(const QString& lasPath, const QString& proPath);

private:
	Ui::LidarNewProClass ui;
	LidarMaster* m_PtrLidarMaster;

	QString m_proPath;
	QString m_lasPath;
private slots:

	// ѡ�񹤳�·��
	void on_tb_proPath_clicked();

	// ѡ�����·��
	void on_tb_OpenLas_clicked();
	// ȷ����������
	void on_tb_OK_clicked();

};
