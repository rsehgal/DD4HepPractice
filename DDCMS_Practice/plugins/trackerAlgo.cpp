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
	std::vector<std::string> innerTubeVector    = args.value<std::vector<std::string> >("InnerTubeVector");
	dd4hep::Volume glog1 ;
	dd4hep::Rotation3D rotation;
	for (unsigned int i=0; i<innerTubeVector.size(); ++i) {
		if(i==0){
		dd4hep::Solid solid = dd4hep::Tube(50., 70, 180.);
		ns.addSolidNS(ns.prepend(innerTubeVector[i]), solid);
		//dd4hep::Volume 
		/*
		** Transformation of InnerTube1 will be taken from trackerAlgo.xml file
		*/
		glog1 = dd4hep::Volume(solid.name(), solid, matter);
		//mother.placeVolume(glog1);
		ns.addVolumeNS(glog1);
		}
		if(i==1){
                dd4hep::Solid solid = dd4hep::Tube(30., 40, 220.);
                ns.addSolidNS(ns.prepend(innerTubeVector[i]), solid);
                dd4hep::Volume glog2 = dd4hep::Volume(solid.name(), solid, matter);
                //mother.placeVolume(glog1);
                /*
                ** Transformation of InnerTube2 is not defined in trackerAlgo.xml and is hardcoded here
                */
                glog1.placeVolume(glog2,dd4hep::Transform3D(rotation,dd4hep::Position(0.,0.,100.)));
                ns.addVolumeNS(glog1);
                }
		
		/*
		** A point worth noting, that placement must be defined only at one place 
		** ie. either in XML file or in the DetectorConstructor.
		**
		** Like in the present case, Placement of InnerTube1 is defined in XML file
		** but placement for InnerTube2 is defined as shown above in DetectorConstructor
		*/

	
	}

	std::cout << "Raman................................" << std::endl;
	return 1;
}

// first argument is the type from the xml file
DECLARE_DDCMS_DETELEMENT(DDCMS_trackerAlgo,algorithm)
