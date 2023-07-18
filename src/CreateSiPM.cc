/// \file  CyFi.hh
/// \brief Implementation of the class to create CyFi

#include "CreateSiPM.hh"

CreateSiPM::CreateSiPM(G4String Name, double ReadSizeX, double ReadSizeY, double ReadSizeZ, double Fill)
{	
    hName = Name;
    hReadSizeX = ReadSizeX;
    hReadSizeY = ReadSizeY;
    hReadSizeZ = ReadSizeZ;
    hFill = Fill;
}

CreateSiPM::CreateSiPM(G4String Name, int NTupla)
{
    hName = Name;
	hNTupla = NTupla;
	hNTUpleCreated=false;
}

CreateSiPM :: ~CreateSiPM()
{}

G4LogicalVolume* CreateSiPM::Create(G4LogicalVolume * LogicWorld)
{
	hLogicWorld = LogicWorld;
	Materials();
	OpticalProperties();
	Volumes();
	SD();
	return hLogicRead;
}

void CreateSiPM::Materials()
{	
	start_print("CreateSiPM::Materials");

	G4double a; // atomic mass
	G4double z; // atomic number
	G4double density;

	//? You can take materials already defined 
	//G4NistManager* nist = G4NistManager::Instance();
	
	//? Or you can define your elements and materials
	//! Elements
	hH  = new G4Element( "hH",  "hH", z =  1., a =   1.01*g/mole);
	hC  = new G4Element( "hC",  "hC", z =  6., a =  12.01*g/mole);
	hN  = new G4Element( "hN",  "hN", z =  7., a =  14.01*g/mole);
	hO  = new G4Element( "hO",  "hO", z =  8., a =  16.00*g/mole);
	hSie= new G4Element("hSi", "hSi", z = 14., a = 28.0855*g/mole);
	hY  = new G4Element( "hY",  "hY", z = 39., a = 88.90585*g/mole);
	hCe = new G4Element("hCe", "hCe", z = 58., a = 140.116*g/mole);
	hLu = new G4Element("hLu", "hLu", z = 71., a = 174.967*g/mole);

	// Vacuuum
	hVacuum = new G4Material("hVacuum",z=1.,a=1.01*g/mole, 
		density = universe_mean_density, 
		kStateGas, 0.1 * kelvin, 1.e-19 * pascal);
	hVacuum_nogamma = new G4Material("hVacuum_nogamma",z=1.,a=1.01*g/mole, 
		density = universe_mean_density, 
		kStateGas, 0.1 * kelvin, 1.e-19 * pascal);

	// Air
	hAir = new G4Material("hAir", density = 0.0010*g/cm3, 2);
	hAir->AddElement(hN, 70 * perCent);
	hAir->AddElement(hO, 30 * perCent);

	// Optical grease
	hOG = new G4Material("hOpticalGrease",z=1.,a=1.01*g/mole, 
		     		 density = universe_mean_density, kStateGas,
				 0.1 * kelvin, 1.e-19 * pascal);

	// Silicium
	G4NistManager* NISTman = G4NistManager::Instance();
	hSi = NISTman->FindOrBuildMaterial("G4_Si");

	// Silicon resin
	hSiResin = new G4Material("hSiResin",z=1.,a=1.01*g/mole, 
		     		 density = universe_mean_density, kStateGas,
				 0.1 * kelvin, 1.e-19 * pascal);

	// Assign default materials
	hSiPMMaterial  = hSiResin;

	finish_print("CreateSiPM::Materials");
}

void CreateSiPM::OpticalProperties()
{
	start_print("CreateSiPM::OpticalProperties");

	//? Material properties tables
	//! fLYSO->GetIonisation()->SetBirksConstant(0.117645*mm/MeV);

	// ==========================================================	
	// Vacuum & Air
	// ==========================================================	
	G4double vacuum_Energy[] = {1.5*eV, 4.*eV};
	const G4int vacnum = sizeof(vacuum_Energy) / sizeof(G4double);

	G4double vRIND = 1.;
	G4double vacuum_RIND[] = {vRIND, vRIND};
	assert(sizeof(vacuum_RIND) == sizeof(vacuum_Energy));

	G4MaterialPropertiesTable* cvacuum_mt = new G4MaterialPropertiesTable();
	cvacuum_mt->AddProperty("RINDEX", vacuum_Energy, vacuum_RIND, vacnum);
	hVacuum->SetMaterialPropertiesTable(cvacuum_mt);
	hAir   ->SetMaterialPropertiesTable(cvacuum_mt);

	// ==========================================================	
	// Silicium
	// ==========================================================	
	G4double Si_Energy[] = {.5*eV, 9.*eV};
	const G4int Sinum = sizeof(vacuum_Energy) / sizeof(G4double);

	G4double Si_RIND[] = {3.4, 3.4};
	assert(sizeof(Si_RIND) == sizeof(Si_Energy));

	G4MaterialPropertiesTable* cSi_mt = new G4MaterialPropertiesTable();
	cSi_mt->AddProperty("RINDEX", Si_Energy, Si_RIND, Sinum);
	hSi->SetMaterialPropertiesTable(cSi_mt);

	// ==========================================================	
	// Silicon resin
	// ==========================================================	
	G4double SiRes_RIND[] = {1.41, 1.41};
	assert(sizeof(SiRes_RIND) == sizeof(Si_Energy));
	
	G4MaterialPropertiesTable* cSiRes_mt = new G4MaterialPropertiesTable();
	cSiRes_mt->AddProperty("RINDEX", Si_Energy, SiRes_RIND, Sinum);
	hSiResin->SetMaterialPropertiesTable(cSiRes_mt);

	// ==========================================================	
	// Optical grease
	// ==========================================================	
	//? better if it was higher?
	G4double OG_RIND[] = {1.465, 1.465};
	assert(sizeof(OG_RIND) == sizeof(Si_Energy));
		
	G4MaterialPropertiesTable* cOG_mt = new G4MaterialPropertiesTable();
	cOG_mt->AddProperty("RINDEX", Si_Energy, OG_RIND, Sinum);
	hOG->SetMaterialPropertiesTable(cOG_mt);

	finish_print("CreateSiPM::OpticalProperties");
}

void CreateSiPM::Volumes()
{
	start_print("CreateSiPM::Volumes");

	hCheckOverlaps = true;

	//? Read Solid and Phys.
	hSiPMSizeX = hFill*hReadSizeX;
	hSiPMSizeY = hFill*hReadSizeY;
	hSiPMSizeZ = 0.5*hReadSizeZ;

	// in
	hSolidRead	= new G4Box("Read", 0.5*hReadSizeX, 0.5*hReadSizeY, 0.5*hReadSizeZ);
    hLogicRead = new G4LogicalVolume(hSolidRead, hSi, "Read");
	hLogicRead->SetVisAttributes(G4Colour(1.0, 0.0, 1.0, 0.4));

	// grease Solid and Phys.
	hSolidGrease = new G4Box("Read", 0.5*hReadSizeX, 0.5*hReadSizeY,0.5 * 0.5*hReadSizeZ);
    hLogicGrease = new G4LogicalVolume(hSolidGrease, hOG, "Grease");
	hLogicGrease->SetVisAttributes(G4Colour(1,0,0, 0.5));

	// SiPM
	hSolidSiPM	= new G4Box("SiPM", 0.5*hSiPMSizeX, 0.5*hSiPMSizeY, 0.5 *hSiPMSizeZ);
    hLogicSiPM 	= new G4LogicalVolume(hSolidSiPM, hSiPMMaterial, "SiPM");
    hLogicSiPM	->SetVisAttributes(G4Colour(0,0,1, 0.5));

	//? Put Grease and SiPM in Read
    G4ThreeVector Grease_pos = G4ThreeVector(0, 0, (0.5*0.5*hReadSizeZ)); 
    G4ThreeVector SiPM_pos = G4ThreeVector(0, 0, -0.5*0.5*hReadSizeZ);
	
	//hPhysRead 	= new G4PVPlacement(0, G4ThreeVector(0, 0, 0), hLogicRead, "Read", hLogicWorld, true, 0, hCheckOverlaps);
	hPhysGrease = new G4PVPlacement(0, Grease_pos , hLogicGrease, "Grease", hLogicRead, false, hCheckOverlaps);
	hPhysSiPM 	= new G4PVPlacement(0, SiPM_pos, hLogicSiPM, "SiPM", hLogicRead, false, hCheckOverlaps);

	//! ? Surfaces
	finish_print("CreateSiPM::Volumes");
}

void CreateSiPM::SD()
{
	start_print("CreateSiPM::SD");

	auto sdManager = G4SDManager::GetSDMpointer();
   	G4String SDname;

	if(hLogicSiPM){
		SiPMSD * SiPM_SD = new SiPMSD(hName);
  		sdManager->AddNewDetector(SiPM_SD);
		hLogicSiPM->SetSensitiveDetector(SiPM_SD);
	}

	finish_print("CreateSiPM::SD");
}


void CreateSiPM::CreateNTuples(){
	start_print("CreateSiPM::CreateNTuple");

	if(!hNTUpleCreated){
		tmp_sipm 	= new SiPMSD(hName,hNTupla);
		hNTUpleCreated = true;
	}

	finish_print("CreateSiPM::CreateNTuple");
}

void CreateSiPM::FillNTuples(G4HCofThisEvent*HCE, const G4Event* event){
	start_print("CreateSiPM::FillNTuples");

	// Get a hold on the hit clollections in the G4SDManager
	G4SDManager* SDMan = G4SDManager::GetSDMpointer();
	fCollIDSiPM = SDMan->GetCollectionID(hName+"/sipmCollection");

	// Instance of the G4AnalysisManager to write out
	G4AnalysisManager *man = G4AnalysisManager::Instance();

	// Read the collection
	SiPMHitsCollection* SiPMHitCollection = (SiPMHitsCollection*) (HCE->GetHC(fCollIDSiPM));

	// For each hit fill the ntupla
	SiPMHit* sipmHit;
	G4int M = SiPMHitCollection->entries();
	for(int i = 0; i < M; i++){
		sipmHit = (*SiPMHitCollection)[i];
		tmp_sipm->FillNtupla(man, sipmHit,hNTupla);
		sipmHit->Clear();
	}

	finish_print("CreateSiPM::FillNTuples");
}