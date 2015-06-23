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
#include <vtkRendererCollection.h>
#include <vtkJPEGWriter.h>
#include <vtkUnsignedCharArray.h>
#include <vtkArrayData.h>
#include <vtkImageCast.h>
#include <vtkWindowToImageFilter.h>

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
void vtkSlicerVirtualRealityViewerLogic::CreateImage(vtkRenderWindow* renderWindow, vtkMRMLMarkupsFiducialNode* fiducialNode, bool equirectangular)
{
  int *size = renderWindow->GetSize();

  double xyzf[3];
  fiducialNode->GetNthFiducialPosition(0, xyzf);

  this->GenerateMap(renderWindow, xyzf[0], xyzf[1], xyzf[2], equirectangular);

  //FILE *binFile = fopen("C:/Work/CubeMap.bmp","wb");
  //fwrite(image, sizeof(unsigned char), size[0]*size[1]*3*12, binFile);
  //fclose(binFile);
}

//----------------------------------------------------------------------------
void vtkSlicerVirtualRealityViewerLogic::CreateImage(vtkRenderWindow* renderWindow, vtkMRMLCameraNode* cameraNode, bool equirectangular)
{
  int *size = renderWindow->GetSize();

  double xyzf[3];
  cameraNode->GetCamera()->GetPosition(xyzf);

  this->GenerateMap(renderWindow, xyzf[0], xyzf[1], xyzf[2], equirectangular);

  //FILE *binFile = fopen("C:/Work/CubeMap.bmp","wb");
  //fwrite(image, sizeof(unsigned char), size[0]*size[1]*3*12, binFile);
  //fclose(binFile);
}

//----------------------------------------------------------------------------
void vtkSlicerVirtualRealityViewerLogic::GenerateMap(vtkRenderWindow* renderWindow, double x, double y, double z, bool equirectangular)
{
  vtkCamera* camera = renderWindow->GetRenderers()->GetFirstRenderer()->GetActiveCamera();
  //vtkCamera* camera = renderer->GetActiveCamera();

  camera->SetPosition(x, y, z);
  camera->SetFocalPoint(x, y + 0.01, z);
  camera->SetViewUp(0, 0, 1);
  camera->UseHorizontalViewAngleOn();
  camera->SetViewAngle(90);
  camera->SetClippingRange(0.3, 500);

	//42 4D 4C 00 00 00 00 00 00 00 1A 00 00 00 0C 00 00 00 C0 03 C0 03 01 00 18 00
  renderWindow->StereoRenderOn();
  int *size = renderWindow->GetSize();

  // Front
  this->WriteImage("C:/Work/s/VRWeb/textures/cube/skybox/lpx.jpg", "C:/Work/s/VRWeb/textures/cube/skybox/rpx.jpg", renderWindow);

  // Left
  camera->Yaw(90);
  this->WriteImage("C:/Work/s/VRWeb/textures/cube/skybox/lpz.jpg", "C:/Work/s/VRWeb/textures/cube/skybox/rpz.jpg", renderWindow);

  // Back
  camera->Yaw(90);
  this->WriteImage("C:/Work/s/VRWeb/textures/cube/skybox/lnx.jpg", "C:/Work/s/VRWeb/textures/cube/skybox/rnx.jpg", renderWindow);

  // Right
  camera->Yaw(90);
  this->WriteImage("C:/Work/s/VRWeb/textures/cube/skybox/lnz.jpg", "C:/Work/s/VRWeb/textures/cube/skybox/rnz.jpg", renderWindow);

  // Top
  camera->Yaw(90);
  camera->SetViewUp(1,0,0);
  camera->Yaw(90);
  this->WriteImage("C:/Work/s/VRWeb/textures/cube/skybox/lpy.jpg", "C:/Work/s/VRWeb/textures/cube/skybox/rpy.jpg", renderWindow);

  // Bottom
  camera->Yaw(180);
  camera->Roll(180);
  this->WriteImage("C:/Work/s/VRWeb/textures/cube/skybox/lny.jpg", "C:/Work/s/VRWeb/textures/cube/skybox/rny.jpg", renderWindow);
//*/
  return;

  //42 4D 4C 00 00 00 00 00 00 00 1A 00 00 00 0C 00 00 00 00 0F 80 07 01 00 18 00
  //42 4D 4C 00 00 00 00 00 00 00 1A 00 00 00 0C 00 00 00 C0 03 C0 03 01 00 18 00
  //42 4D 4C 00 00 00 00 00 00 00 1A 00 00 00 0C 00 00 00 40 0B 80 07 01 00 18 00
  //42 4D 4C 00 00 00 00 00 00 00 1A 00 00 00 0C 00 00 00 80 16 80 07 01 00 18 00
}

void vtkSlicerVirtualRealityViewerLogic::WriteImage(const char* fileName1, const char* fileName2, vtkRenderWindow* renderWindow){
  vtkSmartPointer<vtkWindowToImageFilter> windowToImage = vtkSmartPointer<vtkWindowToImageFilter>::New();
  windowToImage->SetMagnification(1);
  windowToImage->SetInput(renderWindow);
  windowToImage->SetInputBufferTypeToRGB();

  vtkSmartPointer<vtkJPEGWriter> jpegWriter = vtkSmartPointer<vtkJPEGWriter>::New();
  jpegWriter->SetQuality(100);
  jpegWriter->Update();

  renderWindow->SetStereoTypeToLeft();
  renderWindow->Render();
  windowToImage->SetInput(renderWindow);
  windowToImage->Update();
  jpegWriter->SetFileName(fileName1);
  jpegWriter->SetInputData(windowToImage->GetOutput());
  jpegWriter->Update();
  jpegWriter->Write();

  renderWindow->SetStereoTypeToRight();
  renderWindow->Render();
  windowToImage->SetInput(renderWindow);
  windowToImage->Update();
  jpegWriter->SetFileName(fileName2);
  jpegWriter->SetInputData(windowToImage->GetOutput());
  jpegWriter->Update();
  jpegWriter->Write();
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
