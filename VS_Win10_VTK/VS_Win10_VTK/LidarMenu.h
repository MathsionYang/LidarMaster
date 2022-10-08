#pragma once

#include <QObject>
#include <QMenuBar>
#include <QtWidgets>
#include "LidarMaster.h"
#include <QPointer>
#include <QToolBar>
#include <QAction>
#include "LidarNewPro.h"
#include "PtRenderWidget.h"
#include <qcolordialog.h>
#include <qcolor.h>
#include "PtFilterDialog.h"
#include "PtGridFilterDialog.h"
#include "PtKeyPointSIFT.h"
#include "PtKeyPointHarris.h"
class LidarMaster;
class LidarNewPro;
class PtCloudRender;
class LidarMenu  : public QMenuBar
{
	Q_OBJECT

public:
	LidarMenu(QWidget*parent, LidarMaster *lasMaster);
	~LidarMenu();

	// �Ƿ���ʾ��������
	QPointer<QAction> m_isShowProInfoAct;

	// �Ƿ���ʾ������Ϣ����
	QPointer<QAction> m_isShowLasInfoAct;

	// �Ƿ���ʾ������Ϣ����
	QPointer<QAction> m_isShowOtherInfoAct;

private:
	//�����ָ��
	LidarMaster *m_PtrLidarMaster;
	// ������
	QPointer<QToolBar> m_PtrToolBar;
	//�½�����
	QPointer<LidarNewPro>m_PtrNewPro;
	//��Ⱦ�Ի���
	QPointer< PtRenderWidget> m_PtrCloudRender;

	//�˲��Ի���
	QPointer<PtFilterDialog>m_PtrFilterDialog;

	//��������Ի���
	QPointer<PtGridFilterDialog>m_PtGridFilterDialog;

	//SIFT�ؼ�����ʾ�Ի���
	QPointer<PtKeyPointSIFT>m_PtSIFTKeyPoint;

	//Harris�ؼ�����ʾ�Ի���
	QPointer<PtKeyPointHarris>m_PtKeyPointHarris;
	
	// �ļ�ģ��
	void menu_File();
	// ��ͼģ��
	void menu_ShowWin();
	// ��������
	void menu_PtCloudSetup();
	//�����˲�
	void menu_PtFilter();

	// �ؼ�����ʾ
	void menu_showKeyPt();

	//������׼
	void menu_registPt();

	//���Ʒָ�
	void menu_ClipPt();

	//�����ؽ�
	void menu_reBuild();

	//���������
	void menu_about();

signals:
	void sendLidarColor(QColor& color);

private slots:
	//�½�����
	void File_new();
	//�½�����
	void Pro_Open();

	// �Ƿ���ʾ��������
	void showProInfo();
	// �Ƿ���ʾ������Ϣ����
	void showLasInfo();
	// �Ƿ���ʾ������Ϣ����
	void showOtherInfo();

	//��ʾ��Ⱦ�Ի���
	void showRenderDialog();
	// ��ʾ������ɫ��
	void showColorDialog();

	//�˲��Ի���
	void showFilterDialog();
	// �ؼ�����ʾ
	void showKeypoint();

	// �ؼ�����ʾ
	void  showHarrisKeypoint();
	// �²����Ի���
	void showGridFilterDialog();


};
