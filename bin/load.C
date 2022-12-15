
void load()
{
    gROOT->ProcessLine(".L HiRAParticle.cpp+");
    gROOT->ProcessLine(".L HiRAEvent.cpp+");
}