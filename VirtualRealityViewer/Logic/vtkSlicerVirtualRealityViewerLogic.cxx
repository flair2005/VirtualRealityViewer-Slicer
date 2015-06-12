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
  
// VirtualRealityViewer Logic includes
#include "vtkSlicerVirtualRealityViewerLogic.h"

//Custom VTK includes
#include "vtkRenderWindow.h"

// MRML includes
#include "vtkMRMLModelDisplayNode.h"
#include "vtkMRMLModelNode.h"
#include <qMRMLThreeDView.h>
#include <qMRMLThreeDWidget.h>
#include "vtkMRMLCameraNode.h"

#include <qSlicerApplication.h>
#include "qSlicerLayoutManager.h"

// VTK includes
#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkCommand.h>
#include <vtkMRMLScene.h>
#include <vtkObjectFactory.h>
#include <vtkFloatArray.h>


// STD includes
#include <cassert>
#include <time.h>

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkSlicerVirtualRealityViewerLogic);

//----------------------------------------------------------------------------
vtkSlicerVirtualRealityViewerLogic::vtkSlicerVirtualRealityViewerLogic()
{

}

//----------------------------------------------------------------------------
vtkSlicerVirtualRealityViewerLogic::~vtkSlicerVirtualRealityViewerLogic()
{

}

//----------------------------------------------------------------------------
void vtkSlicerVirtualRealityViewerLogic::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf( os, indent );
}

//---------------------------------------------------------------------------
void vtkSlicerVirtualRealityViewerLogic::SetMRMLSceneInternal(vtkMRMLScene * newScene)
{
  vtkNew<vtkIntArray> events;
  events->InsertNextValue(vtkMRMLScene::NodeAddedEvent);
  events->InsertNextValue(vtkMRMLScene::NodeRemovedEvent);
  events->InsertNextValue(vtkMRMLScene::EndImportEvent);
  events->InsertNextValue(vtkMRMLScene::EndBatchProcessEvent);
  events->InsertNextValue(vtkMRMLScene::StartCloseEvent);
  this->SetAndObserveMRMLSceneEventsInternal(newScene, events.GetPointer());
}
