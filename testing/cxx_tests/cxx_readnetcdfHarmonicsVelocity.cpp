//------------------------------GPL---------------------------------------//
// This file is part of ADCIRCModules.
//
// (c) 2015-2018 Zachary Cobell
//
// ADCIRCModules is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// ADCIRCModules is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with ADCIRCModules.  If not, see <http://www.gnu.org/licenses/>.
//------------------------------------------------------------------------//
#include "adcirc.h"
#include <iostream>
#include <cmath>

int main(int argc, char *argv[]) {

  Adcirc::Output::HarmonicsOutput *harm = new Adcirc::Output::HarmonicsOutput("test_files/fort.54.nc",true);
  int ierr = harm->read();
  if(ierr!=Adcirc::NoError){
      delete harm;
      return ierr;
  }

  double m2_u_mag = harm->u_magnitude("M2")->value(0);
  double m2_u_pha = harm->u_phase("M2")->value(0);
  double m2_v_mag = harm->v_magnitude("M2")->value(0);
  double m2_v_pha = harm->v_phase("M2")->value(0);
  
  double k1_u_mag = harm->u_magnitude("K1")->value(50);
  double k1_u_pha = harm->u_phase("K1")->value(50);
  double k1_v_mag = harm->v_magnitude("K1")->value(50);
  double k1_v_pha = harm->v_phase("K1")->value(50);
  
  printf("%22.15f\n",m2_u_mag);
  printf("%22.15f\n",m2_u_pha);
  printf("%22.15f\n",m2_v_mag);
  printf("%22.15f\n",m2_v_pha);
  
  printf("%22.15f\n",k1_u_mag);
  printf("%22.15f\n",k1_u_pha);
  printf("%22.15f\n",k1_v_mag);
  printf("%22.15f\n",k1_v_pha);

  if( fabs(m2_u_mag-7.301591213966624)>0.000001 || fabs(m2_u_pha-100.599489050011670)>0.000001 || 
      fabs(m2_v_mag-3.435861392193815)>0.000001 || fabs(m2_v_pha-100.599488993260096)>0.000001 || 
      fabs(k1_u_mag-0.670506540758823)>0.000001 || fabs(k1_u_pha-272.992844771240755)>0.000001 || 
      fabs(k1_v_mag-0.407799597713285)>0.000001 || fabs(k1_v_pha-219.136772397306800)>0.000001 ) {
      delete harm;
      return 1;
  }

  delete harm;
  return 0;

}
