#ifndef CreateSiPM_h
#define CreateSiPM_h 1

#include "G4MyPrint.hh"
using namespace G4MyPrint;

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4Cache.hh"
#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "SiPMHit.hh"
#include "SiPMSD.hh"

#include "G4GlobalMagFieldMessenger.hh"
#include "G4Cache.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4Event.hh"

class CreateSiPM
{
    public:
		//? Constructor
		CreateSiPM(G4String hName, double hReadSizeX, double hReadSizeY, double hReadSizeZ, double hFill);
		CreateSiPM(G4String hName);
        virtual ~CreateSiPM();

		//? Main function
        void Create(G4LogicalVolume * hLogicWorld);					// This calls the private functions
	
		//? Function to write the data out
		void CreateNTuples();										// Create NTuple (to be called in EventAction)
		void FillNTuples(G4HCofThisEvent*HCE, const G4Event* event);	// Write to file (to be called in EndOfEventAction)

	private:
		//? All sections are separated for clarity
		void Materials(); 										// Required materials
		void OpticalProperties();								// Optical properties of the materials
		void Volumes();											// Geometry 
		void SD();												// SensitiveDetectors

		//? Basic infos
		G4String hName;
		double hFill;

		//? Pointer to the universe
		G4LogicalVolume * hLogicWorld;

		//? Geometry	
		//-----------------------------------
		// Read dimensions
    	G4double hReadSizeX;
    	G4double hReadSizeY;
    	G4double hReadSizeZ;

		// Read dimensions
    	G4double hSiPMSizeX;
    	G4double hSiPMSizeY;
    	G4double hSiPMSizeZ;

		//? Volume, Logical and Placement	
		//-----------------------------------
		// Readout
		G4Box* hSolidRead;
		G4LogicalVolume* hLogicRead;
		G4PVPlacement* hPhysRead;

		// Grease
		G4Box* hSolidGrease;
		G4LogicalVolume* hLogicGrease;
		G4PVPlacement* hPhysGrease;
		
		// SiPM
		G4Box* hSolidSiPM;
		G4LogicalVolume* hLogicSiPM;
		G4PVPlacement* hPhysSiPM;

		G4bool hCheckOverlaps;

		//? Materials & Elements
		//-----------------------------------
		// Materials
		G4Material* hScintMaterial;
		G4Material* hSiPMMaterial;
		G4Material* hMaterial;

		//-----------------------------------
		G4Material* hAir;
		G4Material* hVacuum;
		G4Material* hVacuum_nogamma;
		G4Material* hOG; // optical grease BC 631 index saint gobain
		G4Material* hSi;
		G4Material* hSiResin;

		//-----------------------------------
		// Elements
		G4Element* hH;
		G4Element* hC;
		G4Element* hN;
		G4Element* hO;
		G4Element* hSie;
		G4Element* hY;
		G4Element* hCe;
		G4Element* hLu;

		//? For the EndOfEvent
		//-----------------------------------
		// Hit collection number
		G4int fCollIDSiPM;

		//-----------------------------------
		// temporary SD to create the ntuple
		SiPMSD * tmp_sipm;

		// To track if the ntuple were created
 		bool hNTUpleCreated;
		
		// to register each event just once
		G4int fEvID; 
};

#endif

