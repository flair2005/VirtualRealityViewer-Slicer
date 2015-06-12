/*==============================================================================

  Program: 3D Slicer

  Portions (c) Copyright Brigham and Women's Hospital (BWH) All Rights Reserved.

  See COPYRIGHT.txt
  or http://www.slicer.org/copyright/copyright.txt for details.

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

==============================================================================*/

#include "vtkSlicerVirtualRealityViewerLogic.h"

#include <cmath>

// Qt includes
#include <QDebug>
#include <QtCore>
#include <QDesktopWidget>
#include <QRect>
#include <QTimer>

#include <vtkMRMLScene.h>

// SlicerQt includes
#include "qSlicerVirtualRealityViewerModuleWidget.h"
#include "ui_qSlicerVirtualRealityViewerModule.h"
#include "qSlicerLayoutManager.h"
#include <qSlicerApplication.h>
#include <qSlicerIOManager.h>
#include "qSlicerWidget.h"

#include <QVTKWidget.h>

#include <vtkMRMLViewNode.h>
#include <qMRMLThreeDView.h>
#include <qMRMLThreeDWidget.h>
#include "qMRMLSliderWidget.h"
#include <vtkMRMLCameraNode.h>

#include <QApplication>
#include <QTimer>

#include <vtkMRMLNode.h>
#include <vtkMRMLModelNode.h>
#include <vtkMRMLModelDisplayNode.h>

#include <vtkNew.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkFloatArray.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkMath.h>
#include <vtkMRMLLinearTransformNode.h>
#include <vtkPerspectiveTransform.h>
#include <vtkLineSource.h>
#include <vtkTubeFilter.h>
#include "vtkMRMLScalarVolumeDisplayNode.h"
#include "vtkMRMLSliceNode.h"

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerVirtualRealityViewerModuleWidgetPrivate: public Ui_qSlicerVirtualRealityViewerModule
{
public:
  qSlicerVirtualRealityViewerModuleWidgetPrivate();
  vtkSlicerVirtualRealityViewerLogic* logic() const;
};

//-----------------------------------------------------------------------------
// qSlicerVirtualRealityViewerModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerVirtualRealityViewerModuleWidgetPrivate::qSlicerVirtualRealityViewerModuleWidgetPrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerVirtualRealityViewerModuleWidget methods
//-----------------------------------------------------------------------------
qSlicerVirtualRealityViewerModuleWidget::qSlicerVirtualRealityViewerModuleWidget(QWidget* _parent) : Superclass( _parent ) , d_ptr( new qSlicerVirtualRealityViewerModuleWidgetPrivate)
{

}

//-----------------------------------------------------------------------------
qSlicerVirtualRealityViewerModuleWidget::~qSlicerVirtualRealityViewerModuleWidget()
{
  //delete riftWindow;
}

//-----------------------------------------------------------------------------
void qSlicerVirtualRealityViewerModuleWidget::onStart()
{
  Q_D(qSlicerVirtualRealityViewerModuleWidget);

  //riftWindow = new qMRMLThreeDWidget();
  //riftWindow->setMRMLViewNode(vtkMRMLViewNode::SafeDownCast(d->viewNodeComboBox->currentNode()));
  //riftWindow->showNormal();
  //riftWindow->setMRMLScene(this->mrmlScene());

  //qMRMLThreeDView* threeDView = riftWindow->threeDView();
  qMRMLThreeDView* threeDView = new qMRMLThreeDView();
  threeDView->setMRMLViewNode(vtkMRMLViewNode::SafeDownCast(d->viewNodeComboBox->currentNode()));
  //vtkRenderer* activeRenderer = riftWindow->threeDView()->renderer(); 
  vtkRenderer* activeRenderer = threeDView->renderer();  
  vtkRenderWindow* activeRenderWindow = activeRenderer->GetRenderWindow();
  vtkCamera* camera = activeRenderer->GetActiveCamera();

  camera->SetPosition(0, 0, 0);
  camera->SetFocalPoint(0, 0.1, 0);
  camera->SetViewUp(0, 0, 1);
  camera->UseHorizontalViewAngleOn();
  camera->SetViewAngle(90);
  camera->SetClippingRange(0.3, 500);
  activeRenderWindow->Render();

  int *size = activeRenderWindow->GetSize();
  unsigned char* image = new unsigned char [3*size[0]*size[1]];
  image = activeRenderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  cout << size[0] << " " << size[1] << endl;
  FILE *binFile = fopen("C:/Work/Image1","wb");
  fwrite(image, sizeof(unsigned char), 3*size[0]*size[1], binFile);
  fclose(binFile);

  

  delete threeDView;
}

//-----------------------------------------------------------------------------
void qSlicerVirtualRealityViewerModuleWidget::enter()
{
  this->Superclass::enter();
}

//-----------------------------------------------------------------------------
void qSlicerVirtualRealityViewerModuleWidget::setup()
{
  Q_D(qSlicerVirtualRealityViewerModuleWidget);
  d->setupUi(this);
  
  this->Superclass::setup();
  
  connect( d->startButton, SIGNAL( clicked() ), this, SLOT( onStart() ) );
}
