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
./RatioChDet Input/GomaAvant.png Input/GomaApres.png Output/RatioChDet.tif 3
*/


// This example illustrates the class
// \doxygen{otb}{MeanRatioImageFilter} for detecting changes
// between pairs of images. This filter computes the mean intensity in
// the neighborhood of each pixel of the pair of images to be compared
// and uses the ratio of means as a change indicator. This change
// indicator is then normalized between 0 and 1 by using the classical
// \begin{equation}
// r = 1 - min\{\frac{\mu_A}{\mu_B},\frac{\mu_B}{\mu_A} \},
// \end{equation}
// where $\mu_A$ and $\mu_B$ are the local means. This
// example will use the images shown in
// figure ~\ref{fig:RATCHDETINIM}. These correspond to 2 Radarsat fine
// mode acquisitions before and after a lava flow resulting from a
// volcanic eruption.
// \begin{figure}
// \center
// \includegraphics[width=0.35\textwidth]{GomaAvant.eps}
// \includegraphics[width=0.35\textwidth]{GomaApres.eps}
// \itkcaption[Radarsat Images for Change Detection]{Images used for the
// change detection. Left: Before the eruption. Right: after the eruption.}
// \label{fig:RATCHDETINIM}
// \end{figure}
//
// We start by including the corresponding header file.

#include "otbMeanRatioImageFilter.h"

#include "otbImageFileReader.h"
#include "otbImageFileWriter.h"
#include "otbImage.h"
#include "itkShiftScaleImageFilter.h"
#include "otbCommandProgressUpdate.h"

int main(int argc, char* argv[])
{

  if (argc < 5)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile1 inputImageFile2  outputImageFile radius" << std::endl;
    return -1;
  }

  // Define the dimension of the images
  const unsigned int Dimension = 2;

  // We start by declaring the types for the two input images, the
  // change image and the image to be stored in a file for visualization.

  typedef float                                    InternalPixelType;
  typedef unsigned char                            OutputPixelType;
  typedef otb::Image<InternalPixelType, Dimension> InputImageType1;
  typedef otb::Image<InternalPixelType, Dimension> InputImageType2;
  typedef otb::Image<InternalPixelType, Dimension> ChangeImageType;
  typedef otb::Image<OutputPixelType, Dimension>   OutputImageType;

  //  We can now declare the types for the readers. Since the images
  //  can be vey large, we will force the pipeline to use
  //  streaming. For this purpose, the file writer will be
  //  streamed. This is achieved by using the
  //  \doxygen{otb}{ImageFileWriter} class.

  typedef otb::ImageFileReader<InputImageType1> ReaderType1;
  typedef otb::ImageFileReader<InputImageType2> ReaderType2;
  typedef otb::ImageFileWriter<OutputImageType> WriterType;


  //  The change detector will give a normalized result between 0 and
  //  1. In order to store the result in PNG format we will
  //  rescale the results of the change detection in order to use all
  //  the output pixel type range of values.

  typedef itk::ShiftScaleImageFilter<ChangeImageType, OutputImageType> RescalerType;


  //  The \doxygen{otb}{MeanRatioImageFilter} is templated over
  //  the types of the two input images and the type of the generated change
  //  image.

  typedef otb::MeanRatioImageFilter<InputImageType1, InputImageType2, ChangeImageType> FilterType;


  //  The different elements of the pipeline can now be instantiated.

  ReaderType1::Pointer  reader1  = ReaderType1::New();
  ReaderType2::Pointer  reader2  = ReaderType2::New();
  WriterType::Pointer   writer   = WriterType::New();
  FilterType::Pointer   filter   = FilterType::New();
  RescalerType::Pointer rescaler = RescalerType::New();

  const char* inputFilename1 = argv[1];
  const char* inputFilename2 = argv[2];
  const char* outputFilename = argv[3];
  //  We set the parameters of the different elements of the pipeline.

  reader1->SetFileName(inputFilename1);
  reader2->SetFileName(inputFilename2);
  writer->SetFileName(outputFilename);
  float scale = itk::NumericTraits<OutputPixelType>::max();
  rescaler->SetScale(scale);

  //  The only parameter for this change detector is the radius of
  //  the window used for computing the mean of the intensities.

  filter->SetRadius(atoi(argv[4]));

  //  We build the pipeline by plugging all the elements together.

  filter->SetInput1(reader1->GetOutput());
  filter->SetInput2(reader2->GetOutput());

  rescaler->SetInput(filter->GetOutput());
  writer->SetInput(rescaler->GetOutput());

  typedef otb::CommandProgressUpdate<FilterType> CommandType;

  CommandType::Pointer observer = CommandType::New();
  filter->AddObserver(itk::ProgressEvent(), observer);

  try
  {
    writer->Update();
  }
  catch (itk::ExceptionObject& err)
  {
    std::cout << "ExceptionObject caught !" << std::endl;
    std::cout << err << std::endl;
    return -1;
  }

  // Figure \ref{fig:RESRATCHDET} shows the result of the change
  // detection by ratio of local means.
  // \begin{figure}
  // \center
  // \includegraphics[width=0.35\textwidth]{RatioChDet.eps}
  // \itkcaption[Ratio Change Detection Results]{Result of the
  // ratio of means change detector}
  // \label{fig:RESRATCHDET}
  // \end{figure}

  return EXIT_SUCCESS;
}
