/*=========================================================================

  Program:   Visualization Toolkit
  Module:    Cone6.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//
// This example introduces 3D widgets. 3D widgets take advantage of the
// event/observer design pattern introduced previously. They typically
// have a particular representation in the scene which can be interactively
// selected and manipulated using the mouse and keyboard. As the widgets
// are manipulated, they in turn invoke events such as StartInteractionEvent,
// InteractionEvent, and EndInteractionEvent which can be used to manipulate
// the scene that the widget is embedded in. 3D widgets work in the context
// of the event loop which was set up in the previous example.
//
// Note: there are more 3D widget examples in VTK/Examples/GUI/.
//

// First include the required header files for the VTK classes we are using.
#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkCommand.h"
#include "vtkBoxWidget.h"
#include "vtkTransform.h"
#include "vtkInteractorStyleTrackballCamera.h"
#include "vtkWindowToImageFilter.h"
#include "vtkImageShiftScale.h"
#include "vtkBMPWriter.h"
#include "vtkMath.h"
#include "ChamferTransform.h"
// ---------------------------------------
#include "vtkOBJReader.h"
#include "vtkSmartPointer.h"
#include <string>
// ---------------------------------------

// 
// Define window size
//
#define N 300

//
// Similar to Cone2.cxx, we define a callback for interaction.
//
class vtkMyCallback : public vtkCommand
{
public:
  static vtkMyCallback *New()
    { return new vtkMyCallback; }
  virtual void Execute(vtkObject *caller, unsigned long, void*)
    {
      vtkTransform *t = vtkTransform::New();
      vtkBoxWidget *widget = reinterpret_cast<vtkBoxWidget*>(caller);
      widget->GetTransform(t);
      widget->GetProp3D()->SetUserTransform(t);
      t->Delete();
    }
};

int main()
{
	// ------------- 
	// DATA
	// -------------

	// Objects of interest
	std::string filename = "C:\\VTK\\Data\\OBJ\\Vanquish - Augmented Reaction Suit\\vanquish.obj";
	//vtkSmartPointer<vtkOBJReader> reader = vtkSmartPointer<vtkOBJReader>::New();
	vtkOBJReader *reader = vtkOBJReader::New();
	reader->SetFileName(filename.c_str());
	reader->Update();

	// Secondary objects
	vtkConeSource *cone = vtkConeSource::New();
	cone->SetHeight( 3.0 );
	cone->SetRadius( 1.0 );
	cone->SetResolution( 10 );

	// ------------- 
	// MAPPERS
	// -------------

	// Objects of interest
	vtkPolyDataMapper *readerMapper = vtkPolyDataMapper::New();
	readerMapper->SetInputConnection( reader->GetOutputPort() );
  
	// Secondary objects
	vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
	coneMapper->SetInputConnection( cone->GetOutputPort() );

	// ------------- 
	// ACTORS
	// -------------

	// Objects of interest
	vtkActor *readerActor = vtkActor::New();
	readerActor->SetMapper(readerMapper);
  
	// Secondary objects
	vtkActor *coneActor = vtkActor::New();
	coneActor->SetMapper( coneMapper );

	// ------------- 
	// RENDERER
	// -------------

	vtkRenderer *ren1= vtkRenderer::New();
  
	// ------------- 
	// WINDOW
	// -------------

	vtkRenderWindow *renWin = vtkRenderWindow::New();
	renWin->SetSize( N, N );

	// ------------- 
	// INTERACTOR
	// -------------

	vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
	iren->SetRenderWindow(renWin);

	// ------------- 
	// FIRST RENDER PASS: RENDER OBJECTS OF INTEREST
	// -------------

	// Add and render objects
	ren1->AddActor(readerActor);
	renWin->AddRenderer(ren1);
	renWin->Render();

	// Create Depth Map
	vtkWindowToImageFilter *filter = vtkWindowToImageFilter::New();
	filter->SetInput(renWin);
	filter->SetMagnification(1);
	filter->SetInputBufferTypeToZBuffer();        
	vtkImageShiftScale *scale = vtkImageShiftScale::New();
	scale->SetOutputScalarTypeToUnsignedChar();
	scale->SetInputConnection(filter->GetOutputPort());
	scale->SetShift(0);
	scale->SetScale(-255);
	vtkBMPWriter *imageWriter = vtkBMPWriter::New();
	imageWriter->SetFileName("out2.bmp");
	imageWriter->SetInputConnection(scale->GetOutputPort());
	imageWriter->Write();

	// ------------- 
	// DISTANCE TRANSFORM
	// -------------
	ChamferTransform c(ren1, 30 * vtkMath::Pi() / 180);

	// Rest of scene
	ren1->AddActor(coneActor);
	ren1->SetBackground(0.1, 0.2, 0.4);


	//
	// By default the vtkRenderWindowInteractor instantiates an instance
	// of vtkInteractorStyle. vtkInteractorStyle translates a set of events
	// it observes into operations on the camera, actors, and/or properties
	// in the vtkRenderWindow associated with the vtkRenderWinodwInteractor.
	// Here we specify a particular interactor style.
	vtkInteractorStyleTrackballCamera *style =
	vtkInteractorStyleTrackballCamera::New();
	iren->SetInteractorStyle(style);

	//
	// Here we use a vtkBoxWidget to transform the underlying coneActor (by
	// manipulating its transformation matrix). Many other types of widgets
	// are available for use, see the documentation for more details.
	//
	// The SetInteractor method is how 3D widgets are associated with the render
	// window interactor. Internally, SetInteractor sets up a bunch of callbacks
	// using the Command/Observer mechanism (AddObserver()). The place factor
	// controls the initial size of the widget with respect to the bounding box
	// of the input to the widget.
	vtkBoxWidget *boxWidget = vtkBoxWidget::New();
	boxWidget->SetInteractor(iren);
	boxWidget->SetPlaceFactor(1.25);

	//
	// Place the interactor initially. The input to a 3D widget is used to
	// initially position and scale the widget. The EndInteractionEvent is
	// observed which invokes the SelectPolygons callback.
	//
	boxWidget->SetProp3D(coneActor);
	boxWidget->PlaceWidget();
	vtkMyCallback *callback = vtkMyCallback::New();
	boxWidget->AddObserver(vtkCommand::InteractionEvent, callback);

	//
	// Normally the user presses the "i" key to bring a 3D widget to life. Here
	// we will manually enable it so it appears with the cone.
	//
	boxWidget->On();

	//
	// Start the event loop.
	//
	iren->Initialize();
	iren->Start();

	//
	// Free up any objects we created. All instances in VTK are deleted by
	// using the Delete() method.
	//
	cone->Delete();
	coneMapper->Delete();
	coneActor->Delete();
	reader->Delete();
	readerMapper->Delete();
	readerActor->Delete();
	callback->Delete();
	boxWidget->Delete();
	ren1->Delete();
	renWin->Delete();
	iren->Delete();
	style->Delete();

	return 0;
}


