#include "HiRAEvent.hh"
#include "HiRAParticle.hh"
#include "TClonesArray.h"

ClassImp(HiRAEvent);
HiRAEvent::HiRAEvent()
{
    fParticles = new TClonesArray("HiRAParticle", MAX_MULTI);
    mNParticles = 0;
}

void HiRAEvent::Clear(const Option_t *option = "")
{
    fParticles->Clear("C");
    mNParticles = 0;
}

HiRAParticle *HiRAEvent::AddParticle()
{
    TClonesArray &particles = *fParticles;
    HiRAParticle *particle = new (particles[mNParticles++]) HiRAParticle;
    return particle;
}
