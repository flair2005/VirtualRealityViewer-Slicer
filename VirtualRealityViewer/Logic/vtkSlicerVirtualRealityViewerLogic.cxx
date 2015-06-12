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
void vtkSlicerVirtualRealityViewerLogic::CreateImage(vtkRenderer* renderer, vtkRenderWindow* renderWindow)
{
  vtkCamera* camera = renderer->GetActiveCamera();

  camera->SetPosition(0, 0, 0);
  camera->SetFocalPoint(0, 0.1, 0);
  camera->SetViewUp(0, 0, 1);
  camera->UseHorizontalViewAngleOn();
  camera->SetViewAngle(90);
  camera->SetClippingRange(0.3, 500);

	//42 4D 4C 00 00 00 00 00 00 00 1A 00 00 00 0C 00 00 00 C0 03 C0 03 01 00 18 00

  int *size = renderWindow->GetSize();
  cout << size[0] << " " << size[1] << endl;

  /*
  FILE *binFile = fopen("C:/W/Image1","wb");
  fwrite(image, sizeof(unsigned char), 3*size[0]*size[1], binFile);
  fclose(binFile);
  */

  // Front
  renderWindow->Render();
  unsigned char* imageA = new unsigned char [3*size[0]*size[1]];
  imageA = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  // Left
  camera->Yaw(90);
  renderWindow->Render();
  unsigned char* imageL = new unsigned char [3*size[0]*size[1]];
  imageL = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  // Back
  camera->Yaw(90);
  renderWindow->Render();
  unsigned char* imageP = new unsigned char [3*size[0]*size[1]];
  imageP = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  // Right
  camera->Yaw(90);
  renderWindow->Render();
  unsigned char* imageR = new unsigned char [3*size[0]*size[1]];
  imageR = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  // Top
  camera->Yaw(90);
  camera->SetViewUp(1,0,0);
  camera->Yaw(90);
  renderWindow->Render();
  unsigned char* imageS = new unsigned char [3*size[0]*size[1]];
  imageS = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  // Bottom
  camera->Yaw(180);
  renderWindow->Render();
  unsigned char* imageI = new unsigned char [3*size[0]*size[1]];
  imageI = renderWindow->GetPixelData(0, 0, size[0]-1, size[1]-1, 1);

  unsigned char* cube[6];
  cube[0] = imageS; cube[1] = imageL; cube[2] = imageA; cube[3] = imageR; cube[4] = imageP; cube[5] = imageI;

  unsigned char* stitchedImage = this->CubemapToEquirectangular(cube);


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

  cout<<panoramaHeight<<endl;
  cout<<panoramaWidth<<endl;

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

          //cout << (unsigned long)coord->x << endl;

          // Write the pixel to the panorama
          imagePanorama[ (j * panoramaWidth * 3) + (i * 3)     ] = r;
          imagePanorama[ (j * panoramaWidth * 3) + (i * 3) + 1 ] = g;
          imagePanorama[ (j * panoramaWidth * 3) + (i * 3) + 2 ] = b;
      }
  }
  
  FILE *binFile = fopen("C:/W/Panorama","wb");
  fwrite(imagePanorama, sizeof(unsigned char), 3*panoramaWidth*panoramaHeight, binFile);
  fclose(binFile);

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
