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
	string        parentName = args.parentName();
	Volume         mother      = ns.volume(args.parentName());
	std::cout <<"Mother Name : " << mother.name() << std::endl;
	double                   waferSize  = args.value<double>("WaferSize");    // size of wafer
	double                   waferT     = args.value<double>("WaferThick");   // Thickness of wafer
	double                   cellT      = args.value<double>("CellThick");    // Thickness of depletion layer
	int                      nCells     = args.value<int>("NCells");          // Number of cells
	int                      posSens    = args.value<int>("PosSensitive");    // Position of delpletion layer within wafer
	std::string              material   = args.value<std::string>("Material");// Material
	std::string              fullCN     = args.value<std::string>("FullCell");// Name of the full cell
	std::string              fullSensN  = args.value<std::string>("FullSensitive");// Name of the sensitive part for a full cell
	std::vector<std::string> truncCN    = args.value<std::vector<std::string> >("TruncatedCell");       // Names of truncated cells
	//std::vector<std::string> truncSensN = args.value<std::vector<std::string> >("TruncatedSensitive");  // Names of the sensitive parts for truncated cells
	std::vector<std::string> extenCN    = args.value<std::vector<std::string> >("ExtendedCell");        // Names of extended cells
	//std::vector<std::string> extenSensN = args.value<std::vector<std::string> >("ExtendedSensitive");   // Names of the sensitive parts for extended cells
	std::vector<std::string> cornrCN    = args.value<std::vector<std::string> >("CornerCell");          // Names of the corner cells
	//std::vector<std::string> cornrSensN = args.value<std::vector<std::string> >("CornerSensitive");     // Names of the sensitive parts for corner cells
	
	dd4hep::Material matter = ns.material(material);

//Some PreChecks
/*
	 if ((truncCN.size() != truncSensN.size()) ||
	        (extenCN.size() != extenSensN.size()) ||
	        (cornrCN.size() != cornrSensN.size())) {

		  std::cout << "The number of cells & sensitive differ:"
			       << " Truncated " << truncCN.size() << ":"
			       << truncSensN.size() << " Extended "
			       << extenCN.size() <<":" << extenSensN.size()
			       << " Corners " << cornrCN.size() << ":"
			       << cornrSensN.size();
		  }

	  if ((truncCN.size() != 3) || (extenCN.size() != 3) ||
	        (cornrCN.size() != 6)) {
		  std::cout << "DDHGCalCell: The number of cells does not"
	  			       << " match with Standard: Truncated "
	  			       << truncCN.size() << ":3 Extended "
	  			       << extenCN.size() <<":3" << " Corners "
	  			       << cornrCN.size() << ":6";
	  }

#ifdef EDM_ML_DEBUG
	  std::cout << "DDHGCalCell: Wafer r " << waferSize << std::endl
				<< " T " << waferT << " Cell T " << cellT << std::endl
				<< " Cells/Wafer " << nCells << std::endl
				<< " Material " << material << std::endl
				<< "Sensitive Position " << posSens << std::endl
				<< " Full Cell: " << fullCN << ":"	<< fullSensN << std::endl;
  for (int k=0; k<3; ++k)
	  std::cout << "DDHGCalCell: Truncated Cell[" << k
				  << "] " << truncCN[k] << ":"
				  << truncSensN[k];
  for (int k=0; k<3; ++k)
	  std::cout << "DDHGCalCell: Extended Cell[" << k
				  << "] " << extenCN[k] << ":"
				  << extenSensN[k];
  for (int k=0; k<6; ++k)
	  std::cout << "DDHGCalCell: Corner Cell[" << k
				  << "] " << cornrCN[k] << ":"
				  << cornrSensN[k];
  //std::cout << "Name space " << ns.name();
#endif

  dd4hep::Material matter = ns.material(material);
#ifdef EDM_ML_DEBUG
  std::cout << "DDHGCalCell: " << material
				<< " initialized at " << &matter;
#endif

*/
  dd4hep::Volume glog1, glog2;

  static const double sqrt3    = std::sqrt(3.0);
  static const double waf2cell = 3.0;
  static const double cornerfac= 2.5;
  double R   = waferSize/(waf2cell*nCells);
  double r   = 0.5*R*sqrt3;
  double dx1 = R;
  double dx2 = 0.5*dx1;
  double dx3 = cornerfac*dx2;
  double dx4 = 0.5*dx2;
  double dy1 = r;
  double dy2 = 0.5*dy1;
  double dy3 = 1.5*dy1;
  std::vector<double> xx = {dx1, dx2,-dx2,-dx1,-dx2,dx2,
			    dx3,-dx4,-dx1,-dx1,-dx4,dx3};
  std::vector<double> yy = {0,  dy1,dy1,   0,-dy1,-dy1,
			    dy2,dy3,dy1,-dy1,-dy3,-dy2};
  double zpos = (posSens == 0) ? -0.5*(waferT-cellT) : 0.5*(waferT-cellT);
  dd4hep::Position tran(0,0,zpos);
  
  // First the full cell
  std::vector<double> xw = {xx[0],xx[1],xx[2],xx[3],xx[4],xx[5]};
  std::vector<double> yw = {yy[0],yy[1],yy[2],yy[3],yy[4],yy[5]};
  std::vector<double> zw = {-0.5*waferT,0.5*waferT};
  std::vector<double> zx(2,0), zy(2,0), scale(2,1.0);

  //Creating Wafer
  dd4hep::Solid solid ;
  
  /*
  solid = dd4hep::ExtrudedPolygon(xw, yw, zw, zx, zy, scale); 
  ns.addSolidNS(ns.prepend(fullCN), solid);
  glog1 = dd4hep::Volume(solid.name(), solid, matter);
  ns.addVolumeNS(glog1);
#ifdef EDM_ML_DEBUG
  std::cout << std::endl << std::endl << "DDHGCalCell: " << solid.name()
				<< " extruded polygon made of " << material << std::endl
				<< " z|x|y|s (0) " << zw[0] << ":" << zx[0] 
				<< ":" << zy[0] << ":" << scale[0] << std::endl
				<< " z|x|y|s (1) " << zw[1] << ":" << zx[1] 
				<< ":" << zy[1] << ":" << scale[1] << " and "
				<< xw.size() << " edges"<< std::endl;
  for (unsigned int k=0; k<xw.size(); ++k)
	  std::cout << "[" << k << "] " << xw[k] << ":" << yw[k];
#endif
*/
  //Creating Cell
  std::vector<double> zc = {-0.5*cellT,0.5*cellT};
  solid = dd4hep::ExtrudedPolygon(xw, yw, zc, zx, zy, scale);
  ns.addSolidNS(ns.prepend(fullSensN), solid);
  glog2 = dd4hep::Volume(solid.name(), solid, matter);
  ns.addVolumeNS(glog2);
#ifdef EDM_ML_DEBUG
  std::cout << "DDHGCalCell: " << solid.name()
				<< " extruded polygon made of " << material << std::endl
				<< " z|x|y|s (0) " << zc[0] << ":" << zx[0] 
				<< ":" << zy[0] << ":" << scale[0]  << std::endl
				<< " z|x|y|s (1) " << zc[1] << ":" << zx[1] 
				<< ":" << zy[1] << ":" << scale[1] << " and "
				<< xw.size() << " edges" << std::endl;
  for (unsigned int k=0; k<xw.size(); ++k)
	  std::cout << "[" << k << "] " << xw[k] << ":" << yw[k];
#endif

  dd4hep::Rotation3D rotation;
  //glog1.placeVolume(glog2, 1, dd4hep::Transform3D(rotation,tran));
  
//*****************************************************************************  

{
  //double waferSize_ = 167.4408;
  //double waferSepar_ = 1.;
  // int nCells_= 12;
   
   
   double                   waferSize_  = args.value<double>("WaferSize"); 
   double                   waferSepar_  = args.value<double>("SensorSeparation"); 
   int		                nCells_  = args.value<double>("NCells"); 
   
  
  static const double sqrt3 = std::sqrt(3.0);
  double rM  = 0.5*(waferSize_ + waferSepar_);
  double RM2 = rM/sqrt3;
  double R   = waferSize_/(3.0*nCells_);
  double r   = 0.5*R*sqrt3;


 int counter = 0;
  for (int u=0; u<2*nCells_; ++u) {
    for (int v=0; v<2*nCells_; ++v) {
      if (((v-u) < nCells_) && (u-v) <= nCells_) {
      counter++;
	int    n2 = nCells_/2;
	double yp = (u-0.5*v-n2)*2*r;
	double xp = (1.5*(v-nCells_)+1.0)*R;
	int    cell(0);
	if      ((u == 0)           && (v == 0))           cell = 7;
	else if ((u == 0)           && (v == nCells_-1))   cell = 8;
	else if ((u == nCells_)     && (v == 2*nCells_-1)) cell = 9;
	else if ((u == 2*nCells_-1) && (v == 2*nCells_-1)) cell = 10;
	else if ((u == 2*nCells_-1) && (v == nCells_-1))   cell = 11;
	else if ((u == nCells_)     && (v == 0))           cell = 12;
	else if (u == 0)                                   cell = 1;
	else if ((v-u) == (nCells_-1))                     cell = 4;
	else if (v == (2*nCells_-1))                       cell = 2;
	else if (u == (2*nCells_-1))                       cell = 5;
	else if ((u-v) == nCells_)                         cell = 3;
	else if (v == 0)                                   cell = 6;
//	DDTranslation tran(xp,yp,0);
	//int copy = (cellType_*100 + v)*100 + u;
	//cpv.position(DDName(cellNames_[cell]), glog, copy, tran, rot);
#ifdef EDM_ML_DEBUG
	std::cout  <<" at : " << xp <<" : " << yp << std::endl;
#endif

dd4hep::Position tran(xp,yp,0.);
mother.placeVolume(glog2, counter, dd4hep::Transform3D(rotation,tran));

      }
    }

  }
  std::cout << "Counter : " << counter << std::endl;
  
  }
//*****************************************************************************
  
  
  
  
 // mother.placeVolume(glog1);
#ifdef EDM_ML_DEBUG
  std::cout << "DDHGCalCell: " << glog2.name() << std::endl
				<< " number 1 position in " << glog1.name() << std::endl
				<< " at " << tran << " with " << rotation << std::endl;
#endif
/*
  static const int ir0[] = {0,1,0};
  static const int ir1[] = {1,2,1};
  static const int ir2[] = {2,3,3};
  static const int ir3[] = {3,4,4};
  static const int ir4[] = {5,5,5};
  for (int i=0; i<3; ++i) {
    std::vector<double> xw = {xx[ir0[i]],xx[ir1[i]],xx[ir2[i]],xx[ir3[i]],xx[ir4[i]]};
    std::vector<double> yw = {yy[ir0[i]],yy[ir1[i]],yy[ir2[i]],yy[ir3[i]],yy[ir4[i]]};

    solid = dd4hep::ExtrudedPolygon(xw, yw, zw, zx, zy, scale);
    ns.addSolidNS(ns.prepend(truncCN[i]), solid);
    glog1 = dd4hep::Volume(solid.name(), solid, matter);
    ns.addVolumeNS(glog1);
#ifdef EDM_ML_DEBUG
    std::cout << "DDHGCalCell: " << solid.name()
				  << " extruded polygon made of " << material << std::endl
				  << " z|x|y|s (0) " << zw[0] << ":" << zx[0] 
				  << ":" << zy[0] << ":" << scale[0]  << std::endl
				  << " z|x|y|s (1) " << zw[1] << ":" << zx[1] 
				  << ":" << zy[1] << ":" << scale[1] << " and "
				  << xw.size() << " edges" << std::endl;
    for (unsigned int k=0; k<xw.size(); ++k)
    	std::cout << "[" << k << "] " << xw[k] << ":"
				    << yw[k];
#endif

    solid = dd4hep::ExtrudedPolygon(xw, yw, zc, zx, zy, scale);
    ns.addSolidNS(ns.prepend(truncSensN[i]), solid);
    glog2 = dd4hep::Volume(solid.name(), solid, matter);
    ns.addVolumeNS(glog2);
#ifdef EDM_ML_DEBUG
    std::cout << "DDHGCalCell: " << solid.name()
				  << " extruded polygon made of " << material << std::endl
				  << " z|x|y|s (0) " << zw[0] << ":" << zx[0] 
				  << ":" << zy[0] << ":" << scale[0]  << std::endl
				  << " z|x|y|s (1) " << zw[1] << ":" << zx[1] 
				  << ":" << zy[1] << ":" << scale[1] << " and "
				  << xw.size() << " edges" << std::endl;
    for (unsigned int k=0; k<xw.size(); ++k)
    	std::cout << "[" << k << "] " << xw[k] << ":"
				    << yw[k];
#endif
    glog1.placeVolume(glog2, 1, dd4hep::Transform3D(rotation,tran));
#ifdef EDM_ML_DEBUG
    std::cout << "DDHGCalCell: " << glog2.name()
				  << " number 1 position in " << glog1.name() << std::endl
				  << " at " << tran << " with " << rotation << std::endl;
#endif
  }

  static const int ie0[] = {1,5,0};
  static const int ie1[] = {2,6,1};
  static const int ie2[] = {3,7,8};
  static const int ie3[] = {10,3,9};
  static const int ie4[] = {11,4,5};
  for (int i=0; i<3; ++i) {
    std::vector<double> xw = {xx[ie0[i]],xx[ie1[i]],xx[ie2[i]],xx[ie3[i]],xx[ie4[i]]};
    std::vector<double> yw = {yy[ie0[i]],yy[ie1[i]],yy[ie2[i]],yy[ie3[i]],yy[ie4[i]]};
    solid = dd4hep::ExtrudedPolygon(xw, yw, zw, zx, zy, scale); 
    ns.addSolidNS(ns.prepend(extenCN[i]), solid);
    glog1 = dd4hep::Volume(solid.name(), solid, matter);
    ns.addVolumeNS(glog1);
#ifdef EDM_ML_DEBUG
    std::cout << "DDHGCalCell: " << solid.name()
				  << " extruded polygon made of " << material << std::endl
				  << " z|x|y|s (0) " << zw[0] << ":" << zx[0] 
				  << ":" << zy[0] << ":" << scale[0]  << std::endl
				  << " z|x|y|s (1) " << zw[1] << ":" << zx[1] 
				  << ":" << zy[1] << ":" << scale[1] << " and "
				  << xw.size() << " edges" << std::endl;
    for (unsigned int k=0; k<xw.size(); ++k)
    	std::cout << "[" << k << "] " << xw[k] << ":"
				    << yw[k];
#endif
    solid = dd4hep::ExtrudedPolygon(xw, yw, zc, zx, zy, scale);
    ns.addSolidNS(ns.prepend(extenSensN[i]), solid);
    glog2 = dd4hep::Volume(solid.name(), solid, matter);
    ns.addVolumeNS(glog2);
#ifdef EDM_ML_DEBUG
    std::cout << "DDHGCalCell: " << solid.name()
				  << " extruded polygon made of " << material << std::endl
				  << " z|x|y|s (0) " << zw[0] << ":" << zx[0] 
				  << ":" << zy[0] << ":" << scale[0] << std::endl
				  << " z|x|y|s (1) " << zw[1] << ":" << zx[1] 
				  << ":" << zy[1] << ":" << scale[1] << " and "
				  << xw.size() << " edges" << std::endl;
    for (unsigned int k=0; k<xw.size(); ++k)
    	std::cout << "[" << k << "] " << xw[k] << ":"
				    << yw[k];
#endif
    glog1.placeVolume(glog2, 1, dd4hep::Transform3D(rotation,tran));
#ifdef EDM_ML_DEBUG
    std::cout << "DDHGCalCell: " << glog2.name() << std::endl
				  << " number 1 position in " << glog1.name() << std::endl
				  << " at " << tran << " with " << rotation << std::endl ;
#endif
  }

  static const int ic0[] = {0,1,1,1,1,0};
  static const int ic1[] = {1,2,2,7,3,1};
  static const int ic2[] = {8,3,3,3,4,3};
  static const int ic3[] = {3,5,10,4,5,9};
  static const int ic4[] = {5,11,5,5,6,5};
  for (int i=0; i<6; ++i) {
    std::vector<double> xw = {xx[ic0[i]],xx[ic1[i]],xx[ic2[i]],xx[ic3[i]],xx[ic4[i]]};
    std::vector<double> yw = {yy[ic0[i]],yy[ic1[i]],yy[ic2[i]],yy[ic3[i]],yy[ic4[i]]};
    solid = dd4hep::ExtrudedPolygon(xw, yw, zw, zx, zy, scale);
    ns.addSolidNS(ns.prepend(cornrCN[i]), solid);
    glog1 = dd4hep::Volume(solid.name(), solid, matter);
    ns.addVolumeNS(glog1);
#ifdef EDM_ML_DEBUG
    std::cout << "DDHGCalCell: " << solid.name()
				  << " extruded polygon made of " << material << std::endl
				  << " z|x|y|s (0) " << zw[0] << ":" << zx[0] 
				  << ":" << zy[0] << ":" << scale[0]  << std::endl
				  << " z|x|y|s (1) " << zw[1] << ":" << zx[1] 
				  << ":" << zy[1] << ":" << scale[1] << " and "
				  << xw.size() << " edges" << std::endl;
    for (unsigned int k=0; k<xw.size(); ++k)
    	std::cout << "[" << k << "] " << xw[k] << ":"
				    << yw[k];
#endif
    solid = dd4hep::ExtrudedPolygon(xw, yw, zc, zx, zy, scale);
    ns.addSolidNS(ns.prepend(cornrSensN[i]), solid);
    glog2 = dd4hep::Volume(solid.name(), solid, matter);
    ns.addVolumeNS(glog2);
#ifdef EDM_ML_DEBUG
    std::cout << "DDHGCalCell: " << solid.name()
				  << " extruded polygon made of " << material << std::endl
				  << " z|x|y|s (0) " << zw[0] << ":" << zx[0] 
				  << ":" << zy[0] << ":" << scale[0]  << std::endl
				  << " z|x|y|s (1) " << zw[1] << ":" << zx[1] 
				  << ":" << zy[1] << ":" << scale[1] << " and "
				  << xw.size() << " edges" << std::endl;
    for (unsigned int k=0; k<xw.size(); ++k)
      std::cout << "[" << k << "] " << xw[k] << ":"
				    << yw[k];
#endif
    glog1.placeVolume(glog2, 1, dd4hep::Transform3D(rotation,tran));
    //mother.placeVolume(glog1,1,dd4hep::Transform3D(rotation,tran));
#ifdef EDM_ML_DEBUG
    std::cout  << "DDHGCalCell: " << glog2.name()
				  << " number 1 position in " << glog1.name() << std::endl
				  << " at " << tran << " with " << rotation << std::endl;
#endif
  }
  
  */
  return 1;
  
}

// first argument is the type from the xml file
DECLARE_DDCMS_DETELEMENT(DDCMS_HGCal_DDHGCalCell,algorithm)
