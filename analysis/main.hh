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
#include "TH2D.h"

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
    // data from root file
    int z, a, charge;
    short tele, csi, ef, eb;
    float px, py, pz;
    float pvx, pvy, pvz;
    float enlab, enlabsi, enlablise, enlabcsi, enlabplcalib, eninsi;

    // additional calculations
    float mass;
    float pt, plab, theta, phi, rapidity, rapidity_normed, rapidity_cms, pzcms, encms, pcms, thetacms;
    std::string name;
    void init(const int &i, const double &betacms = 0.2, const double &beam_rapdiity = 0.5);
};

struct event
{
    short mRunNumber, mEventNumber, m4PiMultInBall, m4PiMultInFA, m4PiMult, m4PiMultInHiRA;
    float mEtrans;
    std::vector<particle> particles;
};

void particle::init(const int &i, const double &betacms, const double &beam_rapidity)
{
    // from data in root file
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

    // additional calculations
    this->mass = (pow(this->px, 2.) + pow(this->py, 2.) + pow(this->pz, 2.) - pow(this->enlab, 2.)) / 2. / this->enlab;

    this->pt = TMath::Sqrt(pow(this->px, 2.) + pow(this->py, 2.));
    this->plab = TMath::Sqrt(pow(this->pt, 2.) + pow(this->pz, 2.));

    this->theta = TMath::ATan2(this->pt, this->pz) * TMath::RadToDeg();
    this->phi = TMath::ATan2(this->py, this->px) * TMath::RadToDeg();
    if (this->phi < 0)
    {
        this->phi += 360.;
    }

    this->rapidity = 0.5 * TMath::Log((this->enlab + this->mass + this->pz) / (this->enlab + this->mass - this->pz));
    this->rapidity_normed = this->rapidity / beam_rapidity;

    double gamma = 1. / TMath::Sqrt(1 - pow(betacms, 2.));

    this->pzcms = gamma * (this->pz - betacms * (this->enlab + this->mass));
    this->pcms = TMath::Sqrt(pow(this->pt, 2.) + pow(this->pzcms, 2.));
    this->encms = TMath::Sqrt(pow(this->pcms, 2.) + pow(this->mass, 2.)) - this->mass;
    this->rapidity_cms = 0.5 * TMath::Log((this->encms + this->mass + this->pzcms) / (this->encms + this->mass - this->pzcms));
    this->thetacms = TMath::ATan2(this->pt, this->pzcms) * TMath::RadToDeg();

    this->name = "None";
    if (this->z == 1 && this->a == 1)
    {
        this->name = "p";
    }
    else if (this->z == 1 && this->a == 2)
    {
        this->name = "d";
    }
    else if (this->z == 1 && this->a == 3)
    {
        this->name = "t";
    }
    else if (this->z == 2 && this->a == 3)
    {
        this->name = "3He";
    }
    else if (this->z == 2 && this->a == 4)
    {
        this->name = "4He";
    }
}

struct Reaction
{
    std::string beam, target;
    int beamE;

    std::vector<std::string> nuclei_names = {"Ca40", "Ca48", "Ni58", "Ni64", "Sn112", "Sn124"};
    std::vector<double> nuclei_mass = {
        37224.91769468577,
        44667.49204802292,
        53966.42906919436,
        59548.52352069724,
        104238.68442172016,
        115417.03722472588,
    };

    float betacms, beam_rapidity;
    void init();
};

void Reaction::init()
{
    int bid = std::find(nuclei_names.begin(), nuclei_names.end(), this->beam) - nuclei_names.begin();
    int tid = std::find(nuclei_names.begin(), nuclei_names.end(), this->target) - nuclei_names.begin();
    if (bid == static_cast<int>(nuclei_names.size()) || tid == static_cast<int>(nuclei_names.size()))
    {
        throw std::invalid_argument("symbol is not valud.");
    }

    double m1 = this->nuclei_mass[bid];
    double m2 = this->nuclei_mass[tid];
    int beamA = std::stoi(this->beam.substr(2, this->beam.size() - 2));
    double beam_ke = this->beamE * 1.0 * beamA;
    double beam_energy_tot = beam_ke + m1;
    double mom_beam = TMath::Sqrt(pow(beam_ke, 2.) + 2. * beam_ke * m1);
    double gamma = beam_energy_tot / m1;
    this->betacms = mom_beam / (gamma * m1 + m2);
    this->beam_rapidity = 0.5 * TMath::Log((beam_energy_tot + mom_beam) / (beam_energy_tot - mom_beam));
}

struct files_manager
{

    std::string input_dir;
    std::vector<std::string> folders;
    std::map<std::string, std::vector<std::string>> files;
    void init(TChain *chain);
};

void files_manager::init(TChain *chain)
{
    for (auto &folder : this->folders)
    {
        fs::path folder_path = this->input_dir + "/" + folder;
        for (const auto &f : fs::directory_iterator{folder_path})
        {
            this->files[folder].push_back(f.path());
            fs::perms file_perms = fs::status(f.path()).permissions();

            if ((file_perms & fs::perms::owner_read) != fs::perms::none)
            {
                chain->Add(f.path().c_str());
            }
        }
    }

    chain->SetBranchAddress("mEtrans", &structure.mEtrans);
    chain->SetBranchAddress("mRunNumber", &structure.mRunNumber);
    chain->SetBranchAddress("mEventNumber", &structure.mEventNumber);
    chain->SetBranchAddress("m4PiMultInBall", &structure.m4PiMultInBall);
    chain->SetBranchAddress("m4PiMultInFA", &structure.m4PiMultInFA);
    chain->SetBranchAddress("m4PiMult", &structure.m4PiMult);
    chain->SetBranchAddress("m4PiMultInHiRA", &structure.m4PiMultInHiRA);
    chain->SetBranchAddress("HiRA_NParticles", &structure.HiRA_NParticles);

    chain->SetBranchAddress("HiRA_Z", &structure.HiRA_Z[0]);
    chain->SetBranchAddress("HiRA_A", &structure.HiRA_A[0]);
    chain->SetBranchAddress("HiRA_Charge", &structure.HiRA_Charge[0]);
    chain->SetBranchAddress("HiRA_Telescope", &structure.HiRA_Telescope[0]);
    chain->SetBranchAddress("HiRA_CsI", &structure.HiRA_CsI[0]);
    chain->SetBranchAddress("HiRA_EFStrip", &structure.HiRA_EFStrip[0]);
    chain->SetBranchAddress("HiRA_EBStrip", &structure.HiRA_EBStrip[0]);

    chain->SetBranchAddress("HiRA_Px", &structure.HiRA_Px[0]);
    chain->SetBranchAddress("HiRA_Py", &structure.HiRA_Py[0]);
    chain->SetBranchAddress("HiRA_Pz", &structure.HiRA_Pz[0]);
    chain->SetBranchAddress("HiRA_PixelVectorX", &structure.HiRA_PixelVectorX[0]);
    chain->SetBranchAddress("HiRA_PixelVectorY", &structure.HiRA_PixelVectorY[0]);
    chain->SetBranchAddress("HiRA_PixelVectorZ", &structure.HiRA_PixelVectorZ[0]);

    chain->SetBranchAddress("HiRA_EnLab", &structure.HiRA_EnLab[0]);
    chain->SetBranchAddress("HiRA_EnLabSi", &structure.HiRA_EnLabSi[0]);
    chain->SetBranchAddress("HiRA_EnLabLiSE", &structure.HiRA_EnLabLiSE[0]);
    chain->SetBranchAddress("HiRA_EnLabCsI", &structure.HiRA_EnLabCsI[0]);
    chain->SetBranchAddress("HiRA_EnLabPLCalib", &structure.HiRA_EnLabPLCalib[0]);
    chain->SetBranchAddress("HiRA_EnInSi", &structure.HiRA_EnInSi[0]);

    chain->SetBranchStatus("mEtrans", true);
    chain->SetBranchStatus("mRunNumber", true);
    chain->SetBranchStatus("mEventNumber", true);
    chain->SetBranchStatus("m4PiMultInBall", true);
    chain->SetBranchStatus("m4PiMultInFA", true);
    chain->SetBranchStatus("m4PiMult", true);
    chain->SetBranchStatus("m4PiMultInHiRA", true);
    chain->SetBranchStatus("HiRA_NParticles", true);

    chain->SetBranchStatus("HiRA_Z", true);
    chain->SetBranchStatus("HiRA_A", true);
    chain->SetBranchStatus("HiRA_Charge", true);
    chain->SetBranchStatus("HiRA_Telescope", true);
    chain->SetBranchStatus("HiRA_CsI", true);
    chain->SetBranchStatus("HiRA_EFStrip", true);
    chain->SetBranchStatus("HiRA_EBStrip", true);

    chain->SetBranchStatus("HiRA_Px", true);
    chain->SetBranchStatus("HiRA_Py", true);
    chain->SetBranchStatus("HiRA_Pz", true);
    chain->SetBranchStatus("HiRA_PixelVectorX", true);
    chain->SetBranchStatus("HiRA_PixelVectorY", true);
    chain->SetBranchStatus("HiRA_PixelVectorZ", true);

    chain->SetBranchStatus("HiRA_EnLab", true);
    chain->SetBranchStatus("HiRA_EnLabSi", true);
    chain->SetBranchStatus("HiRA_EnLabLiSE", true);
    chain->SetBranchStatus("HiRA_EnLabCsI", true);
    chain->SetBranchStatus("HiRA_EnLabPLCalib", true);
    chain->SetBranchStatus("HiRA_EnInSi", true);
};