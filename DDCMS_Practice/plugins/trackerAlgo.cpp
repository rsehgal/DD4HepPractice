#include "DD4hep/DetFactoryHelper.h"
#include "DDCMS/DDCMSPlugins.h"
/*
#include "DataFormats/Math/interface/GeantUnits.h"
#include "DetectorDescription/DDCMS/interface/DDPlugins.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
*/

#define EDM_ML_DEBUG

using namespace std;
using namespace dd4hep;
using namespace dd4hep::cms;

//From DD4hep
static long algorithm(Detector& description, ParsingContext& ctxt, xml_h e, SensitiveDetector& /* sens */){
	PlacedVolume  pv;
	Namespace     ns(ctxt, e, true);
	AlgoArguments args(ctxt, e);
	//string        parentName = args.parentName();
	//Volume         mother      = ns.volume(args.parentName());

	return 1;
}

// first argument is the type from the xml file
DECLARE_DDCMS_DETELEMENT(DDCMS_trackerAlgo,algorithm)
