{

  TGraph g;
  g.SetPoint(0, 1e-15, 3.34677645453109e-24);
  g.SetPoint(1, 2e-15, 9.43174743424951e-24);
  g.SetPoint(2, 5e-15, 3.46670610396446e-23);

  TF1 f("f","[0]*x^2+[1]*x+[2]");

  g.Fit("f");
  g.SetMarkerStyle(31);
  g.Draw("ap");










}
