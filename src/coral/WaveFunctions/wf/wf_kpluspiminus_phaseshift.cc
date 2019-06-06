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
#ifndef __CWAVEFUNCTION_WF_KPLUSPIMINUS_PHASESHIFT_CC__
#define __CWAVEFUNCTION_WF_KPLUSPIMINUS_PHASESHIFT_CC__
#include "wavefunction.h"

CWaveFunction_kpluspiminus_phaseshift::CWaveFunction_kpluspiminus_phaseshift(string  parsfilename) : CWaveFunction() {
  int iq,ichannel;
  double q;
  ParsInit(parsfilename);

  m1=MKAON;
  m2=MPI;
	IDENTICAL=0;
  q1q2=-1;
  if(COULOMB==0) q1q2=0;
  nchannels=1;

  ellmax=1;
  InitArrays();
  printf("Arrays Initialized\n");

  ell[0]=1;

  InitWaves();
  printf("Partial Waves Initialized\n");

  channelweight[0]=3.0;

  get_phaseshifts_kpluspiminus();

  for(ichannel=0;ichannel<nchannels;ichannel++){
    for(iq=0;iq<nqmax;iq++){
      q=qarray[iq];
      
      if(q1q2!=0)
	CoulWave::phaseshift_CoulombCorrect(ell[ichannel],q,eta[iq],
				  delta[ichannel][iq],ddeltadq[ichannel][iq]);
      Wepsilon[ichannel][iq]=ddeltadq[ichannel][iq]
	-GetIW(ell[ichannel],epsilon,q,q1q2,eta[iq],delta[ichannel][iq])
	+GetIW(ell[ichannel],epsilon,q,q1q2,eta[iq],0.0);
      Wepsilon[ichannel][iq]=3.0*Wepsilon[ichannel][iq]
	/(4.0*PI*pow(epsilon,3));
    }
  }
  printf("Initialization finished\n");
}

double CWaveFunction_kpluspiminus_phaseshift::CalcPsiSquared(int iq,double r,double ctheta){
  double psisquared,x,dpsi2,q;
  complex<double> psi,hstar,psi0;

  q=qarray[iq];
  if(iq>=nqmax){
    printf("iq too large!\n");
    exit(1);
  }
  psi0=planewave[iq]->planewave(r,ctheta);

  if(STRONG==1){
    if(r<epsilon){
      psisquared=real(psi0*conj(psi0));
      dpsi2=channelweight[0]*2.0*PI*Wepsilon[0][iq]
	*pow(HBARC,3)/(q*q);
      psisquared+=dpsi2;
    }
    else{
      x=q*r/HBARC;
 
      hstar=partwave[ell[0]][iq]->GetPhiIncoming(r)/x;
      psi=psi0+0.5*hstar*(Misc::ceiphi(-2.0*delta[0][iq])-1.0)*ci*(3.0)*ctheta;

      psisquared=((2.0/3.0)*real(psi*conj(psi)))
	+((1.0/3.0)*real(psi0*conj(psi0)));

    }
  }
  else psisquared=real(psi0*conj(psi0));
	psisquared*=RelativisticCorrection(r,iq);
  return psisquared;

}


void CWaveFunction_kpluspiminus_phaseshift::get_phaseshifts_kpluspiminus(){
  double gamma0=50.0,mres=891.66,a=1.0/500.0;
  int ellres=1;
  double e1,e2,q,m,gamma,q0,mtest,tandelta,dtandeltadq,old=0.0;
  int iq;

  q0=sqrt((pow(mres,4.0)+pow(m1,4.0)+pow(m2,4.0)
	   -2*m1*m1*m2*m2-2*mres*mres*(m1*m1+m2*m2))/(4.0*mres*mres));
  mtest=sqrt(m1*m1+q0*q0)+sqrt(m2*m2+q0*q0);
  printf("mtest=%g =? 891.66, q0=%g\n",mtest,q0);
  for(iq=0;iq<=nqmax;iq++){
    q=qarray[iq];
    gamma=gamma0*pow((q/q0)*(1.0+q0*q0*a*a)/(1.0+q*q*a*a),double(2*ellres+1));
    e1=sqrt(m1*m1+q*q);
    e2=sqrt(m2*m2+q*q);
    m=e1+e2;
    tandelta=gamma/(mres-m);
    delta[0][iq]=atan2(gamma,mres-m);
    dtandeltadq=tandelta*3.0*((1.0/q)-2.0*q*a*a/(1.0+q*q*a*a));
    dtandeltadq+=(tandelta/(mres-m))*((q/e1)+(q/e2));
    ddeltadq[0][iq]=dtandeltadq/(1.0+tandelta*tandelta);
    
    if(iq>0) old=delta[0][iq-1];
    printf("q=%g, m=%g, delta=%g, ddeltadq=%g =? %g\n",
	     q,m,delta[0][iq]*180.0/PI,ddeltadq[0][iq]*180.0/PI,
	   (delta[0][iq]-old)*180.0/(delq*PI));

  }
}

#endif