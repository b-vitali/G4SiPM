/// \file  SiPMHit.hh
/// \brief Definition of the SiPMHit class

#ifndef SiPMHit_h
#define SiPMHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"

#include "TVector3.h"

#include "tls.hh"

#include <vector>

class SiPMHit : public G4VHit{
	public:
		SiPMHit();
		SiPMHit(G4VPhysicalVolume* pVol);
		virtual ~SiPMHit();

		SiPMHit(const SiPMHit &right);
		const SiPMHit& operator=(const SiPMHit &right);
		G4bool operator==(const SiPMHit &right) const;

		inline void *operator new(size_t);
		inline void operator delete(void *aHit);

		virtual void Draw();
		virtual void Print();

		inline void SetEvent(std::vector<G4int> val){fEvent = val;}
		inline void SetSiPMNo(std::vector<G4int> val){fSiPMNo = val;}
		inline void SetParticleID(std::vector<G4int> val){fParticleID = val;}

		inline void SetEin (std::vector<G4double> val){fEin  = val;}
		inline void SetThetaIn(std::vector<G4double> val){fThetaIn = val;}

		inline std::vector<G4int> GetEvent(){return fEvent;}
		inline std::vector<G4int> GetSiPMNo(){return fSiPMNo;}
		inline std::vector<G4int> GetParticleID(){return fParticleID;}

		inline std::vector<G4double> GetEin (){return fEin;}
		inline std::vector<G4double> GetThetaIn(){return fThetaIn;}

		inline void SetPosInX(std::vector<G4double> posX){fPosInX = posX;}
		inline void SetPosInY(std::vector<G4double> posY){fPosInY = posY;}
		inline void SetPosInZ(std::vector<G4double> posZ){fPosInZ = posZ;}
		inline void SetPosSiPMInX(std::vector<G4double> posX){fPosSiPMInX = posX;}
		inline void SetPosSiPMInY(std::vector<G4double> posY){fPosSiPMInY = posY;}
		inline void SetPosSiPMInZ(std::vector<G4double> posZ){fPosSiPMInZ = posZ;}
		inline void SetMomInX(std::vector<G4double> momX){fMomInX = momX;}
		inline void SetMomInY(std::vector<G4double> momY){fMomInY = momY;}
		inline void SetMomInZ(std::vector<G4double> momZ){fMomInZ = momZ;}
		inline void SetTimeIn(std::vector<G4double> time){fTimeIn = time;}
		inline std::vector<G4double> GetPosInX(){return fPosInX;}
		inline std::vector<G4double> GetPosInY(){return fPosInY;}
		inline std::vector<G4double> GetPosInZ(){return fPosInZ;}
		inline std::vector<G4double> GetPosSiPMInX(){return fPosSiPMInX;}
		inline std::vector<G4double> GetPosSiPMInY(){return fPosSiPMInY;}
		inline std::vector<G4double> GetPosSiPMInZ(){return fPosSiPMInZ;}
		inline std::vector<G4double> GetMomInX(){return fMomInX;}
		inline std::vector<G4double> GetMomInY(){return fMomInY;}
		inline std::vector<G4double> GetMomInZ(){return fMomInZ;}
		inline std::vector<G4double> GetTimeIn(){return fTimeIn;}

		inline void Clear(){}

	private:
		// G4int fBounce; 
		std::vector<G4int> fEvent, fSiPMNo, fParticleID;
		std::vector<G4double> fEin, fThetaIn;

		std::vector<G4double> fPosInX, fPosInY, fPosInZ; 
		std::vector<G4double> fPosSiPMInX, fPosSiPMInY, fPosSiPMInZ; 
		std::vector<G4double> fMomInX, fMomInY, fMomInZ; 
		std::vector<G4double> fTimeIn;

		const G4VPhysicalVolume* fPhysVol;
};

typedef G4THitsCollection<SiPMHit> SiPMHitsCollection;

extern G4ThreadLocal G4Allocator<SiPMHit>* SiPMHitAllocator;

inline void* SiPMHit::operator new(size_t){
	if(!SiPMHitAllocator) SiPMHitAllocator = new G4Allocator<SiPMHit>;
	return (void*) SiPMHitAllocator->MallocSingle();
}

inline void SiPMHit::operator delete(void* aHit){
	SiPMHitAllocator->FreeSingle((SiPMHit*) aHit);
}

#endif


