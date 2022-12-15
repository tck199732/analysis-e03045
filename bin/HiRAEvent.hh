#ifndef HiRAEvent_hh
#define HiRAEvent_hh

#include "TObject.h"
#include "TClonesArray.h"
#include "HiRAParticle.hh"

class HiRAEvent : public TObject
{
protected:
    static const int MAX_MULTI = 40;

public:
    HiRAEvent();
    ~HiRAEvent() {}

    unsigned short GetRunNumber() { return mRunNumber; }
    void SetRunNumber(const unsigned short &number) { mRunNumber = number; }

    unsigned short GetEventNumber() { return mEventNumber; }
    void SetEventNumber(const unsigned short &number) { mEventNumber = number; }

    unsigned short Get4PiMultiplicityInBall() { return m4PiMultInBall; }
    void Set4PiMultiplicityInBall(const unsigned short &mult) { m4PiMultInBall = mult; }

    unsigned short Get4PiMultiplicityInFA() { return m4PiMultInFA; }
    void Set4PiMultiplicityInFA(const unsigned short &mult) { m4PiMultInFA = mult; }

    unsigned short Get4PiMultiplicity() { return m4PiMult; }
    void Set4PiMultiplicity(const unsigned short &mult) { m4PiMult = mult; }

    unsigned short Get4PiMultiplicityInHiRA() { return m4PiMultInHiRA; }
    void Set4PiMultiplicityInHiRA(const unsigned short &mult) { m4PiMultInHiRA = mult; }

    float GetEtrans() { return mEtrans; }
    void SetEtrans(const float &etrans) { mEtrans = etrans; }

    void Clear(const Option_t *option);

    HiRAParticle *AddParticle();

    TClonesArray *Particles() { return fParticles; }

private:
    unsigned short mRunNumber;   // run number
    unsigned short mEventNumber; // event number

    unsigned short m4PiMultInBall; // 4 pi multiplicity in ball
    unsigned short m4PiMultInFA;   // 4 pi multiplicity in FA
    unsigned short m4PiMult;       // 4 pi multiplicity w/o HiRA
    unsigned short m4PiMultInHiRA; // 4 pi multiplicity in HiRA

    float mEtrans;              // Etrans as measured by 4pi
    TClonesArray *fParticles;   // Array of Particle objects
    unsigned short mNParticles; // Number of Particle's in event

    ClassDef(HiRAEvent, 1)
};

#endif
