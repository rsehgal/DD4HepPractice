/*
 * SimpleTube.cpp
 *
 *  Created on: 07-Jan-2019
 *      Author: rsehgal
 */

#include "DD4hep/DetFactoryHelper.h"

using namespace dd4hep;

static Ref_t create_element(Detector& description, xml_h e, SensitiveDetector /* sens */)  {

	xml_det_t x_det = e;
	std::string name = x_det.nameStr();
	xml_dim_t dim = x_det.dimensions();
	DetElement simpleHollowTube( name, x_det.id()  ) ;

	double inner_r = dim.rmin();
	double outer_r = dim.rmax();
	double z_half = dim.zhalf();
	double tube_thick = outer_r - inner_r;

	Tube   tubeSolid (inner_r, outer_r, z_half ) ;
	Volume logicalTube( name+"Logical", tubeSolid ,  description.material("Air") ) ;


	PlacedVolume pv;
	Volume mother =  description.pickMotherVolume( simpleHollowTube );
	pv = mother.placeVolume(logicalTube);
	pv.addPhysVolID( "system", x_det.id() )  ;
    simpleHollowTube.setPlacement( pv );


	return simpleHollowTube;

}

DECLARE_DETELEMENT(SimpleHollowTube,create_element)

