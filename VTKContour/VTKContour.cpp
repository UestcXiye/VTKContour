/*
#include "VTKContour.h"

#include <vtkConeSource.h>
#include <vtkSTLReader.h>
#include <vtkMultiBlockPLOT3DReader.h>
#include <vtkDataSet.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkShrinkPolyData.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkStructuredGridOutlineFilter.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

VTKContour::VTKContour(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	_pVTKWidget = new QVTKOpenGLNativeWidget();
	this->setCentralWidget(_pVTKWidget);
	// this->showMaximized();

	// 1. generate data
	// vtkSmartPointer<vtkConeSource> cone = vtkSmartPointer<vtkConeSource>::New();
	// or, read data
	// vtkMultiBlockPLOT3DReader ��һ����ȡ���������ڶ�ȡ PLOT3D ��ʽ���ļ��������ʱ���ɽṹ������
	vtkSmartPointer<vtkMultiBlockPLOT3DReader> plot3dReader = vtkSmartPointer<vtkMultiBlockPLOT3DReader>::New();
	plot3dReader->SetXYZFileName("combxyz.bin");
	plot3dReader->SetQFileName("combq.bin");
	plot3dReader->SetScalarFunctionNumber(100);
	plot3dReader->SetVectorFunctionNumber(202);
	qDebug() << plot3dReader->GetOutput()->GetNumberOfBlocks(); // 0
	// ������¹���
	plot3dReader->Update();
	qDebug() << plot3dReader->GetOutput()->GetNumberOfBlocks(); // 1
	vtkDataSet* plot3dOutput = (vtkDataSet*)(plot3dReader->GetOutput()->GetBlock(0));
	// ��ȡ��������
	vtkDataArray* scalars = plot3dOutput->GetPointData()->GetScalars();
	double* range = scalars->GetRange();
	qDebug() << range[0] << ":" << range[1];

	// 2. filter
	// �����ṹ��դ��߽��һ��������
	vtkSmartPointer<vtkStructuredGridOutlineFilter> outline = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
	outline->SetInputData(plot3dOutput);
	// ��ֵ����ȡ
	vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
	contour->SetInputData(plot3dOutput);
	// �� range[0] �� range[1] �ķ�Χ����ȡ 5 ����ֵ��
	contour->GenerateValues(5, range[0], range[1]);

	// 3. mapper
	vtkSmartPointer<vtkPolyDataMapper> outlineMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	vtkSmartPointer<vtkPolyDataMapper> contourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	// 4. actor
	vtkSmartPointer<vtkActor> outlineActor = vtkSmartPointer<vtkActor>::New();
	vtkSmartPointer<vtkActor> contourActor = vtkSmartPointer<vtkActor>::New();

	// 5. renderer
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.3, 0.6, 0.3); // Background Color: Green

	// 6. connect
	outlineMapper->SetInputConnection(outline->GetOutputPort());
	contourMapper->SetInputConnection(contour->GetOutputPort());
	outlineActor->SetMapper(outlineMapper);
	contourActor->SetMapper(contourMapper);
	renderer->AddActor(outlineActor);
	renderer->AddActor(contourActor);

	this->_pVTKWidget->renderWindow()->AddRenderer(renderer);
	this->_pVTKWidget->renderWindow()->Render();
}

VTKContour::~VTKContour()
{}

*/

#include "VTKContour.h"

#include <vtkConeSource.h>
#include <vtkSTLReader.h>
#include <vtkMultiBlockPLOT3DReader.h>
#include <vtkDataSet.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkDataArray.h>
#include <vtkPointData.h>
#include <vtkShrinkPolyData.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkStructuredGridOutlineFilter.h>
#include <vtkContourFilter.h>
#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkLookupTable.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

VTKContour::VTKContour(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	_pVTKWidget = new QVTKOpenGLNativeWidget();
	this->setCentralWidget(_pVTKWidget);
	// this->showMaximized();

	// 1. generate data
	// ��������
	vtkSmartPointer<vtkQuadric> quadric = vtkSmartPointer<vtkQuadric>::New();
	quadric->SetCoefficients(0.5, 1, 0.2, 0, 0.1, 0, 0, 0.2, 0, 0);
	// �Զ���������еȼ��������
	vtkSmartPointer<vtkSampleFunction> sample = vtkSmartPointer<vtkSampleFunction>::New();
	// ���ò����ֱ���Ϊ 30*30*30
	sample->SetSampleDimensions(30, 30, 30);
	sample->SetImplicitFunction(quadric);

	// 2. filter
	// ��ֵ����ȡ�����ɼ����Ŀռ�㣬ת�� vtkPolyData �Ͷ���
	vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
	contour->SetInputConnection(sample->GetOutputPort());
	// �� [0, 1.2] �ķ�Χ����ȡ 5 ����ֵ��
	contour->GenerateValues(5, 0, 1.2);

	// 3. mapper
	vtkSmartPointer<vtkPolyDataMapper> contourMapper = vtkSmartPointer<vtkPolyDataMapper>::New();

	// 4. actor
	vtkSmartPointer<vtkActor> contourActor = vtkSmartPointer<vtkActor>::New();

	// 5. renderer
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(0.3, 0.6, 0.3); // Background Color: Green

	// 6. connect
	contourMapper->SetInputConnection(contour->GetOutputPort());
	contourActor->SetMapper(contourMapper);
	renderer->AddActor(contourActor);

	this->_pVTKWidget->renderWindow()->AddRenderer(renderer);
	this->_pVTKWidget->renderWindow()->Render();
}

VTKContour::~VTKContour()
{}
