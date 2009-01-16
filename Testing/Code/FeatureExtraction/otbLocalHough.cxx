/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkExceptionObject.h"


#include <iostream>

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbLineSpatialObjectList.h"

#include "otbLocalHoughFilter.h"


int otbLocalHough( int argc, char* argv[] )
{
  const char * inputFilename  = argv[1];
  unsigned int  RadiusX((unsigned int)::atoi(argv[2]));
  unsigned int  RadiusY((unsigned int)::atoi(argv[3]));
  unsigned int  NumberOfLines((unsigned int)::atoi(argv[4]));

  typedef unsigned char                                  InputPixelType;
  const   unsigned int                                  Dimension = 2;

  typedef otb::Image< InputPixelType,  Dimension >        InputImageType;

  typedef otb::LocalHoughFilter< InputImageType >   FilterType;

  FilterType::Pointer filter = FilterType::New();

  typedef otb::ImageFileReader< InputImageType  >         ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName( inputFilename  );
  reader->Update();

  FilterType::SizeType Radius;
  Radius[0] = RadiusX;
  Radius[1] = RadiusY;

  typedef otb::LineSpatialObjectList      LinesListType;
  LinesListType::Pointer list = LinesListType::New();

  filter->SetRadius( Radius );
  filter->SetNumberOfLines( NumberOfLines );

  filter->SetInput( reader->GetOutput() );
  filter->Update();

  list = filter->GetOutput();

  std::cout<<list->size()<<std::endl;


  return EXIT_SUCCESS;
}


