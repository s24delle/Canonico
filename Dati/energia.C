{

  TGraph g;
  ifstream file("energia.dat");
  double t,E;
  int i=0;
  
  while(file>>t>>E){
    file >> t >> E;
    g.SetPoint(i,t,E);
    i+=1;
  }


  //  TF1 f("f","[0]*x^2+[1]*x+[2]");

  //  g.Fit("f");
  // g.SetMarkerStyle(31);
  g.Draw("ap");










}
