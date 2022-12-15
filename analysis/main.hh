#include <filesystem>
namespace fs = std::filesystem;
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <vector>
#include <exception>

#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "TMath.h"
#include "TH1D.h"

struct DataStructure
{
    static constexpr int max_multi = 128;
    float mEtrans;
    short mRunNumber;
    short mEventNumber;
    short m4PiMultInBall;
    short m4PiMultInFA;
    short m4PiMult;
    short m4PiMultInHiRA;

    short HiRA_NParticles;
    std::array<short, max_multi> HiRA_Z;
    std::array<short, max_multi> HiRA_A;
    std::array<short, max_multi> HiRA_Charge;
    std::array<short, max_multi> HiRA_Telescope;
    std::array<short, max_multi> HiRA_CsI;
    std::array<short, max_multi> HiRA_EFStrip;
    std::array<short, max_multi> HiRA_EBStrip;
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

DataStructure structure;

struct particle
{
    int z, a, charge;
    short tele, csi, ef, eb;
    float px, py, pz;
    float pvx, pvy, pvz;
    float enlab, enlabsi, enlablise, enlabcsi, enlabplcalib, eninsi;
    float mass;
    void init(const int &i);
};

void particle::init(const int &i)
{
    this->z = structure.HiRA_Z[i];
    this->a = structure.HiRA_A[i];
    this->charge = structure.HiRA_Charge[i];
    this->tele = structure.HiRA_Telescope[i];
    this->csi = structure.HiRA_CsI[i];
    this->ef = structure.HiRA_EFStrip[i];
    this->eb = structure.HiRA_EBStrip[i];
    this->px = structure.HiRA_Px[i];
    this->py = structure.HiRA_Py[i];
    this->pz = structure.HiRA_Pz[i];
    this->pvx = structure.HiRA_PixelVectorX[i];
    this->pvy = structure.HiRA_PixelVectorY[i];
    this->pvz = structure.HiRA_PixelVectorZ[i];
    this->enlab = structure.HiRA_EnLab[i];
    this->enlabsi = structure.HiRA_EnLabSi[i];
    this->enlablise = structure.HiRA_EnLabLiSE[i];
    this->enlabsi = structure.HiRA_EnLabCsI[i];
    this->enlabplcalib = structure.HiRA_EnLabPLCalib[i];
    this->eninsi = structure.HiRA_EnInSi[i];

    this->mass = (pow(this->px, 2.) + pow(this->py, 2.) + pow(this->pz, 2.) - pow(this->enlab, 2.)) / 2. / this->enlab;
}
struct event
{
    unsigned short mRunNumber, mEventNumber, m4PiMultInBall, m4PiMultInFA, m4PiMult, m4PiMultInHiRA;
    float mEtrans;
    std::vector<particle> particles;
};

struct eventcut
{
    std::array<float, 2> cut_etrans;
    std::array<int, 2> m4PiMultInBall = {1, 100};
    std::array<int, 2> m4PiMultInFA = {1, 100};
    std::array<int, 2> m4PiMult = {1, 100};
    std::array<int, 2> m4PiMultInHiRA = {1, 100};
};

struct histograms
{
    TH1D *h1_dalpha_invaraiant_mass;
    TH1D *h1_palpha_invaraiant_mass;
    TH1D *h1_alphaalpha_invariant_mass;
    void init();
    void fill(const particle &p1, const particle &p2);
    void write(TFile *&outf);
};

void histograms::init()
{
    this->h1_dalpha_invaraiant_mass = new TH1D("h1_dalpha_invaraiant_mass", "", 300, -5., 25.);
    this->h1_dalpha_invaraiant_mass->Sumw2();
    this->h1_dalpha_invaraiant_mass->SetDirectory(0);

    this->h1_palpha_invaraiant_mass = new TH1D("h1_palpha_invaraiant_mass", "", 300, -5., 25.);
    this->h1_palpha_invaraiant_mass->Sumw2();
    this->h1_palpha_invaraiant_mass->SetDirectory(0);

    this->h1_alphaalpha_invaraiant_mass = new TH1D("h1_alphaalpha_invaraiant_mass", "", 300, -5., 25.);
    this->h1_alphaalpha_invaraiant_mass->Sumw2();
    this->h1_alphaalpha_invaraiant_mass->SetDirectory(0);
}

void histograms::fill(const particle &p1, const particle &p2)
{
    double px1 = p1.px;
    double px2 = p2.px;
    double py1 = p1.py;
    double py2 = p2.py;
    double pz1 = p1.pz;
    double pz2 = p2.pz;

    double m1 = p1.mass;
    double m2 = p2.mass;
    double e1 = m1 + p1.enlab;
    double e2 = m2 + p2.enlab;

    double p1p2 = px1 * px2 + py1 * py2 + pz1 * pz2;
    double minv = pow(m1, 2.) + pow(m2, 2.) + 2 * (e1 * e2 - p1p2);

    if ((par1.z == 1 && par1.a == 2 && par2.z == 2 && par2.a == 4) ||
        (par2.z == 1 && par2.a == 2 && par1.z == 2 && par1.a == 4))
    {
        minv = sqrt(minv) - 5603.05149494286;
        this->h1_dalpha_invaraiant_mass->Fill(minv);
    }

    if ((par1.z == 1 && par1.a == 1 && par2.z == 2 && par2.a == 4) ||
        (par2.z == 1 && par2.a == 1 && par1.z == 2 && par1.a == 4))
    {
        minv = sqrt(minv) - 4669.149399085722;
        this->h1_palpha_invaraiant_mass->Fill(minv);
    }
    if ((par1.z == 2 && par1.a == 4 && par2.z == 2 && par2.a == 4) ||
        (par2.z == 2 && par2.a == 4 && par1.z == 2 && par1.a == 4))
    {
        minv = sqrt(minv) - 7456.894491337155;
        this->h1_alphaalpha_invaraiant_mass->Fill(minv);
    }
}
void histograms::write(TFile *&outf)
{
    outf->cd();
    this->h1_palpha_invaraiant_mass->Write();
    this->h1_dalpha_invaraiant_mass->Write();
    this->h1_alphaalpha_invaraiant_mass->Write();
}
