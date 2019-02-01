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

#ifndef otbPointSetDensityGaussianFunction_hxx
#define otbPointSetDensityGaussianFunction_hxx

#include "otbPointSetDensityGaussianFunction.h"
#include "otbMath.h"

namespace otb
{
/**
 * Evaluate
 */
template <class TPointSet, class TOutput>
typename PointSetDensityGaussianFunction<TPointSet,   TOutput>
::OutputType
PointSetDensityGaussianFunction<TPointSet,   TOutput>
::Evaluate(const InputType& input) const
{
  double accu = 0;
  double radiussq = m_Radius * m_Radius;

  if (this->GetPointSet()->GetNumberOfPoints() != 0)
    {
    typedef typename TPointSet::PointsContainer::ConstIterator iteratorType;
    iteratorType it = this->GetPointSet()->GetPoints()->Begin();

    while (it != this->GetPointSet()->GetPoints()->End())
      {
      float distX = input[0] - it.Value()[0];
      float distY = input[1] - it.Value()[1];
      float distsq = distX * distX + distY * distY;

      accu += std::exp(-distsq / radiussq / 2);

      ++it;
      }
    accu /= std::sqrt(2 * CONST_PI * radiussq);
    }

  return accu;
}

/**
 * PrintSelf
 */
template <class TPointSet, class TOutput>
void
PointSetDensityGaussianFunction<TPointSet,   TOutput>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);
}

} // end namespace otb

#endif
