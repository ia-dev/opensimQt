/*
 * Copyright 2007 Sandia Corporation.
 * Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
 * license for use of this work by or on behalf of the
 * U.S. Government. Redistribution and use in source and binary forms, with
 * or without modification, are permitted provided that this Notice and any
 * statement of authorship are reproduced on all copies.
 */


//QT
#include "QtGui/qimagereader.h"
#include "qmessagebox.h"
#include <QFileDialog>
#include "ui_opensimQt.h"
#include "opensimQt.h"

#include <vtkDataObjectToTable.h>
#include <vtkElevationFilter.h>
#include "vtkGenericOpenGLRenderWindow.h"
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkQtTableView.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include "vtkSmartPointer.h"
#include <vtkVectorText.h>
#include <vtkSTLReader.h>


// Constructor
opensimQt::opensimQt()
{
  this->ui = new Ui_opensimQt;
  this->ui->setupUi(this);

  // Qt Table View
  this->TableView = vtkSmartPointer<vtkQtTableView>::New();

  // Place the table view in the designer form
  this->ui->tableFrame->layout()->addWidget(this->TableView->GetWidget());
  // Set up action signals and slots
  connect(this->ui->actionOpenFile, SIGNAL(triggered()), this, SLOT(slotOpenFile()));
  connect(this->ui->actionExit, SIGNAL(triggered()), this, SLOT(slotExit()));
};

opensimQt::~opensimQt(){
  // The smart pointers should clean up for up
}

// Action to be taken upon file open
void opensimQt::slotOpenFile(){
      QString s = QFileDialog::getOpenFileName(this,tr("select stl file"), "../data", tr("Image Files (*.stl *.obj)"));
      std::string fileName = s.toStdString().c_str(); //.toUtf8().constData();
      //std::cout<<fileName<<std::endl;
	  vtkNew<vtkSTLReader> stlreader;
	  stlreader->SetFileName( fileName.c_str() );
	  //stlreader->SetFileName("../data/C7.stl");
	  stlreader->Update();
	  vtkNew<vtkPolyDataMapper> stlMapper;
	  stlMapper->SetInputConnection(stlreader->GetOutputPort());
	  vtkNew<vtkActor> stlActor;
	  stlActor->SetMapper(stlMapper.Get());

	  // VTK Renderer
	  vtkNew<vtkRenderer> ren;

	  // Add Actor to renderer
	  ren->AddActor(stlActor);

	  // VTK/Qt wedded
	  vtkNew<vtkGenericOpenGLRenderWindow> renderWindow;
	  this->ui->qvtkWidget->SetRenderWindow(renderWindow);
	  this->ui->qvtkWidget->GetRenderWindow()->AddRenderer(ren);

	  // Just a bit of Qt interest: Culling off the
	  // point data and handing it to a vtkQtTableView
	  vtkNew<vtkDataObjectToTable> toTable;
	  toTable->SetInputConnection(stlreader->GetOutputPort());
	  toTable->SetFieldType(vtkDataObjectToTable::POINT_DATA);

	  // Here we take the end of the VTK pipeline and give it to a Qt View
	  this->TableView->SetRepresentationFromInputConnection(toTable->GetOutputPort());
}

void opensimQt::slotExit() {
  qApp->exit();
}
