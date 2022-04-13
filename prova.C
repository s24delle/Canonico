{

  TRandom3 rnd;
  TH1D h("h","",80,-10,10);
  double kb = 1.38065e-23;
  double T=10;
  double m = 6.68e-26;  
  TF1 f("f","[0]*exp(-(x^2)/[1])");
  f.FixParameter(0,sqrt(m/(2*M_PI*kb*T)));
  f.FixParameter(1,pow(2*kb*T/m,2));
  
  for(int i=0; i<100000; i++){
    h.Fill( f.GetRandom());
  }


  //h.Fit("f");
  h.Draw("");


}
