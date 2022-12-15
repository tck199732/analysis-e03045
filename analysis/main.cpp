#include "main.hh"

// folders = ['40Ca', '40Ca_One', '48Ca', '48Ca_40t_One', '48Ca_One']
struct manager
{
    std::string input_dir, outfile_name;
    std::vector<std::string> folders;
    std::map<std::string, std::vector<std::string>> files;
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
    for (auto &folder : this->folders)
    {
        fs::path folder_path = this->input_dir + folder;
        for (const auto &f : fs::directory_iterator{folder_path})
        {
            this->files[folder].push_back(f.path());
            fs::perms file_perms = fs::status(f.path()).permissions();

            if ((file_perms & fs::perms::owner_read) != fs::perms::none)
            {
                this->chain->Add(f.path().c_str());
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

    this->result.init();
}

void manager::finish()
{
    TFile *outf = new TFile(this->outfile_name.c_str(), "RECREATE");
    this->result.write(outf);
    outf->Write();
    outf->Close();
}