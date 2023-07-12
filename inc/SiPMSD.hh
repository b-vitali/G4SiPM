/// \file  SiPMSD.hh
/// \brief Definition of the SiPMSD class

#ifndef SiPMSD_h
#define SiPMSD_h 1

#include "SiPMHit.hh"

#include "G4ThreeVector.hh"
#include "G4VSensitiveDetector.hh"
#include "G4OpticalPhoton.hh"

#include "TVector3.h"

#include "G4AnalysisManager.hh"			// to access root stuff

#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH2.h"
#include "TProfile.h"
#include "TVectorD.h"

#include "G4SDManager.hh"

class G4Step;
class G4HCofThisEvent;
class G4VLogicalVolume;

class SiPMSD : public G4VSensitiveDetector{
	public:
		SiPMSD(G4String name);
		SiPMSD(G4String name, G4int ntuple);
		virtual ~SiPMSD();

		virtual void Initialize(G4HCofThisEvent* hitsCE);
		virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory*ROhist);

		virtual void CreateEntry(G4Step *aStep);

		virtual void FillHit();
		virtual void FillNtupla(G4AnalysisManager *man, SiPMHit* SiPMHit, G4int ntupla);

		virtual void EndOfEvent(G4HCofThisEvent* hitsCE);
		virtual void clear();
		virtual void DrawAll();
		virtual void PrintAll();
		
	private:
		SiPMHitsCollection* fSiPMCollection;

		G4String SiPMName;

		G4int Trk=0;
		G4int TrkParent=0;
		G4int EntryTrk=0;
		G4int TrkDecay=0;
		G4bool EntryCreated = false;
		G4String debug	= "";

		//G4int fBounce; 
		std::vector<G4int> fEvent, fSiPMNo, fParticleID;
		std::vector<G4double> fEin, fThetaIn;

		std::vector<G4double> fPosInX, fPosInY, fPosInZ; 
		std::vector<G4double> fPosSiPMInX, fPosSiPMInY, fPosSiPMInZ; 
		std::vector<G4double> fMomInX, fMomInY, fMomInZ; 
		std::vector<G4double> fTimeIn;
		
		G4ThreeVector fDirIn_trans, fDirOut_trans;
		G4ThreeVector fDirIn, fDirOut;

		G4int PhotonTime_ID, PhotonPosition_ID;

		G4int hitsCID;
};

#endif


