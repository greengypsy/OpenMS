### the directory name
set(directory include/OpenMS/ANALYSIS/RNPXL)

### list all header files of the directory here
set(sources_list_h
HyperScore.h
ModifiedPeptideGenerator.h
MorpheusScore.h
PScore.h
RNPxlAnnotateAndLocate.h
RNPxlAnnotatedHit.h
RNPxlConstants.h
RNPxlFDR.h
RNPxlFragmentAdductDefinition.h
RNPxlFragmentAnnotationHelper.h
RNPxlFragmentIonGenerator.h
RNPxlMarkerIonExtractor.h
RNPxlModificationsGenerator.h
RNPxlParameterParsing.h
RNPxlReport.h
)

### add path to the filenames
set(sources_h)
foreach(i ${sources_list_h})
	list(APPEND sources_h ${directory}/${i})
endforeach(i)

### source group definition
source_group("Header Files\\OpenMS\\ANALYSIS\\RNPXL" FILES ${sources_h})

set(OpenMS_sources_h ${OpenMS_sources_h} ${sources_h})

