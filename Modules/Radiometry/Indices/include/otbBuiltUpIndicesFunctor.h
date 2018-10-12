/*
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

#ifndef otbBuiltUpIndicesFunctor_h
#define otbBuiltUpIndicesFunctor_h

#include "otbVegetationIndicesFunctor.h"
#include <string>

namespace otb
{
namespace Functor
{
/**
   * \class TM4AndTM5IndexBase
   * \brief Base class for TM4 And TM5 channels of Land Sat
   * (equivalent to Red and NIR of SPOT5)
   *
   *  Implement operators for UnaryFunctorImageFilter templated with a
   *  VectorImage and BinaryFunctorImageFilter templated with single
   *  images.
   *  Subclasses should NOT overload operators, they must  re-implement
   *  the Evaluate() method.
   *
   * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template<class TInput1, class TInput2, class TOutput>
class ITK_TEMPLATE_EXPORT TM4AndTM5IndexBase
{
public:
  /// Vector pixel type used to support both vector images and multiple
  /// input images
  typedef itk::VariableLengthVector<TInput1> InputVectorType;

  //operators !=
  bool operator !=(const TM4AndTM5IndexBase&) const
  {
    return true;
  }
  //operator ==
  bool operator ==(const TM4AndTM5IndexBase& other) const
  {
    return !(*this != other);
  }

  // Operator on vector pixel type
  inline TOutput operator ()(const InputVectorType& inputVector) const
  {
    return this->Evaluate(inputVector[m_TM4Index - 1], static_cast<TInput2>(inputVector[m_TM5Index - 1]));
  }

  // Binary operator
  inline TOutput operator ()(const TInput1& tm4, const TInput2& tm5) const
  {
    return this->Evaluate(tm4, tm5);
  }
  /// Constructor
  TM4AndTM5IndexBase() : m_TM4Index(4), m_TM5Index(5) {}
  /// Desctructor
  virtual ~TM4AndTM5IndexBase() {}

  /// Set TM4 Index
  void SetIndex1(unsigned int channel)
  {
    m_TM4Index = channel;
  }
  /// Get TM4 Index
  unsigned int GetIndex1() const
  {
    return m_TM4Index;
  }
  /// Set TM5 Index
  void SetIndex2(unsigned int channel)
  {
    m_TM5Index = channel;
  }
  /// Get TM5 Index
  unsigned int GetIndex2() const
  {
    return m_TM5Index;
  }

  /** Set index, generic method */
  void SetIndex(BandName::BandName band, unsigned int channel)
  {
    if (band == BandName::RED)
      {
      m_TM4Index = channel;
      }
    if (band == BandName::NIR)
      {
      m_TM5Index = channel;
      }
  }
  /** Get index, generic method */
  unsigned int GetIndex(BandName::BandName band) const
  {
    if (band == BandName::RED)
      {
      return m_TM4Index;
      }
    if (band == BandName::NIR)
      {
      return m_TM5Index;
      }
  }

  /** Return the index name */
  virtual std::string GetName() const = 0;

protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1& tm4, const TInput2& tm5) const = 0;

private:
  unsigned int m_TM4Index;
  unsigned int m_TM5Index;
};

/** \class NDBI
 *  \brief This functor computes the Normalized Difference Built Up Index (NDBI)
 *
 *  [Zha 2003]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput1, class TInput2, class TOutput>
class ITK_TEMPLATE_EXPORT NDBI : public TM4AndTM5IndexBase<TInput1, TInput2, TOutput>
{
public:
  /** Return the index name */
  std::string GetName() const override
  {
    return "NDBI";
  }

  /// Constructor
  NDBI() {}
  /// Desctructor
  ~NDBI() override {}
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1& pTM4, const TInput2& pTM5) const override
  {
    double dTM4 = static_cast<double>(pTM4);
    double dTM5 = static_cast<double>(pTM5);
    if (dTM5 + dTM4 == 0)
      {
      return static_cast<TOutput>(0.);
      }

    return (static_cast<TOutput>((dTM5 - dTM4) / (dTM5 + dTM4)));
  }
};

/** \class ISU
 *  \brief This functor computes the Index surfaces built (ISU)
 *
 *  [Abdellaoui 1997 ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 *
 * \ingroup OTBIndices
 */
template <class TInput1, class TInput2, class TOutput>
class ITK_TEMPLATE_EXPORT ISU : public RAndNIRIndexBase<TInput1, TInput2, TOutput>
{
public:
  /** Return the index name */
  std::string GetName() const override
  {
    return "ISU";
  }

  /// Constructor
  ISU() : m_A(100.), m_B(25.) {}
  /// Desctructor
  ~ISU() override {}

  /** Set/Get A correction */
  void SetA(const double pA)
  {
    m_A = pA;
  }
  double GetA(void) const
  {
    return (m_A);
  }
  /** Set/Get B correction */
  void SetB(const double pB)
  {
    m_B = pB;
  }
  double GetB(void) const
  {
    return (m_B);
  }

protected:
  inline TOutput Evaluate(const TInput1& pRed, const TInput2& pNIR) const override
  {
    double dRed = static_cast<double>(pRed);
    double dNIR = static_cast<double>(pNIR);
    if (dNIR == 0)
      {
      return static_cast<TOutput>(0.);
      }

    return (static_cast<TOutput>(m_A - (m_B * dRed) / (dNIR)));
  }

private:
  double m_A;
  double m_B;
};

} // namespace Functor
} // namespace otb

#endif
