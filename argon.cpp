#include <iostream>
#include <cmath>
#include <vector>
#include <TCanvas.h>
#include <TRandom3.h>
#include <TApplication.h>
#include <TGraphErrors.h>
#include <fstream>
#include <TH1D.h>
#include "libreria.h"


using namespace std;


namespace dati{
  double kb = 1.38065e-23;   //in Joule/K
  double sigma = 3.4e-10;    //nel SI
  double epsilon = 119* kb;  //epsilon in Joule
  string a1,a2;
  double b1,b2;
  double T=10;    //in kelvin
  double tau = 5e-15;     //5 femptosecondi
  double m = 6.68e-26;    //è la massa in chili
  double eta = 5e11;

}





//MAIN

int main(){


  TApplication app("app",0,NULL);

  vector<double> rx;
  vector<double> ry;
  vector<double> rz;
  vector<string> at;

  int N;

  readfile("ar38_to.xyz", N, at, dati::a1, dati::a2, dati::b1, dati::b2 ,rx, ry,rz);
  ofstream file("/home/delle/Computazionale/Canonico/Dati/output.xyz");

  double U;
  cout.precision(15);



  //Generatore
  
  TRandom3 rnd;
  rnd.SetSeed(time(0));
  TRandom3 rnd_uni;
  rnd_uni.SetSeed(time(0));
  vector<double> vx_star;
  vector<double> vy_star;
  vector<double> vz_star;
  double v_cm_x, v_cm_y, v_cm_z;
  for(int i=0; i<N; i++){
    
    vx_star.push_back(  sqrt(dati::kb*dati::T/dati::m)*rnd.Gaus(0,1));    //velocità casuali estratte da maxwelliana
    vy_star.push_back(  sqrt(dati::kb*dati::T/dati::m)*rnd.Gaus(0,1));
    vz_star.push_back(  sqrt(dati::kb*dati::T/dati::m)*rnd.Gaus(0,1));
    v_cm_x += vx_star[i];
    v_cm_y += vy_star[i];
    v_cm_z += vz_star[i];
  }
  v_cm_x = v_cm_x/N;
  v_cm_y = v_cm_y/N;
  v_cm_z = v_cm_z/N;

  vector<double> vx;
  vector<double> vy;
  vector<double> vz;
  for(int i=0; i<N; i++){
    vx.push_back(vx_star[i] - v_cm_x  );    //queste sono le velocità traslate
    vy.push_back(vy_star[i] - v_cm_y  );    //quelle che lui chiama con v barra
    vz.push_back(vz_star[i] - v_cm_z  );
  }

  double K_bar;
 
  K_bar = energia_cin(N,dati::m,vx,vy,vz);
 

  double K_0;
  K_0 = 3./2*N*dati::kb*dati::T ;
 

  for (int i=0; i<N; i++) {
    vx[i] = vx[i]*sqrt(K_0/K_bar);
    vy[i] = vy[i]*sqrt(K_0/K_bar);
    vz[i] = vz[i]*sqrt(K_0/K_bar);
  }


  //Calcolo della forza

  vector<double> fx;
  vector<double> fy;
  vector<double> fz;


  

  //VELOCITY VERLET

  double t=0;
  double time =0;
  double tmax = 5e-10;
  double dt = dati::tau;
  double ax, ay, az;
  vector<double> ax_temp,  ay_temp,  az_temp;
  TGraph g1;
  TGraph g2, g3, g4;
  // TH1D h("h","",100,0,10);
  int n_passi=0;
  int n_campioni=0;
  int num = 0;
  double E,K;
  double T_temp;
  //  double std_dev = 0;
  double r1, r2, r3;
  double T_max = 30;
  double T_min = 10;
  double T_step = 0.05;
  double T_med = 0;
  double E_med, K_med, U_med;
  //  int T_count = 0;
  double x_cm, y_cm, z_cm;


 
  forza(N,dati::epsilon, dati::sigma, rx,ry,rz,fx,fy,fz);      //prima del loop altrimenti le calcolo troppe volte
  ofstream file1("/home/delle/Computazionale/Canonico/Dati/valori_medi.dat");
  ofstream file2("/home/delle/Computazionale/Canonico/Dati/energia.dat");
  file1.precision(12);
  file2.precision(12);

  while(dati::T<=T_max){
    t = 0;
    T_med =0;
    K_med = 0;
    E_med = 0;
    U_med = 0;
    n_campioni = 0;
    num = 0;

    
    while(t<tmax){

      E=0;
      K=0;
      ax_temp.clear();
      ay_temp.clear();
      az_temp.clear();
      v_cm_x = 0;
      for(int i=0; i<N; i++){
	ax = fx[i]/dati::m;     // questa è a(t) calcolata con f(t) per la i esima particella
	ax_temp.push_back(ax);
	ay = fy[i]/dati::m;     // questa è a(t) calcolata con f(t) per la i esima particella
	ay_temp.push_back(ay);
	az = fz[i]/dati::m;     // questa è a(t) calcolata con f(t) per la i esima particella
	az_temp.push_back(az);
	rx[i] = rx[i] + vx[i]*dt + 0.5 * ax*dt*dt;    //calcolo nuove posizioni
	ry[i] = ry[i] + vy[i]*dt + 0.5 * ay*dt*dt;
	rz[i] = rz[i] + vz[i]*dt + 0.5 * az*dt*dt;
      }

      if(n_passi%25000==0){
	x_cm = 0;
	y_cm = 0;
	z_cm = 0;
	for(int i=0; i<N; i++){
	  x_cm += rx[i];
	  y_cm += ry[i];
	  z_cm += rz[i];
	}
	x_cm = x_cm/N;
	y_cm = y_cm/N;
	z_cm = z_cm/N;
	for(int i=0; i<N; i++){
	  rx[i] = rx[i] - x_cm;   
	  ry[i] = ry[i] - y_cm;
	  rz[i] = rz[i] - z_cm;
	}
	writefile(file,N,at,dati::a1,dati::a2,dati::b1,dati::b2,rx,ry,rz);
	for(int i=0; i<N; i++){
	  rx[i] = rx[i] + x_cm;   
	  ry[i] = ry[i] + y_cm;
	  rz[i] = rz[i] + z_cm;
	}
      }	    
      forza(N,dati::epsilon, dati::sigma, rx,ry,rz,fx,fy,fz); 
    
      for(int i=0; i<N; i++){
     
	ax = fx[i]/dati::m ;   //sono le nuove accelerazioni
	ay = fy[i]/dati::m ;
	az = fz[i]/dati::m ;

	r1 = rnd_uni.Rndm();
	if(r1 > dati::eta*dt){
	  vx[i] = vx[i] + 0.5*( ax_temp[i] + ax )*dt;      //calcolo nuove velocità
	}
	if(r1 <= dati::eta*dt){
	  vx[i] = sqrt(dati::kb*dati::T/dati::m)*rnd.Gaus(0,1);
	}
      
	r2 = rnd_uni.Rndm();
	if(r2 > dati::eta*dt){
	  vy[i] = vy[i] + 0.5*( ay_temp[i] + ay )*dt;
	}
	if(r2 <= dati::eta*dt){
	  vy[i] =  sqrt(dati::kb*dati::T/dati::m)*rnd.Gaus(0,1);
	}
      
	r3 = rnd_uni.Rndm();
	if(r3 > dati::eta*dt){
	  vz[i] = vz[i] + 0.5*( az_temp[i] + az )*dt;
	}
	if(r3 <= dati::eta*dt){
	  vz[i] =  sqrt(dati::kb*dati::T/dati::m)*rnd.Gaus(0,1);
	} 
      }
  
      /*
      g1.SetPoint(n_passi,time,E);
      g2.SetPoint(n_passi,time,U);
      g3.SetPoint(n_passi,time,K);
      g4.SetPoint(n_passi,time,T_temp);
      */

      n_campioni += 1;         //conta il numero di passi da 1 a 100k per una sola temperatura
      n_passi += 1;            //è il numero totale di passi di tutti i cicli
      t += dt;
      if(n_campioni>=5000){
	K = energia_cin(N,dati::m,vx,vy,vz);
	K_med += K;
	U = energia_pot(N, dati::epsilon, dati::sigma, dati::kb, rx, ry, rz);
	U_med += U;
	E = K+U;
	E_med += E;
	T_temp = 2./3 * K /(N*dati::kb);
	T_med += T_temp;
	num += 1;
	if(n_campioni%10==0){
	  file2 << time << '\t' << E << endl;
	}
      }
      time += dt;
    }
    n_passi = n_passi -1;
    K_med = K_med/num;
    U_med = U_med/num;
    E_med = E_med/num;
    T_med = T_med/num;


    file1 << dati::T <<  '\t' << T_med <<  '\t' << K_med <<  '\t' << U_med <<   '\t'  << E_med << endl;
    cout << dati::T << endl;
    cout << T_med << endl;
    cout << endl;
    dati::T += T_step;
  }

  dati::T = dati::T -1;

   forza(N,dati::epsilon, dati::sigma, rx,ry,rz,fx,fy,fz);      //prima del loop altrimenti le calcolo troppe volte

   
  while(dati::T>=T_min){
    t = 0;
    T_med =0;
    K_med = 0;
    E_med = 0;
    U_med = 0;
    n_campioni = 0;
    num = 0;

    
    while(t<tmax){

      E=0;
      K=0;
      ax_temp.clear();
      ay_temp.clear();
      az_temp.clear();
      v_cm_x = 0;
      for(int i=0; i<N; i++){
	ax = fx[i]/dati::m;     // questa è a(t) calcolata con f(t) per la i esima particella
	ax_temp.push_back(ax);
	ay = fy[i]/dati::m;     // questa è a(t) calcolata con f(t) per la i esima particella
	ay_temp.push_back(ay);
	az = fz[i]/dati::m;     // questa è a(t) calcolata con f(t) per la i esima particella
	az_temp.push_back(az);
	rx[i] = rx[i] + vx[i]*dt + 0.5 * ax*dt*dt;    //calcolo nuove posizioni
	ry[i] = ry[i] + vy[i]*dt + 0.5 * ay*dt*dt;
	rz[i] = rz[i] + vz[i]*dt + 0.5 * az*dt*dt;
      }

      if(n_passi%25000==0){
	x_cm = 0;
	y_cm = 0;
	z_cm = 0;
	for(int i=0; i<N; i++){
	  x_cm += rx[i];
	  y_cm += ry[i];
	  z_cm += rz[i];
	}
	x_cm = x_cm/N;
	y_cm = y_cm/N;
	z_cm = z_cm/N;
	for(int i=0; i<N; i++){
	  rx[i] = rx[i] - x_cm;   
	  ry[i] = ry[i] - y_cm;
	  rz[i] = rz[i] - z_cm;
	}
	writefile(file,N,at,dati::a1,dati::a2,dati::b1,dati::b2,rx,ry,rz);
	for(int i=0; i<N; i++){
	  rx[i] = rx[i] + x_cm;   
	  ry[i] = ry[i] + y_cm;
	  rz[i] = rz[i] + z_cm;
	}
      }	    
       forza(N,dati::epsilon, dati::sigma, rx,ry,rz,fx,fy,fz); 
    
      for(int i=0; i<N; i++){
     
	ax = fx[i]/dati::m ;   //sono le nuove accelerazioni
	ay = fy[i]/dati::m ;
	az = fz[i]/dati::m ;

	r1 = rnd_uni.Rndm();
	if(r1 > dati::eta*dt){
	  vx[i] = vx[i] + 0.5*( ax_temp[i] + ax )*dt;      //calcolo nuove velocità
	}
	if(r1 <= dati::eta*dt){
	  vx[i] = sqrt(dati::kb*dati::T/dati::m)*rnd.Gaus(0,1);
	}
      
	r2 = rnd_uni.Rndm();
	if(r2 > dati::eta*dt){
	  vy[i] = vy[i] + 0.5*( ay_temp[i] + ay )*dt;
	}
	if(r2 <= dati::eta*dt){
	  vy[i] =  sqrt(dati::kb*dati::T/dati::m)*rnd.Gaus(0,1);
	}
      
	r3 = rnd_uni.Rndm();
	if(r3 > dati::eta*dt){
	  vz[i] = vz[i] + 0.5*( az_temp[i] + az )*dt;
	}
	if(r3 <= dati::eta*dt){
	  vz[i] =  sqrt(dati::kb*dati::T/dati::m)*rnd.Gaus(0,1);
	} 
      }
   
      /*
      g1.SetPoint(n_passi,time,E);
      g2.SetPoint(n_passi,time,U);
      g3.SetPoint(n_passi,time,K);
      g4.SetPoint(n_passi,time,T_temp);
      */
   
      n_campioni += 1;         //conta il numero di passi da 1 a 100k per una sola temperatura
      n_passi += 1;            //è il numero totale di passi di tutti i cicli
      t += dt;
      if(n_campioni>=5000){
	K = energia_cin(N,dati::m,vx,vy,vz);
	K_med += K;
	U = energia_pot(N, dati::epsilon, dati::sigma, dati::kb, rx, ry, rz);
	U_med += U;
	E = K+U;
	E_med += E;
	T_temp = 2./3 * K /(N*dati::kb);
	T_med += T_temp;
	num += 1;
	if(n_campioni%10==0){
	  file2 << time <<  '\t'  << E << endl;
	}
      }
      time += dt;
    }
    n_passi = n_passi -1;
    K_med = K_med/num;
    U_med = U_med/num;
    E_med = E_med/num;
    T_med = T_med/num;


    file1 << dati::T <<  '\t'   << T_med <<  '\t'   << K_med <<  '\t'   << U_med <<  '\t'   << E_med << endl;
    cout << dati::T << endl;
    cout << T_med << endl;
    cout << endl;
    dati::T -= T_step;
  }

 



  file.close();
  file1.close();
  file2.close();
  
  /*
    for(int i=0; i<n_campioni; i++){
    std_dev += pow( g1.GetPointY(i) - E_med  ,2);
    }
    std_dev = sqrt(std_dev);
    cout << "numero di passi = " << n_passi << endl;
  */


  //GRAFICI CONTROLLO
  /*
    TCanvas c("","",800,600);
  c.Divide(2,2);

  c.cd(1);
  // TCanvas c1;
  g1.SetTitle("Energia");
  g1.Draw("ap");
  c.cd(2);
  //TCanvas c2;
  g2.SetTitle("Energia potenziale");
  g2.Draw("ap");
  c.cd(3);
  // TCanvas c3;
  g3.SetTitle("Energia cinetica");
  g3.Draw("ap");
  c.cd(4);
  //TCanvas c4;
  g4.SetTitle("Temperatura");
  g4.Draw("ap");
  */





  


  return 0;
  app.Run(true);



}
