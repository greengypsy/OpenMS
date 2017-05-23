// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2017.
//
// This software is released under a three-clause BSD license:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of any author or any participating institution
//    may be used to endorse or promote products derived from this software
//    without specific prior written permission.
// For a full list of authors, refer to the file AUTHORS.
// --------------------------------------------------------------------------
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL ANY OF THE AUTHORS OR THE CONTRIBUTING
// INSTITUTIONS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
// ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// --------------------------------------------------------------------------
// $Maintainer: Timo Sachsenberg $
// $Authors: Stephan Aiche $
// --------------------------------------------------------------------------

#ifndef OPENMS_ANALYSIS_QUANTITATION_ITRAQFOURPLEXQUANTITATIONMETHOD_H
#define OPENMS_ANALYSIS_QUANTITATION_ITRAQFOURPLEXQUANTITATIONMETHOD_H

#include <OpenMS/ANALYSIS/QUANTITATION/IsobaricQuantitationMethod.h>

namespace OpenMS
{
  /**
    @brief iTRAQ 4 plex quantitation to be used with the IsobaricQuantitation.
   
    @htmlinclude OpenMS_ItraqFourPlexQuantitationMethod.parameters
  */
  class OPENMS_DLLAPI ItraqFourPlexQuantitationMethod :
    public IsobaricQuantitationMethod
  {
public:
    /// Default c'tor
    ItraqFourPlexQuantitationMethod();

    /// d'tor
    ~ItraqFourPlexQuantitationMethod();

    /// Copy c'tor
    ItraqFourPlexQuantitationMethod(const ItraqFourPlexQuantitationMethod& other);

    /// Assignment operator
    ItraqFourPlexQuantitationMethod& operator=(const ItraqFourPlexQuantitationMethod& rhs);

    /// @brief Methods to implement from IsobaricQuantitationMethod
    /// @{

    const String& getName() const;

    const IsobaricChannelList& getChannelInformation() const;

    Size getNumberOfChannels() const;

    virtual Matrix<double> getIsotopeCorrectionMatrix() const;

    Size getReferenceChannel() const;
    
    /// @}

private:
    /// the actual information on the different itraq4plex channels.
    IsobaricChannelList channels_;

    /// The name of the quantitation method.
    static const String name_;
    
    /// The reference channel for this experiment.
    Size reference_channel_;

protected:
    /// implemented for DefaultParamHandler
    void setDefaultParams_();

    /// implemented for DefaultParamHandler
    void updateMembers_();
  };
} // namespace

#endif // OPENMS_ANALYSIS_QUANTITATION_ITRAQFOURPLEXQUANTITATIONMETHOD_H
