/// \file  SiPMHit.cc
/// \brief Implementation of the SiPMHit class

#include "SiPMHit.hh"
#include "G4ios.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

G4ThreadLocal G4Allocator<SiPMHit>* SiPMHitAllocator = nullptr;

SiPMHit::SiPMHit() : 
	fPhysVol(nullptr){}

SiPMHit::SiPMHit(G4VPhysicalVolume* pVol) : fPhysVol(pVol){}

SiPMHit::~SiPMHit(){}

SiPMHit::SiPMHit(const SiPMHit &hit) : G4VHit(){	
	fEvent = hit.fEvent;
	fSiPMNo = hit.fSiPMNo;
	fParticleID = hit.fParticleID;

	fEin  = hit.fEin;
	fThetaIn = hit.fThetaIn;
	fPosInX = hit.fPosInX;
	fPosInY = hit.fPosInY;
	fPosInZ = hit.fPosInZ;
	fPosSiPMInX = hit.fPosSiPMInX;
	fPosSiPMInY = hit.fPosSiPMInY;
	fPosSiPMInZ = hit.fPosSiPMInZ;
	fMomInX = hit.fMomInX;
	fMomInY = hit.fMomInY;
	fMomInZ = hit.fMomInZ;
	fTimeIn = hit.fTimeIn;
}

const SiPMHit& SiPMHit::operator=(const SiPMHit &hit){
	fEvent = hit.fEvent;
	fSiPMNo = hit.fSiPMNo;
	fParticleID = hit.fParticleID;
	
	fEin  = hit.fEin;
	fThetaIn = hit.fThetaIn;
	fPosInX = hit.fPosInX;
	fPosInY = hit.fPosInY;
	fPosInZ = hit.fPosInZ;
	fPosSiPMInX = hit.fPosSiPMInX;
	fPosSiPMInY = hit.fPosSiPMInY;
	fPosSiPMInZ = hit.fPosSiPMInZ;
	fMomInX = hit.fMomInX;
	fMomInY = hit.fMomInY;
	fMomInZ = hit.fMomInZ;
	fTimeIn = hit.fTimeIn;

	return* this;
}

G4bool SiPMHit::operator==(const SiPMHit &) const{
	return false;
}

void SiPMHit::Draw(){}

void SiPMHit::Print(){}


