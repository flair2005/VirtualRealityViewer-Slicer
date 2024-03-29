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

// Qt includes
#include <QDebug>
#include <QtCore>
#include <QDesktopWidget>
#include <QRect>
#include <QVTKWidget.h>

// SlicerQt includes
#include "qSlicerVirtualRealityViewerModuleWidget.h"
#include "ui_qSlicerVirtualRealityViewerModule.h"
#include "qSlicerLayoutManager.h"
#include <qSlicerApplication.h>
#include <qSlicerIOManager.h>
#include "qSlicerWidget.h"

// Logic includes
#include "vtkSlicerVirtualRealityViewerLogic.h"

// MRML includes
#include <vtkMRMLScene.h>
#include <vtkMRMLViewNode.h>
#include <vtkMRMLCameraNode.h>
#include <vtkMRMLMarkupsFiducialNode.h>

// MRMLWidgets includes
#include <qMRMLThreeDView.h>
#include <qMRMLThreeDWidget.h>

// VTK includes
#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

#include <windows.h>
#include <ShellApi.h>

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerVirtualRealityViewerModuleWidgetPrivate: public Ui_qSlicerVirtualRealityViewerModule
{
  Q_DECLARE_PUBLIC(qSlicerVirtualRealityViewerModuleWidget);
protected:
  qSlicerVirtualRealityViewerModuleWidget* const q_ptr;
public:
  qSlicerVirtualRealityViewerModuleWidgetPrivate(qSlicerVirtualRealityViewerModuleWidget& object);

  vtkSlicerVirtualRealityViewerLogic* logic() const;
};

//-----------------------------------------------------------------------------
// qSlicerVirtualRealityViewerModuleWidgetPrivate methods

//-----------------------------------------------------------------------------
qSlicerVirtualRealityViewerModuleWidgetPrivate::qSlicerVirtualRealityViewerModuleWidgetPrivate(qSlicerVirtualRealityViewerModuleWidget& object)
  : q_ptr(&object)
{
}

//-----------------------------------------------------------------------------
vtkSlicerVirtualRealityViewerLogic* qSlicerVirtualRealityViewerModuleWidgetPrivate::logic() const
{
  Q_Q(const qSlicerVirtualRealityViewerModuleWidget);
  return vtkSlicerVirtualRealityViewerLogic::SafeDownCast(q->logic());
}

//-----------------------------------------------------------------------------
// qSlicerVirtualRealityViewerModuleWidget methods
//-----------------------------------------------------------------------------
qSlicerVirtualRealityViewerModuleWidget::qSlicerVirtualRealityViewerModuleWidget(QWidget* _parent) : Superclass( _parent ) , d_ptr( new qSlicerVirtualRealityViewerModuleWidgetPrivate(*this))
{

}

//-----------------------------------------------------------------------------
qSlicerVirtualRealityViewerModuleWidget::~qSlicerVirtualRealityViewerModuleWidget()
{
  //delete riftWindow;
}

//-----------------------------------------------------------------------------
void qSlicerVirtualRealityViewerModuleWidget::onTimerUpdate()
{
  Q_D(qSlicerVirtualRealityViewerModuleWidget);
  
  qMRMLThreeDWidget* riftWindow = new qMRMLThreeDWidget();
  riftWindow->setMRMLViewNode(vtkMRMLViewNode::SafeDownCast(d->viewNodeComboBox->currentNode()));

  riftWindow->showNormal();
  riftWindow->hide();

  riftWindow->setMRMLScene(this->mrmlScene());
  riftWindow->setFixedSize(QSize(960, 975));

  //qMRMLThreeDView* threeDView = new qMRMLThreeDView();
  //threeDView->setMRMLViewNode(vtkMRMLViewNode::SafeDownCast(d->viewNodeComboBox->currentNode()));
  qMRMLThreeDView* threeDView = riftWindow->threeDView();
  threeDView->setBackgroundColor(Qt::black);
  threeDView->setBackgroundColor2(Qt::black);
 
  vtkRenderer* activeRenderer = threeDView->renderer();
  vtkRenderWindow* activeRenderWindow = activeRenderer->GetRenderWindow();
  vtkMRMLMarkupsFiducialNode* fiducialNode = vtkMRMLMarkupsFiducialNode::SafeDownCast(d->fiducialListComboBox->currentNode());
  //vtkMRMLCameraNode* cameraNode = vtkMRMLCameraNode::SafeDownCast(d->cameraNodeComboBox->currentNode());
  d->logic()->CreateImage(activeRenderWindow, fiducialNode, false);

  activeRenderWindow->OffScreenRenderingOn();
  delete riftWindow;
  //delete threeDView;
}


//-----------------------------------------------------------------------------
void qSlicerVirtualRealityViewerModuleWidget::onStart()
{
  //QTimer *timer = new QTimer(this);
  //connect(timer, SIGNAL(timeout()), this, SLOT(onTimerUpdate()));
  //timer->start(100);

  this->onTimerUpdate();

  ShellExecute(NULL, "open", "C:/Work/s/VRWeb/web2.html", NULL, NULL, SW_SHOWNORMAL);
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
