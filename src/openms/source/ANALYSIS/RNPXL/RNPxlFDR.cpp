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

#include <OpenMS/ANALYSIS/RNPXL/RNPxlFDR.h>
#include <OpenMS/ANALYSIS/ID/FalseDiscoveryRate.h>
#include <OpenMS/FILTERING/ID/IDFilter.h>
#include <OpenMS/FORMAT/IdXMLFile.h>
#include <OpenMS/FORMAT/TextFile.h>

using namespace std;

namespace OpenMS
{
  RNPxlFDR::RNPxlFDR(size_t report_top_hits):
    report_top_hits_(report_top_hits)
  {
  }

  void RNPxlFDR::QValueAtPSMLevel(vector<PeptideIdentification>& peptide_ids) const
  {
    FalseDiscoveryRate fdr;
    Param p = fdr.getParameters();
    p.setValue("add_decoy_proteins", "true"); // we still want decoys in the result (e.g., to run percolator)
    p.setValue("add_decoy_peptides", "true");
    if (report_top_hits_ >= 2)
    {
      p.setValue("use_all_hits", "true");
    }
    fdr.setParameters(p);
    fdr.apply(peptide_ids);
  }

  void RNPxlFDR::PeptideAndXLQValueAtPSMLevel(
      const vector<ProteinIdentification>& protein_ids,
      const vector<PeptideIdentification>& peptide_ids, 
      vector<PeptideIdentification>& pep_pi,
      double peptide_PSM_qvalue_threshold,
      vector<PeptideIdentification>& xl_pi,
      vector<double> xl_PSM_qvalue_thresholds,
      const String& out_idxml) const
  {
    FalseDiscoveryRate fdr;
    Param p = fdr.getParameters();
    p.setValue("add_decoy_proteins", "true"); // we still want decoys in the result (e.g., to run percolator)
    p.setValue("add_decoy_peptides", "true");
    if (report_top_hits_ >= 2)
    {
      p.setValue("use_all_hits", "true");
    }
    fdr.setParameters(p);

    for (const auto & pi : peptide_ids)
    {
      vector<PeptideHit> pep_ph, xl_ph;
      for (const auto & ph : pi.getHits())
      {
         if (static_cast<int>(ph.getMetaValue("NuXL:isXL")) == 0)
         { // only add best hit
           if (pep_ph.empty() && xl_ph.empty()) pep_ph.push_back(ph); 
         }
         else
         {
           if (pep_ph.empty() && xl_ph.empty()) xl_ph.push_back(ph); 
         }
      }
      if (!pep_ph.empty()) { pep_pi.push_back(pi); pep_pi.back().setHits(pep_ph); }
      if (!xl_ph.empty()) { xl_pi.push_back(pi); xl_pi.back().setHits(xl_ph); }
    }

    // calculate FDRs separately
    fdr.apply(xl_pi); 
    fdr.apply(pep_pi);

    IDFilter::removeDecoyHits(xl_pi);
    IDFilter::removeDecoyHits(pep_pi);

    if (peptide_PSM_qvalue_threshold > 0.0 && peptide_PSM_qvalue_threshold < 1.0)
    {
      IDFilter::filterHitsByScore(pep_pi, peptide_PSM_qvalue_threshold); 
    }

    {
      vector<ProteinIdentification> tmp_prots = protein_ids;
      IDFilter::removeUnreferencedProteins(tmp_prots, pep_pi);
      IdXMLFile().store(out_idxml + String::number(peptide_PSM_qvalue_threshold, 4) + "_peptides.idXML", tmp_prots, pep_pi);
    }

   // treat disabled filtering as 100% FDR
   std::replace(xl_PSM_qvalue_thresholds.begin(), xl_PSM_qvalue_thresholds.end(), 0.0, 1.0);
   std::sort(xl_PSM_qvalue_thresholds.begin(), xl_PSM_qvalue_thresholds.end(), greater<double>()); // important: sort by threshold (descending) to generate results by applying increasingly stringent q-value filters
   for (double xlFDR : xl_PSM_qvalue_thresholds)
   { 
     OPENMS_LOG_INFO << "Writing XL results at xl-FDR: " << xlFDR << endl;
     if (xlFDR > 0.0 && xlFDR < 1.0)
     {
       IDFilter::filterHitsByScore(xl_pi, xlFDR);
     }
     {
       vector<ProteinIdentification> tmp_prots = protein_ids;
       IDFilter::removeUnreferencedProteins(tmp_prots, xl_pi);
       // PSM result
       IdXMLFile().store(out_idxml + String::number(xlFDR, 4) + "_XLs.idXML", tmp_prots, xl_pi);

       // protein result
       TextFile tsv_file;
       RNPxlProteinReport::annotateProteinModificationForTopHits(tmp_prots, xl_pi, tsv_file);
       tsv_file.store(out_idxml + "_proteins" + String::number(xlFDR, 4) + "_XLs.tsv");
     }
   }
  }

} // namespace

