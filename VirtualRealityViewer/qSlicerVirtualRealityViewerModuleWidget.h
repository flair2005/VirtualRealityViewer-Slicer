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

#ifndef __qSlicerVirtualRealityViewerModuleWidget_h
#define __qSlicerVirtualRealityViewerModuleWidget_h

// SlicerQt includes
#include "qSlicerAbstractModuleWidget.h"
#include "qSlicerVirtualRealityViewerModuleExport.h"

// MRML includes

// VTK includes
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkMRMLCameraNode.h>
#include <vtkCamera.h>
#include <qMRMLThreeDWidget.h>
#include <vtkSmartPointer.h>
#include "vtkRenderer.h"
#include "vtkCamera.h"

class qSlicerVirtualRealityViewerModuleWidgetPrivate;
class vtkMRMLNode;

/// \ingroup Slicer_QtModules_ExtensionTemplate
class Q_SLICER_QTMODULES_VIRTUALREALITYVIEWER_EXPORT qSlicerVirtualRealityViewerModuleWidget :
  public qSlicerAbstractModuleWidget
{
  Q_OBJECT

public:

  typedef qSlicerAbstractModuleWidget Superclass;
  qSlicerVirtualRealityViewerModuleWidget(QWidget *parent=0);
  virtual ~qSlicerVirtualRealityViewerModuleWidget();

protected:
  QScopedPointer<qSlicerVirtualRealityViewerModuleWidgetPrivate> d_ptr;
  
  virtual void setup();
  virtual void enter();

  
protected slots:
  void onStart();


private:
  Q_DECLARE_PRIVATE(qSlicerVirtualRealityViewerModuleWidget);
  Q_DISABLE_COPY(qSlicerVirtualRealityViewerModuleWidget);
};

#endif
