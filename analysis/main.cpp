#include "main.hh"

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
    TH2D *h2_multi_etrans;
    std::map<std::string, TH2D *> h2_pt_rapidity;   // pt per nucleon VS rapidity / beam rapidity (lab)
    std::map<std::string, TH2D *> h2_kinergy_theta; // ke per nucleon VS theta (cms)

    std::vector<std::string> particle_names = {"p", "d", "t", "3He", "4He"};
    void init();
    void normalize();
    void fill(const event &ev);
    void fill(const particle &par);
    void write(TFile *&outf);
};

// folders = ['40Ca', '40Ca_One', '48Ca', '48Ca_40t_One', '48Ca_One']
struct manager
{
    std::string beam, target;
    int beamE;
    std::string input_dir, outfile_name;
    std::vector<std::string> folders;

    Reaction reaction;
    double betacms, beam_rapidity;

    TChain *chain;
    histograms result;
    void init();
    void read();
    void finish();
};

int main(int argc, char **argv)
{
    std::string beam = argv[1];
    std::string target = argv[2];
    int beamE = std::stoi(argv[3]);
    std::vector<std::string> folders;
    std::string input_dir = argv[4];
    std::string outfile_name = argv[5];
    if (argc <= 5)
    {
        std::cerr << "not enough arguments" << std::endl;
        return 99;
    }

    std::cout << "input directory : " << input_dir << std::endl;
    std::cout << "output file : " << outfile_name << std::endl;
    for (int i = 6; i < argc; i++)
    {
        folders.push_back(argv[i]);
        std::cout << argv[i] << std::endl;
    }

    manager master{beam, target, beamE, input_dir, outfile_name, folders};
    master.init();
    master.read();
    master.finish();
    return 0;
}

void manager::read()
{
    int nentries = this->chain->GetEntries();
    std::cout << "total number of events = " << nentries << std::endl;
    for (int n = 0; n < nentries; n++)
    {
        if (n % 100000 == 0)
        {
            std::cout << "events processed : " << n << std::endl;
        }

        this->chain->GetEntry(n);
        event ev{
            structure.mRunNumber,
            structure.mEventNumber,
            structure.m4PiMultInBall,
            structure.m4PiMultInFA,
            structure.m4PiMult,
            structure.m4PiMultInHiRA,
            structure.mEtrans,
        };

        // if eventcut, continue;
        for (int i = 0; i < structure.HiRA_NParticles; i++)
        {
            particle par;
            par.init(i, this->betacms, this->beam_rapidity);
            ev.particles.push_back(par);
        }
        this->result.fill(ev);
    }
}

void manager::init()
{
    this->reaction = {this->beam, this->target, this->beamE};
    this->reaction.init();
    this->betacms = this->reaction.betacms;
    this->beam_rapidity = this->reaction.beam_rapidity;
    std::cout << this->betacms << "\t" << this->beam_rapidity << std::endl;

    this->chain = new TChain("HiRAEvent");
    files_manager fm = {this->input_dir, this->folders};
    fm.init(chain);
    this->result.init();
}

void manager::finish()
{
    TFile *outf = new TFile(this->outfile_name.c_str(), "RECREATE");
    this->result.normalize();
    this->result.write(outf);
    outf->Write();
    outf->Close();
}

void histograms::init()
{
    this->h2_multi_etrans = new TH2D("h2_multi_etrans", "", 40, 0, 40, 600, 0, 600);
    this->h2_multi_etrans->Sumw2();
    this->h2_multi_etrans->SetDirectory(0);
    for (auto &pn : this->particle_names)
    {
        this->h2_pt_rapidity[pn] = new TH2D(("h2_pt_rapidity_" + pn).c_str(), "", 100, 0., 1., 800, 0., 800);
        this->h2_kinergy_theta[pn] = new TH2D(("h2_kinergy_theta_" + pn).c_str(), "", 200, 0., 200, 800, 10, 70);
        this->h2_pt_rapidity[pn]->Sumw2();
        this->h2_pt_rapidity[pn]->SetDirectory(0);
        this->h2_kinergy_theta[pn]->Sumw2();
        this->h2_kinergy_theta[pn]->SetDirectory(0);
    }
}

void histograms::fill(const event &ev)
{
    this->h2_multi_etrans->Fill(ev.m4PiMult, ev.mEtrans);
    for (auto &par : ev.particles)
    {
        this->fill(par);
    }
}
void histograms::fill(const particle &par)
{
    std::string pn = par.name;
    this->h2_pt_rapidity[pn]->Fill(par.rapidity_normed, par.pt / par.a);
    this->h2_kinergy_theta[pn]->Fill(par.encms / par.a, par.thetacms);
}

void histograms::normalize()
{
    double norm = this->h2_multi_etrans->GetEntries();

    this->h2_multi_etrans->Scale(1. / norm);
    for (auto &pn : this->particle_names)
    {
        this->h2_kinergy_theta[pn]->Scale(1. / norm);
        this->h2_pt_rapidity[pn]->Scale(1. / norm);
    }
}

void histograms::write(TFile *&outf)
{
    outf->cd();

    this->h2_multi_etrans->Write();
    for (auto &pn : this->particle_names)
    {
        this->h2_kinergy_theta[pn]->Write();
        this->h2_pt_rapidity[pn]->Write();
    }
}