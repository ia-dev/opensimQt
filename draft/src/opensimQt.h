/*=========================================================================

  Program:   Visualization Toolkit
  Module:    opensimQt.h
  Language:  C++

  Copyright 2009 Sandia Corporation.
  Under the terms of Contract DE-AC04-94AL85000, there is a non-exclusive
  license for use of this work by or on behalf of the
  U.S. Government. Redistribution and use in source and binary forms, with
  or without modification, are permitted provided that this Notice and any
  statement of authorship are reproduced on all copies.

=========================================================================*/
#ifndef opensimQt_H
#define opensimQt_H

#include "vtkSmartPointer.h"    // Required for smart pointer internal ivars.
#include <QMainWindow>

// Forward Qt class declarations
class Ui_opensimQt;

// Forward VTK class declarations
class vtkQtTableView;


class opensimQt : public QMainWindow
{
  Q_OBJECT

public:

  // Constructor/Destructor
  opensimQt();
  ~opensimQt();

public slots:

  virtual void slotOpenFile();
  virtual void slotExit();

protected:

protected slots:

private:

  vtkSmartPointer<vtkQtTableView>         TableView;

  // Designer form
  Ui_opensimQt *ui;
};

#endif // opensimQt_H
