
#include "HiRAParticle.hh"
ClassImp(HiRAParticle);
HiRAParticle::HiRAParticle()
{
    fPx = 0.;
    fPy = 0.;
    fPz = 0.;
    fEnLab = 0.;
    fEnLabSi = 0.;
    fEnLabLiSE = 0.;
    fEnLabCsI = 0.;
    fEnLabPLCalib = 0.;
    fEnInSi = 0.;

    mCharge = 0;
    mPid = 0;
    mPixelVectorX = 0.;
    mPixelVectorY = 0.;
    mPixelVectorZ = 0.;
    mTelescope = 0;
    mEFStrip = 0;
    mEBStrip = 0;
    mCsI = 0;
}
