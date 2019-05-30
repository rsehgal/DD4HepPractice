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

static long algorithm(Detector& description, ParsingContext& ctxt, xml_h e, SensitiveDetector& /* sens */){
	PlacedVolume  pv;
	Namespace     ns(ctxt, e, true);
	AlgoArguments args(ctxt, e);
	string        parentName = args.parentName();
	Volume         mother      = ns.volume(args.parentName());
	std::string              material   = args.value<std::string>("Material");
	dd4hep::Material matter = ns.material(material);
	std::cout << "===================================================================" << std::endl;
	std::cout << "Mother Name : " << mother.name() << " : Material : " << material << std::endl;
	std::cout << "===================================================================" << std::endl;
	std::vector<std::string> innerConeVector    = args.value<std::vector<std::string> >("InnerConeVector");
	for (unsigned int i=0; i<innerConeVector.size(); ++i) {
		if(i==0){
		dd4hep::Solid solid = dd4hep::Cone(10., 20.,30., 40., 50.);
		ns.addSolidNS(ns.prepend(innerConeVector[i]), solid);
		dd4hep::Volume glog1 = dd4hep::Volume(solid.name(), solid, matter);
		//mother.placeVolume(glog1);
		ns.addVolumeNS(glog1);
		}
		if(i==1){
                dd4hep::Solid solid = dd4hep::Cone(50., 20.,40., 40., 50.);
                ns.addSolidNS(ns.prepend(innerConeVector[i]), solid);
                dd4hep::Volume glog1 = dd4hep::Volume(solid.name(), solid, matter);
                //mother.placeVolume(glog1);
                ns.addVolumeNS(glog1);
                }
       if(i==2){
                dd4hep::Solid solid = dd4hep::Tube(40., 50.,20);
                ns.addSolidNS(ns.prepend(innerConeVector[i]), solid);
                dd4hep::Volume glog1 = dd4hep::Volume(solid.name(), solid, matter);
                //mother.placeVolume(glog1);
                ns.addVolumeNS(glog1);
                }
		

	
	}

	std::cout << "Raman................................" << std::endl;
	return 1;
}

// first argument is the type from the xml file
DECLARE_DDCMS_DETELEMENT(DDCMS_coneAlgo,algorithm)
