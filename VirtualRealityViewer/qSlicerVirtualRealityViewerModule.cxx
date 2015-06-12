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
#include <QtPlugin>

// ExtensionTemplate Logic includes
#include <vtkSlicerVirtualRealityViewerLogic.h>

// ExtensionTemplate includes
#include "qSlicerVirtualRealityViewerModule.h"
#include "qSlicerVirtualRealityViewerModuleWidget.h"

//-----------------------------------------------------------------------------
Q_EXPORT_PLUGIN2(qSlicerVirtualRealityViewerModule, qSlicerVirtualRealityViewerModule);

//-----------------------------------------------------------------------------
/// \ingroup Slicer_QtModules_ExtensionTemplate
class qSlicerVirtualRealityViewerModulePrivate
{
public:
  qSlicerVirtualRealityViewerModulePrivate();
};

//-----------------------------------------------------------------------------
// qSlicerVirtualRealityViewerModulePrivate methods

//-----------------------------------------------------------------------------
qSlicerVirtualRealityViewerModulePrivate::qSlicerVirtualRealityViewerModulePrivate()
{
}

//-----------------------------------------------------------------------------
// qSlicerVirtualRealityViewerModule methods

//-----------------------------------------------------------------------------
qSlicerVirtualRealityViewerModule::qSlicerVirtualRealityViewerModule(QObject* _parent)
  : Superclass(_parent)
  , d_ptr(new qSlicerVirtualRealityViewerModulePrivate)
{
}

//-----------------------------------------------------------------------------
qSlicerVirtualRealityViewerModule::~qSlicerVirtualRealityViewerModule()
{
}

//-----------------------------------------------------------------------------
QString qSlicerVirtualRealityViewerModule::helpText()const
{
  return "This module is still under construction";
}

//-----------------------------------------------------------------------------
QString qSlicerVirtualRealityViewerModule::acknowledgementText()const
{
  return "";
}

//-----------------------------------------------------------------------------
QStringList qSlicerVirtualRealityViewerModule::contributors()const
{
  QStringList moduleContributors;
  moduleContributors << QString( "Franklin King (Queen's University), " );
  return moduleContributors;
}

//-----------------------------------------------------------------------------
QIcon qSlicerVirtualRealityViewerModule::icon()const
{
  return QIcon(":/Icons/VirtualRealityViewer.png");
}

//-----------------------------------------------------------------------------
QStringList qSlicerVirtualRealityViewerModule::categories() const
{
  return QStringList() << "VirtualRealityViewer";
}

//-----------------------------------------------------------------------------
QStringList qSlicerVirtualRealityViewerModule::dependencies() const
{
  return QStringList();
}

//-----------------------------------------------------------------------------
void qSlicerVirtualRealityViewerModule::setup()
{
  this->Superclass::setup();
}

//-----------------------------------------------------------------------------
qSlicerAbstractModuleRepresentation * qSlicerVirtualRealityViewerModule::createWidgetRepresentation()
{
  return new qSlicerVirtualRealityViewerModuleWidget;
}

//-----------------------------------------------------------------------------
vtkMRMLAbstractLogic* qSlicerVirtualRealityViewerModule::createLogic()
{
  return vtkSlicerVirtualRealityViewerLogic::New();
}
