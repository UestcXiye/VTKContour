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
	// vtkMultiBlockPLOT3DReader 是一个读取器对象，用于读取 PLOT3D 格式的文件并在输出时生成结构化网格
	vtkSmartPointer<vtkMultiBlockPLOT3DReader> plot3dReader = vtkSmartPointer<vtkMultiBlockPLOT3DReader>::New();
	plot3dReader->SetXYZFileName("combxyz.bin");
	plot3dReader->SetQFileName("combq.bin");
	plot3dReader->SetScalarFunctionNumber(100);
	plot3dReader->SetVectorFunctionNumber(202);
	qDebug() << plot3dReader->GetOutput()->GetNumberOfBlocks(); // 0
	// 反向更新管线
	plot3dReader->Update();
	qDebug() << plot3dReader->GetOutput()->GetNumberOfBlocks(); // 1
	vtkDataSet* plot3dOutput = (vtkDataSet*)(plot3dReader->GetOutput()->GetBlock(0));
	// 获取标量数据
	vtkDataArray* scalars = plot3dOutput->GetPointData()->GetScalars();
	double* range = scalars->GetRange();
	qDebug() << range[0] << ":" << range[1];

	// 2. filter
	// 产生结构化栅格边界的一个线轮廓
	vtkSmartPointer<vtkStructuredGridOutlineFilter> outline = vtkSmartPointer<vtkStructuredGridOutlineFilter>::New();
	outline->SetInputData(plot3dOutput);
	// 等值面提取
	vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
	contour->SetInputData(plot3dOutput);
	// 在 range[0] 到 range[1] 的范围内提取 5 个等值面
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
	// 二次曲面
	vtkSmartPointer<vtkQuadric> quadric = vtkSmartPointer<vtkQuadric>::New();
	quadric->SetCoefficients(0.5, 1, 0.2, 0, 0.1, 0, 0, 0.2, 0, 0);
	// 对二次曲面进行等间隔逐点采样
	vtkSmartPointer<vtkSampleFunction> sample = vtkSmartPointer<vtkSampleFunction>::New();
	// 设置采样分辨率为 30*30*30
	sample->SetSampleDimensions(30, 30, 30);
	sample->SetImplicitFunction(quadric);

	// 2. filter
	// 等值面提取，将采集到的空间点，转成 vtkPolyData 型对象
	vtkSmartPointer<vtkContourFilter> contour = vtkSmartPointer<vtkContourFilter>::New();
	contour->SetInputConnection(sample->GetOutputPort());
	// 在 [0, 1.2] 的范围内提取 5 个等值面
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
