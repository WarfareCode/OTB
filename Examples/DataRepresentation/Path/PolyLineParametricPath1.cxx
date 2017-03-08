/*
 * Copyright (C) 1999-2011 Insight Software Consortium
 * Copyright (C) 2005-2017 Centre National d'Etudes Spatiales (CNES)
 *
 * This file is part of Orfeo Toolbox
 *
 *     https://www.orfeo-toolbox.org/
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */



// Software Guide : BeginLatex
//
// This example illustrates how to use the \doxygen{itk}{PolyLineParametricPath}.
// This class will typically be used for representing in a concise way the
// output of an image segmentation algorithm in 2D. See section
// \ref{sec:Alignments} for an example in the context of alignment
// detection.  The
// \code{PolyLineParametricPath} however could also be used for representing
// any open or close curve in N-Dimensions as a linear piece-wise approximation.
//
//
// First, the header file of the \code{PolyLineParametricPath} class must be included.
//
// Software Guide : EndLatex

#include "otbImage.h"
#include "otbImageFileReader.h"

// Software Guide : BeginCodeSnippet
#include "itkPolyLineParametricPath.h"
// Software Guide : EndCodeSnippet

int main(int argc, char * argv[])
{

  if (argc < 2)
    {
    std::cerr << "Missing arguments" << std::endl;
    std::cerr << "Usage: PolyLineParametricPath  inputImageFileName" <<
    std::endl;
    return -1;
    }

  // Software Guide : BeginLatex
  //
  // The path is instantiated over the dimension of the image.
  // Software Guide : EndLatex

  // Software Guide : BeginCodeSnippet
  const unsigned int Dimension = 2;

  typedef otb::Image<unsigned char, Dimension> ImageType;

  typedef itk::PolyLineParametricPath<Dimension> PathType;
  // Software Guide : EndCodeSnippet

  typedef otb::ImageFileReader<ImageType> ReaderType;

  ReaderType::Pointer reader = ReaderType::New();

  reader->SetFileName(argv[1]);

  try
    {
    reader->Update();
    }
  catch (itk::ExceptionObject& excp)
    {
    std::cout << "Problem reading the input image " << std::endl;
    std::cout << excp << std::endl;
    return -1;
    }

  // Software Guide : BeginCodeSnippet
  ImageType::ConstPointer image = reader->GetOutput();

  PathType::Pointer path = PathType::New();

  path->Initialize();

  typedef PathType::ContinuousIndexType ContinuousIndexType;

  ContinuousIndexType cindex;

  typedef ImageType::PointType ImagePointType;

  ImagePointType origin = image->GetOrigin();

  ImageType::SpacingType spacing = image->GetSpacing();
  ImageType::SizeType    size    = image->GetBufferedRegion().GetSize();

  ImagePointType point;

  point[0] = origin[0] + spacing[0] * size[0];
  point[1] = origin[1] + spacing[1] * size[1];

  image->TransformPhysicalPointToContinuousIndex(origin, cindex);

  path->AddVertex(cindex);

  image->TransformPhysicalPointToContinuousIndex(point, cindex);

  path->AddVertex(cindex);
  // Software Guide : EndCodeSnippet

  return EXIT_SUCCESS;
}
