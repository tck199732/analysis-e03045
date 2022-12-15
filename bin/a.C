void a()
{
    TChain *chain = new TChain("HiRAEvent");
    chain->Add("/data/chajecki/EXP_DATA/40Ca/40Ca_02.root");

    HiRAEvent *ev = new HiRAEvent();
    chain->SetBranchAddress("HiRAEvent", &ev);

    int nentries = chain->GetEntries();
    for (int i = 0; i < nentries; i++)
    {
        chain->GetEntry(i);
        TClonesArray *tracks = ev->Particles();

        int ntracks = tracks->GetEntries();
        for (int n = 0; n < ntracks; n++)
        {
            HiRAParticle *track = (HiRAParticle *)tracks->UncheckedAt(n);
        }
    }
}