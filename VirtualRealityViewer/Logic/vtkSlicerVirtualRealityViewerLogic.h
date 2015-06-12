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

// .NAME vtkSlicerVirtualRealityViewerLogic
// .SECTION Description
#ifndef __vtkSlicerVirtualRealityViewerLogic_h
#define __vtkSlicerVirtualRealityViewerLogic_h

// Slicer includes
#include "vtkSlicerModuleLogic.h"

// MRML includes
#include "vtkMRMLSliceNode.h"
#include "qMRMLSliceWidget.h"
#include <qSlicerApplication.h>
#include <vtkRenderer.h>

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>

// STD includes
#include <cstdlib>

#include "vtkSlicerVirtualRealityViewerModuleLogicExport.h"

/// \ingroup Slicer_QtModules_ExtensionTemplate
class VTK_SLICER_VIRTUALREALITYVIEWER_MODULE_LOGIC_EXPORT vtkSlicerVirtualRealityViewerLogic :
  public vtkSlicerModuleLogic
{
public:

  static vtkSlicerVirtualRealityViewerLogic *New();
  vtkTypeMacro(vtkSlicerVirtualRealityViewerLogic, vtkSlicerModuleLogic);
  void PrintSelf(ostream& os, vtkIndent indent);
  
  void CreateImage(vtkRenderer*, vtkRenderWindow*);
  unsigned char* CubemapToEquirectangular(unsigned char* cube[]);

protected:
  vtkSlicerVirtualRealityViewerLogic();
  virtual ~vtkSlicerVirtualRealityViewerLogic();

  virtual void SetMRMLSceneInternal(vtkMRMLScene* newScene);
  /// Register MRML Node classes to Scene. Gets called automatically when the MRMLScene is attached to this logic class.
  
private:

  vtkSlicerVirtualRealityViewerLogic(const vtkSlicerVirtualRealityViewerLogic&); // Not implemented
  void operator=(const vtkSlicerVirtualRealityViewerLogic&);               // Not implemented
};

#endif
