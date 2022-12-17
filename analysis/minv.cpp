#include "main.hh"

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
    TH1D *h1_alpha_alpha_invariant_mass;
    void init();
    void fill(const particle &p1, const particle &p2);
    void write(TFile *&outf);
};

// folders = ['40Ca', '40Ca_One', '48Ca', '48Ca_40t_One', '48Ca_One']
struct manager
{
    std::string input_dir, outfile_name;
    std::vector<std::string> folders;
    TChain *chain;
    histograms result;
    void init();
    void read();
    void finish();
};

int main(int argc, char **argv)
{
    std::vector<std::string> folders;
    std::string input_dir = argv[1];
    std::string outfile_name = argv[2];
    if (argc <= 3)
    {
        std::cerr << "not enough arguments" << std::endl;
        return 99;
    }

    std::cout << "input directory : " << input_dir << std::endl;
    std::cout << "output file : " << outfile_name << std::endl;
    for (int i = 3; i < argc; i++)
    {
        folders.push_back(argv[i]);
        std::cout << argv[i] << std::endl;
    }

    manager master{input_dir, outfile_name, folders};
    master.init();
    master.read();
    master.finish();
    return 0;
}

void manager::read()
{
    int nentries = this->chain->GetEntries();
    std::cout << "total number of events" << nentries << std::endl;
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
            for (int j = i + 1; j < structure.HiRA_NParticles; j++)
            {
                particle par1, par2;
                par1.init(i);
                par2.init(j);
                this->result.fill(par1, par2);
            }
        }
    }
}

void manager::init()
{
    this->chain = new TChain("HiRAEvent");
    files_manager fm = {this->input_dir, this->folders};
    fm.init(chain);
    this->result.init();
}

void manager::finish()
{
    TFile *outf = new TFile(this->outfile_name.c_str(), "RECREATE");
    this->result.write(outf);
    outf->Write();
    outf->Close();
}

void histograms::init()
{
    this->h1_dalpha_invaraiant_mass = new TH1D("h1_dalpha_invaraiant_mass", "", 300, -5., 25.);
    this->h1_dalpha_invaraiant_mass->Sumw2();
    this->h1_dalpha_invaraiant_mass->SetDirectory(0);

    this->h1_palpha_invaraiant_mass = new TH1D("h1_palpha_invaraiant_mass", "", 300, -5., 25.);
    this->h1_palpha_invaraiant_mass->Sumw2();
    this->h1_palpha_invaraiant_mass->SetDirectory(0);

    this->h1_alpha_alpha_invariant_mass = new TH1D("h1_alpha_alpha_invariant_mass", "", 300, -5., 25.);
    this->h1_alpha_alpha_invariant_mass->Sumw2();
    this->h1_alpha_alpha_invariant_mass->SetDirectory(0);
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

    if ((p1.z == 1 && p1.a == 2 && p2.z == 2 && p2.a == 4) ||
        (p2.z == 1 && p2.a == 2 && p1.z == 2 && p1.a == 4))
    {
        minv = sqrt(minv) - 5603.05149494286;
        this->h1_dalpha_invaraiant_mass->Fill(minv);
    }

    if ((p1.z == 1 && p1.a == 1 && p2.z == 2 && p2.a == 4) ||
        (p2.z == 1 && p2.a == 1 && p1.z == 2 && p1.a == 4))
    {
        minv = sqrt(minv) - 4669.149399085722;
        this->h1_palpha_invaraiant_mass->Fill(minv);
    }
    if ((p1.z == 2 && p1.a == 4 && p2.z == 2 && p2.a == 4) ||
        (p2.z == 2 && p2.a == 4 && p1.z == 2 && p1.a == 4))
    {
        minv = sqrt(minv) - 7456.894491337155;
        this->h1_alpha_alpha_invariant_mass->Fill(minv);
    }
}
void histograms::write(TFile *&outf)
{
    outf->cd();
    this->h1_palpha_invaraiant_mass->Write();
    this->h1_dalpha_invaraiant_mass->Write();
    this->h1_alpha_alpha_invariant_mass->Write();
}