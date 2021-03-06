/*
 * Copyright (C) 2005-2019 Centre National d'Etudes Spatiales (CNES)
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


/* Example usage:
./FrostImageFilter Input/GomaSmall.png Output/GomaSmallFrostFiltered.png 5 0.1
*/


// This example illustrates the use of the \doxygen{otb}{FrostImageFilter}.
// This filter belongs to the family of the edge-preserving smoothing
// filters which are usually used for speckle reduction in radar
// images.
//
// This filter uses a negative exponential convolution kernel.
// The output of the filter for pixel p is:
//      $ \hat I_{s}=\sum_{p\in\eta_{p}} m_{p}I_{p} $
//
// where :   $ m_{p}=\frac{KC_{s}^{2}\exp(-KC_{s}^{2}d_{s, p})}{\sum_{p\in\eta_{p}} KC_{s}^{2}\exp(-KC_{s}^{2}d_{s, p})} $
//    and  $ d_{s, p}=\sqrt{(i-i_{p})^2+(j-j_{p})^2} $
//
// \begin{itemize}
// \item $ K $     : the decrease coefficient
// \item $ (i, j)$ : the coordinates of the pixel inside the region
// defined by $ \eta_{s} $
// \item $ (i_{p}, j_{p})$ : the coordinates of the pixels belonging to $ \eta_{p} \subset \eta_{s} $
// \item $ C_{s}$ : the variation coefficient computed over $ \eta_{p}$
// \end{itemize}
//
//
//
// Most of this example is similar to the previous one and only the differences
// will be highlighted.
//
// First, we need to include the header:

#include "otbFrostImageFilter.h"

#include "otbImage.h"
#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"

int main(int argc, char* argv[])
{

  if (argc != 5)
  {
    std::cerr << "Usage: " << argv[0] << " inputImageFile ";
    std::cerr << " outputImageFile radius deramp" << std::endl;
    return EXIT_FAILURE;
  }

  typedef unsigned char PixelType;

  typedef otb::Image<PixelType, 2> InputImageType;
  typedef otb::Image<PixelType, 2> OutputImageType;

  //  The filter can be instantiated using the image types defined previously.

  typedef otb::FrostImageFilter<InputImageType, OutputImageType> FilterType;

  typedef otb::ImageFileReader<InputImageType> ReaderType;

  typedef otb::ImageFileWriter<OutputImageType> WriterType;

  ReaderType::Pointer reader = ReaderType::New();
  FilterType::Pointer filter = FilterType::New();

  WriterType::Pointer writer = WriterType::New();
  writer->SetInput(filter->GetOutput());
  reader->SetFileName(argv[1]);

  //  The image obtained with the reader is passed as input to the
  //  \doxygen{otb}{FrostImageFilter}.
  //
  //  \index{otb::FrostImageFilter!SetInput()}
  //  \index{otb::FileImageReader!GetOutput()}

  filter->SetInput(reader->GetOutput());

  //  The method \code{SetRadius()} defines the size of the window to
  //  be used for the computation of the local statistics. The method
  //  \code{SetDeramp()} sets the $K$ coefficient.
  //
  //  \index{otb::FrostImageFilter!SetRadius()}
  //  \index{otb::FrostImageFilter!SetDeramp()}
  //  \index{SetDeramp()!otb::FrostImageFilter}

  FilterType::SizeType Radius;
  Radius[0] = atoi(argv[3]);
  Radius[1] = atoi(argv[3]);

  filter->SetRadius(Radius);
  filter->SetDeramp(atof(argv[4]));

  writer->SetFileName(argv[2]);
  writer->Update();

  // Figure~\ref{fig:FROST_FILTER} shows the result of applying the Frost
  // filter to a SAR image.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.44\textwidth]{GomaSmall.eps}
  // \includegraphics[width=0.44\textwidth]{GomaSmallFrostFiltered.eps}
  // \itkcaption[Frost Filter Application]{Result of applying the
  // \doxygen{otb}{FrostImageFilter} to a SAR image.}
  // \label{fig:FROST_FILTER}
  // \end{figure}
  //
  //  \relatedClasses
  //  \begin{itemize}
  //  \item \doxygen{otb}{LeeImageFilter}
  //  \end{itemize}

  return EXIT_SUCCESS;
}
