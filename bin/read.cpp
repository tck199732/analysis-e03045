#include "HiRAEvent.hh"
#include "HiRAParticle.hh"

#include <filesystem>
namespace fs = std::filesystem;
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <array>
#include <exception>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TClonesArray.h"

struct DataStructure
{
    static constexpr int max_multi = 128;

    float mEtrans;
    unsigned short mRunNumber;
    unsigned short mEventNumber;
    unsigned short m4PiMultInBall;
    unsigned short m4PiMultInFA;
    unsigned short m4PiMult;
    unsigned short m4PiMultInHiRA;

    unsigned short HiRA_NParticles;
    std::array<unsigned short, max_multi> HiRA_Z;
    std::array<unsigned short, max_multi> HiRA_A;
    std::array<unsigned short, max_multi> HiRA_Charge;
    std::array<unsigned short, max_multi> HiRA_Telescope;
    std::array<unsigned short, max_multi> HiRA_CsI;
    std::array<unsigned short, max_multi> HiRA_EFStrip;
    std::array<unsigned short, max_multi> HiRA_EBStrip;
    std::array<float, max_multi> HiRA_Px;
    std::array<float, max_multi> HiRA_Py;
    std::array<float, max_multi> HiRA_Pz;
    std::array<float, max_multi> HiRA_PixelVectorX;
    std::array<float, max_multi> HiRA_PixelVectorY;
    std::array<float, max_multi> HiRA_PixelVectorZ;
    std::array<float, max_multi> HiRA_EnLab;
    std::array<float, max_multi> HiRA_EnLabSi;
    std::array<float, max_multi> HiRA_EnLabLiSE;
    std::array<float, max_multi> HiRA_EnLabCsI;
    std::array<float, max_multi> HiRA_EnLabPLCalib;
    std::array<float, max_multi> HiRA_EnInSi;
};

DataStructure event;
HiRAEvent *hira_event; // = new HiRAEvent();

TTree *get_tree(const std::string &tr_name = "HiRAEvent")
{
    TTree *tr = new TTree(tr_name.c_str(), "");
    // HiRAEvent
    tr->Branch("mEtrans", &event.mEtrans, "mEtrans/F");
    tr->Branch("mRunNumber", &event.mRunNumber, "mRunNumber/S");
    tr->Branch("mEventNumber", &event.mEventNumber, "mEventNumber/S");
    tr->Branch("m4PiMultInBall", &event.m4PiMultInBall, "m4PiMultInBall/S");
    tr->Branch("m4PiMultInFA", &event.m4PiMultInFA, "m4PiMultInFA/S");
    tr->Branch("m4PiMult", &event.m4PiMult, "m4PiMult/S");
    tr->Branch("m4PiMultInHiRA", &event.m4PiMultInHiRA, "m4PiMultInHiRA/S");

    // HiRAParticle
    tr->Branch("HiRA_NParticles", &event.HiRA_NParticles, "HiRA_NParticles/S");

    tr->Branch("HiRA_A", &event.HiRA_A[0], "HiRA_A[HiRA_NParticles]/S");
    tr->Branch("HiRA_Z", &event.HiRA_Z[0], "HiRA_Z[HiRA_NParticles]/S");
    tr->Branch("HiRA_Charge", &event.HiRA_Charge[0], "HiRA_Charge[HiRA_NParticles]/S");
    tr->Branch("HiRA_Telescope", event.HiRA_Telescope[0], "HiRA_Telescope[HiRA_NParticles]/S");
    tr->Branch("HiRA_CsI", &event.HiRA_CsI[0], "HiRA_CsI[HiRA_NParticles]/S");
    tr->Branch("HiRA_EFStrip", &event.HiRA_EFStrip[0], "HiRA_EFStrip[HiRA_NParticles]/S");
    tr->Branch("HiRA_EBStrip", &event.HiRA_EBStrip[0], "HiRA_EBStrip[HiRA_NParticles]/S");

    tr->Branch("HiRA_Px", &event.HiRA_Px[0], "HiRA_Px[HiRA_NParticles]/F");
    tr->Branch("HiRA_Py", &event.HiRA_Py[0], "HiRA_Py[HiRA_NParticles]/F");
    tr->Branch("HiRA_Pz", &event.HiRA_Pz[0], "HiRA_Pz[HiRA_NParticles]/F");

    tr->Branch("HiRA_PixelVectorX", &event.HiRA_PixelVectorX[0], "HiRA_PixelVectorX[HiRA_NParticles]/F");
    tr->Branch("HiRA_PixelVectorY", &event.HiRA_PixelVectorY[0], "HiRA_PixelVectorY[HiRA_NParticles]/F");
    tr->Branch("HiRA_PixelVectorZ", &event.HiRA_PixelVectorZ[0], "HiRA_PixelVectorZ[HiRA_NParticles]/F");

    tr->Branch("HiRA_EnLab", &event.HiRA_EnLab[0], "HiRA_EnLab[HiRA_NParticles]/F");
    tr->Branch("HiRA_EnLabSi", &event.HiRA_EnLabSi[0], "HiRA_EnLabSi[HiRA_NParticles]/F");
    tr->Branch("HiRA_EnLabLiSE", &event.HiRA_EnLabLiSE[0], "HiRA_EnLabLiSE[HiRA_NParticles]/F");
    tr->Branch("HiRA_EnLabCsI", &event.HiRA_EnLabCsI[0], "HiRA_EnLabCsI[HiRA_NParticles]/F");
    tr->Branch("HiRA_EnLabPLCalib", &event.HiRA_EnLabPLCalib[0], "HiRA_EnLabPLCalib[HiRA_NParticles]/F");
    tr->Branch("HiRA_EnInSi", &event.HiRA_EnInSi[0], "HiRA_EnInSi[HiRA_NParticles]/F");

    return tr;
}

void fill_struct(HiRAEvent *hira_event)
{
    event.mRunNumber = hira_event->GetRunNumber();
    event.mEventNumber = hira_event->GetEventNumber();
    event.m4PiMultInBall = hira_event->Get4PiMultiplicityInBall();
    event.m4PiMultInFA = hira_event->Get4PiMultiplicityInFA();
    event.m4PiMult = hira_event->Get4PiMultiplicity();
    event.m4PiMultInHiRA = hira_event->Get4PiMultiplicityInHiRA();
    event.mEtrans = hira_event->GetEtrans();

    TClonesArray *particles = hira_event->Particles();
    event.HiRA_NParticles = particles->GetEntries();

    for (int n = 0; n < event.HiRA_NParticles; n++)
    {
        HiRAParticle *track = (HiRAParticle *)particles->UncheckedAt(n);
        event.HiRA_Px[n] = track->GetPx();
        event.HiRA_Py[n] = track->GetPy();
        event.HiRA_Pz[n] = track->GetPz();
        event.HiRA_PixelVectorX[n] = track->GetPixelVectorX();
        event.HiRA_PixelVectorY[n] = track->GetPixelVectorY();
        event.HiRA_PixelVectorZ[n] = track->GetPixelVectorZ();

        event.HiRA_EnLab[n] = track->GetEnLab();
        event.HiRA_EnLabSi[n] = track->GetEnLabSi();
        event.HiRA_EnLabLiSE[n] = track->GetEnLabLiSE();
        event.HiRA_EnLabCsI[n] = track->GetEnLabCsI();
        event.HiRA_EnLabPLCalib[n] = track->GetEnLabPLCalib();
        event.HiRA_EnInSi[n] = track->GetEnInSi();

        event.HiRA_Telescope[n] = track->GetTelescope();
        event.HiRA_CsI[n] = track->GetCsI();
        event.HiRA_EFStrip[n] = track->GetEFStrip();
        event.HiRA_EBStrip[n] = track->GetEBStrip();

        event.HiRA_Charge[n] = track->GetCharge();
        switch (track->GetPid())
        {
        case 2212:
        {
            event.HiRA_A[n] = 1;
            event.HiRA_Z[n] = 1;
            break;
        }
        case 1:
        {
            event.HiRA_A[n] = 2;
            event.HiRA_Z[n] = 1;
            break;
        }
        case 2:
        {
            event.HiRA_A[n] = 3;
            event.HiRA_Z[n] = 1;
            break;
        }
        case 3:
        {
            event.HiRA_A[n] = 3;
            event.HiRA_Z[n] = 2;
            break;
        }
        case 4:
        {
            event.HiRA_A[n] = 4;
            event.HiRA_Z[n] = 2;
            break;
        }
        default:
        {
            event.HiRA_A[n] = 0;
            event.HiRA_Z[n] = 0;
            break;
        }
        }
    }
    return;
}

TChain *input(const std::string &fname, const std::string &tr_name = "HiRAEvent")
{
    TChain *chain = new TChain(tr_name.c_str());
    chain->Add(fname.c_str());
    chain->SetBranchAddress("HiRAEvent", &hira_event);
    return chain;
}

void read(const std::string &filename = "/data/chajecki/EXP_DATA/40Ca/40Ca_05.root", const std::string &outfile_name = "./test2.root")
{
    TChain *chain = input(filename);
    int nentries = chain->GetEntries();
    TFile *outfile = new TFile(outfile_name.c_str(), "RECREATE");
    TTree *tree = get_tree();

    int status = 0;

    for (int ievt = 0; ievt < nentries; ievt++)
    {
        try
        {
            int bytes = chain->GetEntry(ievt);
        }
        catch (exception &e)
        {
            std::cerr << e.what() << std::endl;
            status = 1;
            break;
        }
        fill_struct(hira_event);
        tree->Fill();
    }
    if (status == 1)
    {
        return;
    }

    outfile->cd();
    tree->Write();
    outfile->Write();
}