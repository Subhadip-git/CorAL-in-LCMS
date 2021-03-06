// <<BEGIN-copyright>>
// 
//                 The GNU General Public License (GPL) Version 2, June 1991
// 
// Copyright (c) 2013, Lawrence Livermore National Security, LLC. Produced at the Lawrence 
// Livermore National Laboratory. Written by Ron Soltz (soltz1@llnl.gov), David A. Brown 
// (dbrown@bnl.gov) and Scott Pratt (pratts@pa.msu.edu).
// 
// CODE-CODE-643336 All rights reserved. 
// 
// This file is part of CorAL, Version: 1.17.
// 
// Please see the file LICENSE.TXT in the main directory of this source code distribution.
// 
// This program is free software; you can redistribute it and/or modify it under the terms of 
// the GNU General Public License (as published by the Free Software Foundation) version 2, 
// dated June 1991.
// 
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; 
// without even the IMPLIED WARRANTY OF MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
// See the terms and conditions of the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with this program; 
// if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, 
// MA 02111-1307 USA
// 
// <<END-copyright>>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include <complex>

using namespace std;

#include "constants.h"
#include "sf.h"

int main(){
  double x,y;
  complex<double> z(3.0,2.3523);
  complex<double> ci(0.0,1.0);
  printf("Enter real and imag parts of z :");
  scanf("%lf %lf",&x,&y);
  z=x+ci*y;

  printf("(%g,%g) =? (%g,%g)\n",real(CoulWave::cgamma(z)),
	 imag(CoulWave::cgamma(z)), 
       0.0, //	 real(CoulWave::altcgamma(z)),
       0.0); //	 imag(CoulWave::altcgamma(z)));
  return 0;
}


