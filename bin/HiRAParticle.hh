#include "TObject.h"
#include "TClonesArray.h"
#include "TVector3.h"

#ifndef HiRAParticle_hh
#define HiRAParticle_hh

class HiRAParticle : public TObject
{
public:
    HiRAParticle();
    virtual ~HiRAParticle() { ; }

    unsigned short GetCharge() { return mCharge; }
    void SetCharge(const unsigned short &charge) { mCharge = charge; }

    void SetP(const float &Px, const float &Py, const float &Pz)
    {
        fPx = Px;
        fPy = Py;
        fPz = Pz;
    }
    float GetPx() { return fPx; }
    float GetPy() { return fPy; }
    float GetPz() { return fPz; }

    void SetPid(const unsigned short &id) { mPid = id; }
    unsigned short GetPid() { return mPid; }

    void SetPixelVector(const float &x, const float &y, const float &z)
    {
        mPixelVectorX = x;
        mPixelVectorY = y;
        mPixelVectorZ = z;
    }
    float GetPixelVectorX() { return mPixelVectorX; }
    float GetPixelVectorY() { return mPixelVectorY; }
    float GetPixelVectorZ() { return mPixelVectorZ; }

    void SetTelescope(const unsigned short &tele) { mTelescope = tele; }
    unsigned short GetTelescope() { return mTelescope; }

    void SetEFStrip(const unsigned short &strip) { mEFStrip = strip; }
    unsigned short GetEFStrip() { return mEFStrip; }

    void SetEBStrip(const unsigned short &strip) { mEBStrip = strip; }
    unsigned short GetEBStrip() { return mEBStrip; }

    void SetCsI(const unsigned short &csi) { mCsI = csi; }
    unsigned short GetCsI() { return mCsI; }

    void SetEnLabPLCalib(const float &en) { fEnLabPLCalib = en; }
    float GetEnLabPLCalib() { return fEnLabPLCalib; }

    void SetEnLab(const float &en) { fEnLab = en; }
    float GetEnLab() { return fEnLab; }

    void SetEnLabSi(const float &en) { fEnLabSi = en; }
    float GetEnLabSi() { return fEnLabSi; }

    void SetEnLabLiSE(const float &en) { fEnLabLiSE = en; }
    float GetEnLabLiSE() { return fEnLabLiSE; }

    void SetEnLabCsI(const float &en) { fEnLabCsI = en; }
    float GetEnLabCsI() { return fEnLabCsI; }

    void SetEnInSi(const float &en) { fEnInSi = en; }
    float GetEnInSi() { return fEnInSi; }

private:
    float fPx;           // x component of particle momentum
    float fPy;           // y component of particle momentum
    float fPz;           // z component of particle momentum
    float fEnLab;        // lab energy of the particle
    float fEnLabSi;      // lab energy of the particle with dE measured by Si
    float fEnLabLiSE;    // lab energy of the particle determined by LISE
    float fEnLabCsI;     // lab energy of the particle measured by CSI only
    float fEnLabPLCalib; // lab energy of the particle based on parton-like calibration
    float fEnInSi;       // energy deposited in Si

    unsigned short mCharge; // charge of the particle
    unsigned short mPid;    // particle id

    float mPixelVectorX;
    float mPixelVectorY;
    float mPixelVectorZ;
    unsigned short mTelescope;
    unsigned short mEFStrip;
    unsigned short mEBStrip;
    unsigned short mCsI;

    ClassDef(HiRAParticle, 1)
};

#endif