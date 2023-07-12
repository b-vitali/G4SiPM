/// \file  SiPMSD.cc
/// \brief Implementation of the SiPMSD class

#include "SiPMSD.hh"
#include "SiPMHit.hh"
#include "RunAction.hh"

#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4ios.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"

#include "G4Box.hh"
#include "G4GeometryTolerance.hh"
#include "G4VSolid.hh"


#include "TVector3.h"
SiPMSD::SiPMSD(G4String name) : 
G4VSensitiveDetector(name){ //fThetaOut((Double_t)(std::numeric_limits<double>::infinity()))
	SiPMName = name;
	fSiPMCollection = nullptr;
	collectionName.insert("sipmCollection");
	hitsCID = -1;
	G4cout<<"create SiPMSD: "<<name<<G4endl;

	G4AnalysisManager *man = G4AnalysisManager::Instance();

	double x = 20. + 2; 	x= x/2.;
	double y = 20. + 2;		y= y/2.;
	double z = 2.5 + 0.5;	z= z/2.;
	PhotonTime_ID = man->CreateH2(name+"_PhotonTime","PhotonTime", 1024, 0., 1024, 200, 0,200);
	//PhotonPosition_ID	= man->CreateH3(name+"_PhotonPos","PhotonPosition",100, -2., 2, 220, -11., 11, 20, 0,20);

}

SiPMSD::SiPMSD(G4String name, G4int ntuple):
G4VSensitiveDetector(name){
	G4cout<<"-----------------------------------------\n";
	G4cout<<"Create a tmp SiPMSD:"<<G4endl;
	G4cout<<"Createntupla "<<ntuple<<" for SiPM : "<<name;
	G4cout<<"\n-----------------------------------------\n";

	G4AnalysisManager *man = G4AnalysisManager::Instance();

	// Ntuple for the SiPMillator
	man->CreateNtuple(name, name);

	man->CreateNtupleIColumn("fEvent",fEvent);
	man->CreateNtupleIColumn("fSiPMNo",fSiPMNo);
	man->CreateNtupleIColumn("fParticleID",fParticleID);

	man->CreateNtupleDColumn("fEin",fEin);
	man->CreateNtupleDColumn("fThetaIn",fThetaIn);

	man->CreateNtupleDColumn("fPosInX",fPosInX);
	man->CreateNtupleDColumn("fPosInY",fPosInY);
	man->CreateNtupleDColumn("fPosInZ",fPosInZ);
	man->CreateNtupleDColumn("fPosSiPMInX",fPosSiPMInX);
	man->CreateNtupleDColumn("fPosSiPMInY",fPosSiPMInY);
	man->CreateNtupleDColumn("fPosSiPMInZ",fPosSiPMInZ);
	man->CreateNtupleDColumn("fMomInX",fMomInX);
	man->CreateNtupleDColumn("fMomInY",fMomInY);
	man->CreateNtupleDColumn("fMomInZ",fMomInZ);	
	man->CreateNtupleDColumn("fTimeIn",fTimeIn);
	
	man->FinishNtuple(ntuple);
}

SiPMSD::~SiPMSD(){}

void SiPMSD::Initialize(G4HCofThisEvent* hitsCE){
	fSiPMCollection = new SiPMHitsCollection(SensitiveDetectorName, collectionName[0]);

	// Putting all the hits in the same place
	
	if(hitsCID<0){
		hitsCID = G4SDManager::GetSDMpointer()->GetCollectionID(fSiPMCollection); 
	}
	hitsCE->AddHitsCollection(hitsCID, fSiPMCollection);

	// aid variables just to check
	Trk=-5;
	TrkParent=-5;

	/*
		Debug feature:
		use a G4String and "contains"
		no printout == ""; p = pre-info, i = in, o = out, g = gamma,
		e = else, 1 = trk ==1, + = additionla info; 
	*/
	// G4String debug	= "p1 i o n g e";
	debug	= "";
}


void SiPMSD::CreateEntry(G4Step *aStep){
	//G4cout<<"CreateEntry SiPM trk : "<<aStep->GetTrack()->GetTrackID()<<" -> "<<Trk<<G4endl;

	G4Track * track = aStep->GetTrack();
	G4StepPoint* preStep = aStep->GetPreStepPoint();
    G4TouchableHistory* thePreTouchable = (G4TouchableHistory*)(preStep->GetTouchable());

	fEvent.push_back(G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID());
	fParticleID.push_back(track->GetParticleDefinition()->GetPDGEncoding());

	//! GetCopyNumber 0 or 1 if there is an "element volume" with "read" and "SiPM in"
	int q = 0;
	int bUD=1;
    int bSiPM=8;
    // G4cout<<"Start q = "<<q<<G4endl;
	// q = q + preStep->GetTouchable()->GetCopyNumber(2);
    // G4cout<<"q = "<<q<<G4endl;
	// q = q<<bUD;
    // G4cout<<"q = "<<q<<G4endl;
	// q = q + preStep->GetTouchable()->GetCopyNumber(1);
    // G4cout<<"q = "<<q<<G4endl;
	// q = q<<bSiPM;
    // G4cout<<"q = "<<q<<G4endl;
	// q = q + preStep->GetTouchable()->GetCopyNumber(0);
    // G4cout<<"q = "<<q<<G4endl;
	
	// std::cout<<" "<<preStep->GetTouchable()->GetCopyNumber(1)<<" "<<preStep->GetTouchable()->GetCopyNumber(2)<<std::endl;
	q = preStep->GetTouchable()->GetCopyNumber(1)*1000+preStep->GetTouchable()->GetCopyNumber(2);
	fSiPMNo.push_back(q);
	fEin.push_back(preStep->GetKineticEnergy());
	fMomInX.push_back(preStep->GetMomentum().getX());
	fMomInY.push_back(preStep->GetMomentum().getY());
	fMomInZ.push_back(preStep->GetMomentum().getZ());

	//? Position and time SIPM OR PHOTON
	fPosInX.push_back(aStep->GetPreStepPoint()->GetPosition().getX());
	fPosInY.push_back(aStep->GetPreStepPoint()->GetPosition().getY());
	fPosInZ.push_back(aStep->GetPreStepPoint()->GetPosition().getZ());
	fPosSiPMInX.push_back(preStep->GetTouchable()->GetTranslation().getX());
	fPosSiPMInY.push_back(preStep->GetTouchable()->GetTranslation().getY());
	fPosSiPMInZ.push_back(preStep->GetTouchable()->GetTranslation().getZ());
	fTimeIn.push_back(aStep->GetPreStepPoint()->GetGlobalTime());

	//? Angle (transform the momentum direction to the volume's reference system)
	G4ThreeVector worldPos = preStep->GetPosition();
	G4ThreeVector localPos = thePreTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
	G4AffineTransform momentumTransform = thePreTouchable->GetHistory()->GetTopTransform();
	momentumTransform.SetNetTranslation(G4ThreeVector(0,0,0));
	fDirIn = aStep->GetPreStepPoint()->GetMomentumDirection();
	fDirIn_trans = momentumTransform.TransformPoint(aStep->GetPreStepPoint()->GetMomentumDirection());
	
	if(G4StrUtil::contains(debug, "i+")) G4cout<<"fDirIn [pre trasform] : "<<fDirIn.x()<<" "<<fDirIn.y()<<" "<<fDirIn.z()<<G4endl;
	if(G4StrUtil::contains(debug, "i+")) G4cout<<"fDirIn [Volume's reference] :"<<fDirIn_trans.x()<<" "<<fDirIn_trans.y()<<" "<<fDirIn_trans.z()<<G4endl;

	G4ThreeVector norm = -thePreTouchable->GetVolume(0)->GetLogicalVolume()->GetSolid()->SurfaceNormal(localPos);
	fThetaIn.push_back(norm.dot(fDirIn_trans));

	if(G4StrUtil::contains(debug, "i+")) G4cout<<"norm: "<<norm.x()<<" "<<norm.y()<<" "<<norm.z()<<G4endl;
	if(G4StrUtil::contains(debug, "i")) G4cout<<"cos(fThetaIn) = "<<norm.dot(fDirIn_trans)<<" and fThetaIn [deg] = "<<std::acos(norm.dot(fDirIn_trans)) * 180/CLHEP::pi<<G4endl;

	EntryTrk = aStep->GetTrack()->GetTrackID();
}



G4bool SiPMSD::ProcessHits(G4Step *aStep, G4TouchableHistory* ROhist){	

	if(G4StrUtil::contains(debug, "p"))	G4cout<<"Ev : "<<G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID()<<G4endl;

	if(aStep->GetStepLength() == 0 && aStep->GetTotalEnergyDeposit() == 0) {
		// G4cout<<"step of lenght 0 and edep 0"<<G4endl; 
		return false;
	}

	//? Take start and end of the G4Step
	G4StepPoint* preStep = aStep->GetPreStepPoint();
	G4StepPoint* postStep = aStep->GetPostStepPoint();

	//? Take the G4VPhysicalVolume for both start and end
    G4TouchableHistory* thePreTouchable = (G4TouchableHistory*)(preStep->GetTouchable());
    G4VPhysicalVolume* thePrePV = thePreTouchable->GetVolume();
    G4TouchableHistory* thePostTouchable = (G4TouchableHistory*)(postStep->GetTouchable());
    G4VPhysicalVolume* thePostPV = thePostTouchable->GetVolume();

	if(G4StrUtil::contains(debug, "p+")) G4cout<<thePrePV->GetName()<<" "<<thePostPV->GetName()<< G4endl;
	if(G4StrUtil::contains(debug, "p+")) G4cout<<"track id "<< aStep->GetTrack()->GetTrackID()<< G4endl;

	Trk = aStep->GetTrack()->GetTrackID();
	TrkParent = aStep->GetTrack()->GetParentID();

	int pdgid = aStep->GetTrack()->GetDynamicParticle()->GetParticleDefinition()->GetPDGEncoding();
	//G4cout<<"pdgid : "<<pdgid<<G4endl;

	//? A photon is the first entry? 
	// if( pdgid == 22 || pdgid == -22 ){ //photon pdgid == 22 or -22
        CreateEntry(aStep);
		
        G4ThreeVector worldPos = postStep->GetPosition();
        G4ThreeVector localpos = thePreTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPos);
		
    	G4AnalysisManager *man = G4AnalysisManager::Instance();

        man->FillH2(PhotonTime_ID, postStep->GetGlobalTime(), fSiPMNo.at(fSiPMNo.size()-1));
        //man->FillH3(PhotonPosition_ID, localpos.z(), localpos.y(), fSiPMNo.at(fSiPMNo.size()-1));

	// }
	// else G4cout<<"Not a photon entering a SiPM : "<<pdgid<<G4endl;
	aStep->GetTrack()->SetTrackStatus(fStopAndKill);
}

void SiPMSD::FillHit(){
	SiPMHit* Hit = new SiPMHit();

	Hit->SetEvent(fEvent);
	
	Hit->SetSiPMNo(fSiPMNo);
	Hit->SetParticleID(fParticleID);

	Hit->SetEin(fEin);
	Hit->SetThetaIn(fThetaIn); //std::acos(fThetaIn) * 180/CLHEP::pi
	
	Hit->SetPosInX(fPosInX);
	Hit->SetPosInY(fPosInY);
	Hit->SetPosInZ(fPosInZ);
	Hit->SetPosSiPMInX(fPosSiPMInX);
	Hit->SetPosSiPMInY(fPosSiPMInY);
	Hit->SetPosSiPMInZ(fPosSiPMInZ);
	Hit->SetMomInX(fMomInX);
	Hit->SetMomInY(fMomInY);
	Hit->SetMomInZ(fMomInZ);
	Hit->SetTimeIn(fTimeIn);

	fSiPMCollection->insert(Hit);

	fEvent.clear();
	fSiPMNo.clear();
	fParticleID.clear();

	fEin.clear();
	fThetaIn.clear();
	
	fPosInX.clear();
	fPosInY.clear();
	fPosInZ.clear();
	fPosSiPMInX.clear();
	fPosSiPMInY.clear();
	fPosSiPMInZ.clear();
	fMomInX.clear();
	fMomInY.clear();
	fMomInZ.clear();
	fTimeIn.clear();

	
	fDirIn_trans = fDirOut_trans = G4ThreeVector();
	fDirIn = fDirOut = G4ThreeVector();
}

void SiPMSD::EndOfEvent(G4HCofThisEvent* hitsCE){
	
	G4cout<<"\n=========================================\n";
	G4cout<<"========== SiPMSD::EndOfEvent ===========\n";
	G4cout<<"End of event n: "<<G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID()<<G4endl;
	if(fEvent.size()>0){
		G4cout<<"Number of sub-hits in "<<SiPMName<<" : "<<fEvent.size();
		FillHit();
	}
	else G4cout<<SiPMName<<" is empty!";
	G4cout<<"\n=========================================\n";
}

void SiPMSD::clear(){}

void SiPMSD::DrawAll(){}

void SiPMSD::PrintAll(){}

void SiPMSD::FillNtupla(G4AnalysisManager *man, SiPMHit* SiPMHit, G4int ntupla){
	
	G4cout<<"-----------------------------------------\n";
	G4cout<<"Fill SiPM Ntupla: "<<ntupla;	
	G4cout<<"\n-----------------------------------------\n";

	fEvent 	=  SiPMHit->GetEvent();
	fSiPMNo 	=  SiPMHit->GetSiPMNo();
	fParticleID 	=  SiPMHit->GetParticleID();
	fEin 	=  SiPMHit->GetEin();
	fThetaIn 	=  SiPMHit->GetThetaIn();
	fPosInX 	=  SiPMHit->GetPosInX();
	fPosInY 	=  SiPMHit->GetPosInY();
	fPosInZ 	=  SiPMHit->GetPosInZ();
	fPosSiPMInX 	=  SiPMHit->GetPosSiPMInX();
	fPosSiPMInY 	=  SiPMHit->GetPosSiPMInY();
	fPosSiPMInZ 	=  SiPMHit->GetPosSiPMInZ();
	fMomInX 	=  SiPMHit->GetMomInX();
	fMomInY 	=  SiPMHit->GetMomInY();
	fMomInZ 	=  SiPMHit->GetMomInZ();
	fTimeIn 	=  SiPMHit->GetTimeIn();

	man->AddNtupleRow(ntupla);
}