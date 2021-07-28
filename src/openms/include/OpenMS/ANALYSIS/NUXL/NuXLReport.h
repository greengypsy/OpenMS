// --------------------------------------------------------------------------
//                   OpenMS -- Open-Source Mass Spectrometry
// --------------------------------------------------------------------------
// Copyright The OpenMS Team -- Eberhard Karls University Tuebingen,
// ETH Zurich, and Freie Universitaet Berlin 2002-2020.
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
// $Authors: Timo Sachsenberg $
// --------------------------------------------------------------------------

#pragma once

#include <OpenMS/KERNEL/StandardTypes.h>
#include <OpenMS/ANALYSIS/NUXL/NuXLMarkerIonExtractor.h>
#include <OpenMS/DATASTRUCTURES/ListUtilsIO.h>

#include <OpenMS/METADATA/PeptideIdentification.h>
#include <OpenMS/KERNEL/MSSpectrum.h>
#include <OpenMS/KERNEL/MSExperiment.h>
#include <OpenMS/CONCEPT/Constants.h>
#include <OpenMS/FORMAT/TextFile.h>

#include <vector>

namespace OpenMS
{

/// @brief struct to hold a single report line
struct OPENMS_DLLAPI NuXLReportRow
{
  bool no_id;

  // columns
  double rt;
  double original_mz;
  String accessions;
  String peptide;
  String NA;
  Int charge;
  double score;
  int rank;
  double best_localization_score;
  String localization_scores;
  String best_localization;
  double peptide_weight;
  double NA_weight;
  double xl_weight;
  StringList meta_values; // the actual values of exported metadata
  NuXLMarkerIonExtractor::MarkerIonsType marker_ions;
  double abs_prec_error;
  double rel_prec_error;
  double m_H;
  double m_2H;
  double m_3H;
  double m_4H;
  String fragment_annotation;
  String getString(const String& separator) const;
};

/// create header line
struct OPENMS_DLLAPI NuXLReportRowHeader
{
  static String getString(const String& separator, const StringList& meta_values_to_export);
};

/// create PSM report
struct OPENMS_DLLAPI NuXLReport
{
  static std::vector<NuXLReportRow> annotate(
    const PeakMap& spectra, 
    std::vector<PeptideIdentification>& peptide_ids, 
    const StringList& meta_values_to_export,
    double marker_ions_tolerance);
};


/// protein report
struct OPENMS_DLLAPI RNPxlProteinReport
{
  static void annotateProteinModificationForTopHits(std::vector<ProteinIdentification>& prot_ids, 
    const std::vector<PeptideIdentification>& peps, 
    TextFile& tsv_file,
    bool report_decoys = false);

  static void mapAccessionToTDProteins(ProteinIdentification& prot_id, std::map<String, ProteinHit*>& acc2protein_targets, std::map<String, ProteinHit*>& acc2protein_decoys)
  {
    std::vector<ProteinHit>& proteins = prot_id.getHits();
    for (ProteinHit& protein : proteins)
    {
      if (protein.getMetaValue("target_decoy").toString().hasPrefix("target"))
      {
        acc2protein_targets[protein.getAccession()] = &protein;
      }
      else
      {
        acc2protein_decoys[protein.getAccession()] = &protein;
      }
    }
  }
  
};

}

