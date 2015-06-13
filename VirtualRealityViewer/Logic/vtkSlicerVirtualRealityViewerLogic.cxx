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

// MRML includes
#include "vtkMRMLModelDisplayNode.h"
#include "vtkMRMLModelNode.h"
#include <qMRMLThreeDView.h>
#include <qMRMLThreeDWidget.h>
#include "vtkMRMLCameraNode.h"
#include <vtkMRMLMarkupsFiducialNode.h>

#include <qSlicerApplication.h>
#include "qSlicerLayoutManager.h"

// VTK includes
#include <vtkNew.h>
#include <vtkSmartPointer.h>
#include <vtkCommand.h>
#include <vtkMRMLScene.h>
#include <vtkObjectFactory.h>
#include <vtkCamera.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>

// CubeToCyl includes
#include <Cube2Cyl.hpp>

// STD includes
#include <cassert>

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
void vtkSlicerVirtualRealityViewerLogic::CreateImage(vtkRenderer* renderer, vtkRenderWindow* renderWindow, vtkMRMLMarkupsFiducialNode* fiducialNode)
{
  vtkCamera* camera = renderer->GetActiveCamera();
  
  //fiducialNode->GetNumberOfFiducials();
  double xyzf[3];
  fiducialNode->GetNthFiducialPosition(0, xyzf);

  camera->SetPosition(xyzf[0], xyzf[1], xyzf[2]);
  camera->SetFocalPoint(xyzf[0], xyzf[1] + 0.1, xyzf[2]);
  camera->SetViewUp(0, 0, 1);
  camera->UseHorizontalViewAngleOn();
  camera->SetViewAngle(90);
  camera->SetClippingRange(0.3, 500);

	//42 4D 4C 00 00 00 00 00 00 00 1A 00 00 00 0C 00 00 00 C0 03 C0 03 01 00 18 00
  renderWindow->StereoRenderOn();
  int *size = renderWindow->GetSize();

  // Front
  renderWindow->SetStereoTypeToLeft();
  renderWindow->Render();
  unsigned char* imageA_L = new unsigned char [3*size[0]*size[1]];
  imageA_L = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  renderWindow->SetStereoTypeToRight();
  renderWindow->Render();
  unsigned char* imageA_R = new unsigned char [3*size[0]*size[1]];
  imageA_R = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  // Left
  camera->Yaw(90);
  renderWindow->SetStereoTypeToLeft();
  renderWindow->Render();
  unsigned char* imageL_L = new unsigned char [3*size[0]*size[1]];
  imageL_L = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  renderWindow->SetStereoTypeToRight();
  renderWindow->Render();
  unsigned char* imageL_R = new unsigned char [3*size[0]*size[1]];
  imageL_R = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  // Back
  camera->Yaw(90);
  renderWindow->SetStereoTypeToLeft();
  renderWindow->Render();
  unsigned char* imageP_L = new unsigned char [3*size[0]*size[1]];
  imageP_L = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  renderWindow->SetStereoTypeToRight();
  renderWindow->Render();
  unsigned char* imageP_R = new unsigned char [3*size[0]*size[1]];
  imageP_R = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  // Right
  camera->Yaw(90);
  renderWindow->SetStereoTypeToLeft();
  renderWindow->Render();
  unsigned char* imageR_L = new unsigned char [3*size[0]*size[1]];
  imageR_L = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  renderWindow->SetStereoTypeToRight();
  renderWindow->Render();
  unsigned char* imageR_R = new unsigned char [3*size[0]*size[1]];
  imageR_R = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  // Top
  camera->Yaw(90);
  camera->SetViewUp(1,0,0);
  camera->Yaw(90);
  renderWindow->SetStereoTypeToLeft();
  renderWindow->Render();
  unsigned char* imageS_L = new unsigned char [3*size[0]*size[1]];
  imageS_L = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  renderWindow->SetStereoTypeToRight();
  renderWindow->Render();
  unsigned char* imageS_R = new unsigned char [3*size[0]*size[1]];
  imageS_R = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  // Bottom
  camera->Yaw(180);
  renderWindow->SetStereoTypeToLeft();
  renderWindow->Render();
  unsigned char* imageI_L = new unsigned char [3*size[0]*size[1]];
  imageI_L = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  renderWindow->SetStereoTypeToRight();
  renderWindow->Render();
  unsigned char* imageI_R = new unsigned char [3*size[0]*size[1]];
  imageI_R = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);
  

  unsigned char* cube_L[6];
  cube_L[0] = imageS_L; cube_L[1] = imageL_L; cube_L[2] = imageA_L; cube_L[3] = imageR_L; cube_L[4] = imageP_L; cube_L[5] = imageI_L;

  unsigned char* stitchedImage_L = this->CubemapToEquirectangular(cube_L);
  FILE *binFile = fopen("LeftPanorama","wb");
  fwrite(stitchedImage_L, sizeof(unsigned char), 3*3840*1920, binFile);
  fclose(binFile);


  unsigned char* cube_R[6];
  cube_R[0] = imageS_R; cube_R[1] = imageL_R; cube_R[2] = imageA_R; cube_R[3] = imageR_R; cube_R[4] = imageP_R; cube_R[5] = imageI_R;

  unsigned char* stitchedImage_R = this->CubemapToEquirectangular(cube_R);
  binFile = fopen("RightPanorama","wb");
  fwrite(stitchedImage_R, sizeof(unsigned char), 3*3840*1920, binFile);
  fclose(binFile);
}

//----------------------------------------------------------------------------
unsigned char* vtkSlicerVirtualRealityViewerLogic::CubemapToEquirectangular(unsigned char* cube[])
{
  //42 4D 4C 00 00 00 00 00 00 00 1A 00 00 00 0C 00 00 00 00 0F 80 07 01 00 18 00

  //960x960
  int i = 0;
  int j = 0;

  Cube2Cyl cube2cylAlgo;
  cube2cylAlgo.init(960, M_PI, 2.0 * M_PI);
  cube2cylAlgo.genMap();
  
  unsigned int panoramaWidth  = cube2cylAlgo.pxPanoSizeH;
  unsigned int panoramaHeight = cube2cylAlgo.pxPanoSizeV;

  unsigned char* imagePanorama = new unsigned char [3*panoramaWidth*panoramaHeight];

  const CUBE_COORD* coord = NULL;
  
  // Map the pixels from the panorama back to the source image
  for (i = 0; i < panoramaWidth; ++i) {
      for (j = 0; j < panoramaHeight; ++j) {
          // Get the corresponding position of (i, j)
          coord = cube2cylAlgo.getCoord(i, j);

          // Access the pixel
          unsigned char r = cube[coord->face][ ((unsigned long)coord->y * 960 * 3) + ((unsigned long)coord->x * 3)     ];
          unsigned char g = cube[coord->face][ ((unsigned long)coord->y * 960 * 3) + ((unsigned long)coord->x * 3) + 1 ];
          unsigned char b = cube[coord->face][ ((unsigned long)coord->y * 960 * 3) + ((unsigned long)coord->x * 3) + 2 ];

          // Write the pixel to the panorama
          imagePanorama[ (j * panoramaWidth * 3) + (i * 3)     ] = r;
          imagePanorama[ (j * panoramaWidth * 3) + (i * 3) + 1 ] = g;
          imagePanorama[ (j * panoramaWidth * 3) + (i * 3) + 2 ] = b;
      }
  }

  return imagePanorama;
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
