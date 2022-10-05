#include "LidarMaster.h"
#include <iostream>
#include <string>
#include <QVTKOpenGLNativeWidget.h>
#include "vtkGenericOpenGLRenderWindow.h"
#include "lasreader.hpp"
#include "laswriter.hpp"
#include <pcl/filters/voxel_grid.h>
#include <pcl/filters/approximate_voxel_grid.h>
#include <pcl/filters/random_sample.h>
#include <pcl/filters/uniform_sampling.h>
#include <pcl/filters/statistical_outlier_removal.h>
LidarMaster::LidarMaster(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	//��ʼ�������ļ���
	m_PtrProTree = new QTreeWidget(this);
	//�����ļ���ͷΪ��
	m_PtrProTree->setHeaderHidden(true);

	m_PtrLasInfoTree = new QTreeWidget(this);


	connect(m_PtrProTree, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(treeItemClickedSlot(QTreeWidgetItem*, int)));
	// ��������������
	MainFramAttri();
	// ��ʼ���˵���ָ��
	m_PtrMenu = new LidarMenu(this, this);
	this->setMenuBar(m_PtrMenu);
	m_LidarWidget = new QVTKOpenGLNativeWidget;
	/*m_PtrQVtkWindow = new QVTKWindow(this, this);*/

	m_dockMain->setWidget(m_LidarWidget);
	
	renderer2 = vtkSmartPointer<vtkRenderer>::New();
    renderWindow2 = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    renderWindow2->AddRenderer(renderer2);
    viewer.reset(new pcl::visualization::PCLVisualizer(renderer2, renderWindow2, "viewer", false));
	m_mapCloud.clear();
}

LidarMaster::~LidarMaster()
{
	if (m_MainThread != nullptr)
	{
		if (m_MainThread->joinable())
		{
			m_MainThread->join();
		}
		m_MainThread = nullptr;
	}
}

void LidarMaster::recvRenderCoords(QString& strAxis)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr tmpCloud;
	tmpCloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
	getPtCLoud(m_mapCloud[m_nCloudIndex], tmpCloud);
	if (m_mapCloud.size() > 0 && tmpCloud->points.size() > 0)
    {
        pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZ> render(tmpCloud, strAxis.toStdString());
		viewer->updatePointCloud(tmpCloud, render, QString::number(m_nCloudIndex).toStdString());
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, QString::number(m_nCloudIndex).toStdString());
		viewer->spin();
		update();
    }

}
void LidarMaster::recColorInfoSlot(QColor& color)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr tmpCloud;
	tmpCloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
	getPtCLoud(m_mapCloud[m_nCloudIndex], tmpCloud);
	if (m_mapCloud.size() >0 && !tmpCloud->empty())
	{
		pcl::visualization::PointCloudColorHandlerCustom<pcl::PointXYZ> render(tmpCloud, color.redF() * 255, color.greenF() * 255, color.blueF() * 255);
		viewer->updatePointCloud(tmpCloud, render, QString::number(m_nCloudIndex).toStdString());
		viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, QString::number(m_nCloudIndex).toStdString());
		viewer->spin();
		update();
	}
	
	
	

}
int LidarMaster::savePtCloud(pcl::PointCloud<pcl::PointXYZ>::Ptr tmpCloud, const QString& saveFileName)
{
	int saveRes = 1;
	if (saveFileName.endsWith(".pcd", Qt::CaseInsensitive))
	{
		saveRes = pcl::io::savePCDFileBinary(saveFileName.toStdString(), *tmpCloud);
	}
	if (saveFileName.endsWith(".ply", Qt::CaseInsensitive))
	{
		saveRes = pcl::io::savePLYFileBinary(saveFileName.toStdString(), *tmpCloud);
	}

	if (saveRes != 0)
	{
		PCL_ERROR("Error writing point cloud %s\n", saveFileName.toStdString().c_str());
		return -1;
	}
	else
	{
		return 0;
	}
}

int LidarMaster::saveCloudToMap(QString& lasFile)
{
	if (m_mapCloud.size() <=0)
	{
		m_mapCloud.insert(0, lasFile);
		return 0;
	}
	else
	{
		QList<int> keys = {};
		keys = m_mapCloud.keys();
		int mm = 0;
		mm = keys[m_mapCloud.keys().size() - 1];
		m_mapCloud.insert(mm+1 , lasFile);

		return mm + 1;
	}
}
void LidarMaster::recvFilterVal(int type, double filterVal,QString& lasFile)
{
	pcl::PointCloud<pcl::PointXYZ>::Ptr tmpCloud;
	tmpCloud.reset(new pcl::PointCloud<pcl::PointXYZ>);

	getPtCLoud(lasFile, tmpCloud);
	
	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out(new pcl::PointCloud<pcl::PointXYZ>());
	QString saveFileName = QFileDialog::getSaveFileName(this,QStringLiteral("�����˲����"), QStringLiteral("./result.pcd"), QStringLiteral("��������(*.ply *.pcd)"));
	

	if (saveFileName.isEmpty()) return;

	QString outFileName = "";
	//����
	if (type == 1)
	{
		pcl::VoxelGrid<pcl::PointXYZ> voxel_Grid;
		voxel_Grid.setLeafSize(filterVal, filterVal, filterVal);
		voxel_Grid.setInputCloud(tmpCloud);
		voxel_Grid.filter(*cloud_out);
		outFileName = "vg";
	}
	// ��������
	else if (type == 2)
	{
		pcl::ApproximateVoxelGrid<pcl::PointXYZ> appVoxel_Grid;
		appVoxel_Grid.setLeafSize(filterVal, filterVal, filterVal);
		appVoxel_Grid.setInputCloud(tmpCloud);
		appVoxel_Grid.filter(*cloud_out);
		outFileName = "avg";
	}
	int saveRes = 1;
	saveFileName = QFileInfo(saveFileName).absolutePath() + "/" + QFileInfo(saveFileName).baseName() + "_" + outFileName + "." + QFileInfo(saveFileName).suffix();
	saveRes = savePtCloud(cloud_out, saveFileName);

	saveCloudToMap(saveFileName);

	if (saveRes != 0)
	{
		PCL_ERROR("Error writing point cloud %s\n", saveFileName.toStdString().c_str());
		return;
	}
	else
	{
		QTreeWidgetItem* item11 = new QTreeWidgetItem();
		item11->setIcon(0, QIcon(":/LidarMaster/img/las.png"));
		item11->setCheckState(0, Qt::Unchecked);
		item11->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsAutoTristate);
		item11->setText(0, QFileInfo(saveFileName).fileName());
		item11->setData(0, Qt::UserRole + 1, saveFileName);
		m_PtrProTree->topLevelItem(0)->addChild(item11);
		QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("�˲����"), QStringLiteral("�ر�"));
		emit closeFilterDialogSignal();
	}
}
void LidarMaster::rectoLeftSlot()
{
	if (m_mapCloud.size() <= 1)
		return;
	if (m_nCloudIndex.load() == 0)
		return;
}
void LidarMaster::rectoRightSlot()
{
	if (m_mapCloud.size() <= 1)
		return;
	if (m_nCloudIndex.load() == 0)
		return;
}
void LidarMaster::recvGridAndType(int gridVal, int type)
{
	if (m_mapCloud.size() > 0)
	{
		pcl::PointCloud<pcl::PointXYZ>::Ptr tmpCloud;
		tmpCloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
		getPtCLoud(m_mapCloud[m_nCloudIndex], tmpCloud);
		pcl::PointCloud<pcl::PointXYZ>::Ptr cloud_out(new pcl::PointCloud<pcl::PointXYZ>());
		QString saveFileName = QFileDialog::getSaveFileName(this, QStringLiteral("�����˲����"), QStringLiteral("./result.pcd"), QStringLiteral("��������(*.ply *.pcd)"));
		if (saveFileName.isEmpty()) return;
		QString outFileName = "";

		if (0 == type)
		{
			pcl::RandomSample<pcl::PointXYZ> res;
			res.setInputCloud(tmpCloud);
			res.setSample(gridVal);
			res.filter(*cloud_out);
			outFileName = "rs";
		}
		else if (1 == type)
		{
			pcl::UniformSampling<pcl::PointXYZ> uni;
			uni.setRadiusSearch(gridVal);
			uni.setInputCloud(tmpCloud);
			uni.filter(*cloud_out);
			outFileName = "us";
		}
		else if (2 == type)
		{
			pcl::StatisticalOutlierRemoval<pcl::PointXYZ> sor;
			sor.setInputCloud(tmpCloud);
			sor.setMeanK(gridVal);
			sor.setStddevMulThresh(1);
			
			sor.filter(*cloud_out);
			outFileName = "sor";
		}

		int saveRes = 1;
		saveFileName = QFileInfo(saveFileName).absolutePath() + "/" + QFileInfo(saveFileName).baseName() + "_" + outFileName + "." + QFileInfo(saveFileName).suffix();
		saveRes = savePtCloud(cloud_out, saveFileName);

		saveCloudToMap(saveFileName);

		if (saveRes != 0)
		{
			PCL_ERROR("Error writing point cloud %s\n", saveFileName.toStdString().c_str());
			return;
		}
		else
		{
			QTreeWidgetItem* item11 = new QTreeWidgetItem();
			item11->setIcon(0, QIcon(":/LidarMaster/img/las.png"));
			item11->setCheckState(0, Qt::Unchecked);
			item11->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsAutoTristate);
			item11->setText(0, QFileInfo(saveFileName).fileName());
			item11->setData(0, Qt::UserRole + 1, saveFileName);
			m_PtrProTree->topLevelItem(0)->addChild(item11);
			QMessageBox::information(this, QStringLiteral("��ʾ"), QStringLiteral("�˲����"), QStringLiteral("�ر�"));
			emit closeGridFilterDialogSignal();
		}
	}


}
void LidarMaster::MainFramAttri()
{
	// �������
	setWindowTitle(QStringLiteral("LidarMaster"));
	// ���ͼ��
	setWindowIcon(QIcon(":/LidarMaster/img/logo.png"));
	// ���ô�С
	resize(QGuiApplication::primaryScreen()->availableSize() * 16 / 9);

	// ɾ�����봰��
	QWidget* centralWidget = takeCentralWidget();
	if (centralWidget)
		delete centralWidget;
	//����Ƕ��dock
	setDockNestingEnabled(true);

	// ����״̬��
	QStatusBar* stBar = statusBar();
	setStatusBar(stBar);
	//�����ʾ��Ϣ���Ҳ�
	QLabel* lab_cprt = new QLabel(QStringLiteral("@��Ȩ����1114809057@qq.com"), this);
	lab_cprt->setStyleSheet("color:#000000;margin-right:10px;");
	stBar->addPermanentWidget(lab_cprt);
	stBar->setStyleSheet("background:#ffffff;border: 1px solid black;");

	// ������ʾ����
	m_dockMain = new QDockWidget(this);
	m_dockMain->setWindowTitle(QStringLiteral("����"));
	// ����Ϊ���ƶ��ɸ����������ɹر�
	m_dockMain->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	// ���ƶ���Χ������
	m_dockMain->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);  

	// ������Ϣ
	m_dockProInfo = new QDockWidget(this);
	m_dockProInfo->setWindowTitle(QStringLiteral("����"));
	m_dockProInfo->setFeatures(QDockWidget::DockWidgetClosable);

	// ������Ϣ
	m_dockLasInfo = new QDockWidget(this);
	m_dockLasInfo->setWindowTitle(QStringLiteral("����"));
	m_dockLasInfo->setFeatures(QDockWidget::AllDockWidgetFeatures);

	// ������Ϣ
	m_dockOtherInfo = new QDockWidget(this);
	m_dockOtherInfo->setWindowTitle(QStringLiteral("����"));
	m_dockOtherInfo->setFeatures(QDockWidget::AllDockWidgetFeatures);

	// �����Ӵ����С
	m_dockProInfo->setFixedWidth(200);
	m_dockLasInfo->setFixedWidth(200);
	m_dockOtherInfo->setFixedWidth(200);
	m_dockLasInfo->setMaximumHeight(400);

	// ���ַ���
	setCentralWidget(m_dockMain);
	addDockWidget(Qt::LeftDockWidgetArea, m_dockProInfo);
	addDockWidget(Qt::RightDockWidgetArea, m_dockLasInfo);
	addDockWidget(Qt::RightDockWidgetArea, m_dockOtherInfo);

	// ���ַ���
	splitDockWidget(m_dockLasInfo, m_dockOtherInfo, Qt::Vertical);

	connect(m_dockLasInfo, SIGNAL(visibilityChanged(bool)), this, SLOT(isLasInfo()));
	connect(m_dockOtherInfo, SIGNAL(visibilityChanged(bool)), this, SLOT(isOtherInfo()));
	connect(m_dockProInfo, SIGNAL(visibilityChanged(bool)), this, SLOT(isProInfo()));
	m_dockProInfo->setWidget(m_PtrProTree);
	setLasInfoDock();

}
void LidarMaster::setLasInfoDock()
{
	QStringList strList;
	strList << QStringLiteral("����") << QStringLiteral("ֵ");
	m_PtrLasInfoTree->setHeaderLabels(strList);

	QTreeWidgetItem* parentItem1 = new QTreeWidgetItem(QStringList() << QStringLiteral("������Ϣ"));
	parentItem1->setIcon(0, QIcon(":/LidarMaster/img/info.png"));
	m_PtrLasInfoTree->addTopLevelItem(parentItem1);
	QTreeWidgetItem* item11 = new QTreeWidgetItem();
	item11->setText(0, QStringLiteral("���Ƹ���"));
	item11->setText(1, QStringLiteral(""));

	QTreeWidgetItem* item12 = new QTreeWidgetItem();
	item12->setText(0, QStringLiteral("���Ƹ�ʽ"));
	item12->setText(1, QStringLiteral(""));
	QTreeWidgetItem* item13 = new QTreeWidgetItem();
	item13->setText(0, QStringLiteral("��������"));
	item13->setText(1, QStringLiteral(""));

	
	
	parentItem1->addChild(item11);
	parentItem1->addChild(item12);
	parentItem1->addChild(item13);

	QTreeWidgetItem* parentItem2 = new QTreeWidgetItem(QStringList() << QStringLiteral("�߶���Ϣ"));
	parentItem2->setIcon(0, QIcon(":/LidarMaster/img/height.png"));
	m_PtrLasInfoTree->addTopLevelItem(parentItem2);
	QTreeWidgetItem* item21 = new QTreeWidgetItem();
	item21->setText(0, QStringLiteral("���߶�"));
	item21->setText(1, QStringLiteral(""));

	QTreeWidgetItem* item22 = new QTreeWidgetItem();
	item22->setText(0, QStringLiteral("��С�߶�"));
	item22->setText(1, QStringLiteral(""));
	QTreeWidgetItem* item23 = new QTreeWidgetItem();
	item23->setText(0, QStringLiteral("ƽ���߶�"));
	item23->setText(1, QStringLiteral(""));

	parentItem2->addChild(item21);
	parentItem2->addChild(item22);
	parentItem2->addChild(item23);

	m_dockLasInfo->setWidget(m_PtrLasInfoTree);
}

void LidarMaster::isProInfo()
{
	if (m_dockProInfo->isHidden())
	{
		m_PtrMenu->m_isShowProInfoAct->setIcon(QIcon(":/LidarMaster/img/hide.png"));
	}
	else
	{
		m_PtrMenu->m_isShowProInfoAct->setIcon(QIcon(":/LidarMaster/img/show.png"));
	}
}

void LidarMaster::isOtherInfo()
{
	if (m_dockOtherInfo->isHidden())
	{
		m_PtrMenu->m_isShowOtherInfoAct->setIcon(QIcon(":/LidarMaster/img/hide.png"));
	}
	else
	{
		m_PtrMenu->m_isShowOtherInfoAct->setIcon(QIcon(":/LidarMaster/img/show.png"));
	}
}

void LidarMaster::treeItemClickedSlot(QTreeWidgetItem* item, int col)
{

	
	
	if (item->parent())
	{
		if (item->checkState(0)==Qt::Checked)
		{
			QString lidarFile = item->data(0, Qt::UserRole + 1).toString();

			int count = 0;
			for (auto iter = m_mapCloud.begin(); iter != m_mapCloud.end(); iter++)
			{
				QString fileName = iter.value();
				if (fileName == lidarFile)
				{
					count++;
				}
			}
			int index = 0;
			if (0 == count)
			{
				saveCloudToMap(lidarFile);
			}
			m_nCloudIndex = getIndex(lidarFile);
			showLidarData(lidarFile, m_nCloudIndex);
		}
		else
		{
			viewer->removeAllPointClouds();
			viewer->removeAllShapes();
			viewer->spin();
		}
	}
}
int LidarMaster::getIndex(QString& file)
{
	for (auto iter = m_mapCloud.begin(); iter != m_mapCloud.end(); iter++)
	{
		QString fileName = iter.value();
		if (fileName == file)
		{
			return iter.key();
		}
	}
}


void LidarMaster::showLidarData(QString& lidarFile, int col)
{

	viewer->removeAllPointClouds();
	viewer->removeAllShapes();

//#if VTK_MAJOR_VERSION > 8
	m_LidarWidget->setRenderWindow(viewer->getRenderWindow());
    viewer->setupInteractor(m_LidarWidget->interactor(), m_LidarWidget->renderWindow());
//#else
//    viewer.reset(new pcl::visualization::PCLVisualizer("viewer", false));
//    this->SetRenderWindow(viewer->getRenderWindow());
//    viewer->setupInteractor(this->GetInteractor(), this->GetRenderWindow());
//#endif

    if (lidarFile.isEmpty())
    {
        return;
    }

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
	cloud.reset(new pcl::PointCloud<pcl::PointXYZ>);
	getPtCLoud(lidarFile, cloud);
	int index = m_nCloudIndex;

	
	m_PtrLasInfoTree->topLevelItem(0)->child(0)->setText(1, QString::number(cloud->points.size()));
	m_PtrLasInfoTree->topLevelItem(0)->child(1)->setText(1, QFileInfo(lidarFile).suffix());
	if (cloud->height == 1)
	{
		m_PtrLasInfoTree->topLevelItem(0)->child(2)->setText(1, QStringLiteral("�������"));
	}
	else
	{
		m_PtrLasInfoTree->topLevelItem(0)->child(2)->setText(1, QStringLiteral("�������"));
	}
	if (m_MainThread != nullptr)
		m_MainThread->join();
	m_MainThread.reset(new std::thread([=] {showPtCloudHeightInfo(); }));
   
    // ��ʾ���ͼ

    viewer->setBackgroundColor(0, 0, 0); //���ñ���
    // viewer->addCoordinateSystem (15.0); //��������ϵ
    pcl::visualization::PointCloudColorHandlerGenericField<pcl::PointXYZ> fildColor(cloud, "z");
    viewer->addPointCloud<pcl::PointXYZ>(cloud, fildColor, QString::number(index).toStdString());
    viewer->setPointCloudRenderingProperties(pcl::visualization::PCL_VISUALIZER_POINT_SIZE, 1, QString::number(index).toStdString());
    viewer->resetCamera();
	m_LidarWidget->update();
	viewer->spin();
	
}

void LidarMaster::showPtCloudHeightInfo()
{
	double aveHeight = 0, maxHeight = 0,minHeight  = DBL_MAX;

	pcl::PointCloud<pcl::PointXYZ>::Ptr cloud;
	cloud.reset(new pcl::PointCloud<pcl::PointXYZ>);

	getPtCLoud(m_mapCloud[m_nCloudIndex], cloud);


	if (m_mapCloud.size() >0 &&!cloud->empty())
	{	
		double totalHeight = 0;
		for (int i = 0; i < cloud->points.size(); i++)
		{
			maxHeight = maxHeight > cloud->points[i].z ? maxHeight : cloud->points[i].z;
			minHeight = minHeight > cloud->points[i].z ? cloud->points[i].z : minHeight;
			totalHeight += cloud->points[i].z;
		}
		aveHeight = totalHeight / (cloud->points.size()*1.0);

		m_PtrLasInfoTree->topLevelItem(1)->child(0)->setText(1, QString::number(maxHeight));
		m_PtrLasInfoTree->topLevelItem(1)->child(1)->setText(1, QString::number(minHeight));
		m_PtrLasInfoTree->topLevelItem(1)->child(2)->setText(1, QString::number(aveHeight));
	}
}

void LidarMaster::getPtCLoud(QString& lidarFile,pcl::PointCloud<pcl::PointXYZ>::Ptr cloud)
{
	if (QFileInfo(lidarFile).suffix().contains("pcd"))
	{
		pcl::PCDReader reader;
		reader.read(lidarFile.toStdString(), *cloud);
	}
	else if (QFileInfo(lidarFile).suffix().contains("ply"))
	{
		pcl::PolygonMesh meshData;//��ȡԭʼ����
		pcl::io::loadPolygonFile(lidarFile.toStdString(), meshData);
		pcl::fromPCLPointCloud2(meshData.cloud, *cloud);//��obj����ת��Ϊ��������
	}
	else if (QFileInfo(lidarFile).suffix().contains("las"))
	{
		LASreadOpener lasreadopener;
		QByteArray ba = lidarFile.toLatin1();
		lasreadopener.set_file_name(ba.data());
		LASreader* lasreader = lasreadopener.open(false);
		size_t ct = lasreader->header.number_of_point_records;
		cloud->points.resize(ct);
		cloud->width = 1;
		cloud->height = ct;
		cloud->is_dense = false;
		size_t i = 0;
		while (lasreader->read_point() && i < ct)
		{
			cloud->points[i].x = lasreader->point.get_x();
			cloud->points[i].y = lasreader->point.get_y();
			cloud->points[i].z = lasreader->point.get_z();
			++i;
		}
	}
}




void LidarMaster::isLasInfo()
{
	if (m_dockLasInfo->isHidden())
	{
		m_PtrMenu->m_isShowLasInfoAct->setIcon(QIcon(":/LidarMaster/img/hide.png"));
	}
	else
	{
		m_PtrMenu->m_isShowLasInfoAct->setIcon(QIcon(":/LidarMaster/img/show.png"));
	}
}
