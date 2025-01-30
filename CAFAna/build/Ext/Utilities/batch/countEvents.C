void countEvents(TString fname)
{
  gErrorIgnoreLevel=kError;  
  TFile file(fname);
  std::cout<<Events->GetEntries()<<std::endl;
  return;
}
