# SiPM Geant4 code
## Table of contents
* [Introduction](#Introduction)
* [Structure](#technologies)
* [Setup](#setup)

## Introduction
This project is an attempt to have a standalone code to easly drop in GEANT4 simulation a SiPM.

The SiPMSD will save incoming particles and will kill them after storing the informations.

```
std::vector<G4int> fEvent, fSiPMNo, fParticleID; 
std::vector<G4double> fEin, fThetaIn;

std::vector<G4double> fPosInX, fPosInY, fPosInZ; 
std::vector<G4double> fPosSiPMInX, fPosSiPMInY, fPosSiPMInZ; 
std::vector<G4double> fMomInX, fMomInY, fMomInZ; 
std::vector<G4double> fTimeIn;
```


A ntuple and two 2D histograms will be automatically generated via 

```
G4AnalysisManager *man = G4AnalysisManager::Instance();
```

## Structure
SiPM contains:
* inc
    * SiPMHit.hh
    * SiPMSD.hh
* src
    * SiPMHit.cc
    * SiPMSD.cc
* SiPM_Waveform.cc
* MyPrint.cpp
	
The last two files work in junction and are not related to GEANT4 simulation.

The simulation ends saving the information of every particle enetring the SiPM but no waveform is generated.

This is done *offline* running the SiPM_Waveform program, which can be configured.

```
************************************************************
************ Waveform analysis setup ... Start! ************
* File name   : output                                     *
* N Events    : 2                                          *
* Save waves  : 1                                          *
* Debug       : 0                                          *
* Verbose     : 0                                          *
* PDE         : 0.40                                       *
* Threshold   : -1.00                                      *
* Time window : [-10, 30] ns                               *
* Dark rate   : 1e+05                                      *
* Dark prob   : 3e-03                                      *
************************************************************
* Proceede (1/0)?                                          *
```

## Setup
How do you implement this in your own simulation?

Let's assume a sim fodler, containing with scr, inc, and a build folder.

After cloning the repository you will have an additional folder with its own scs and inc.

```
$ git clone https://github.com/b-vitali/SiPM.git
```

Then you need to include these files in your CMakeLists.txt

It should look something like this:

```
include_directories(
    ...
    ${PROJECT_SOURCE_DIR}/SiPM/inc
    ${PROJECT_SOURCE_DIR}/SiPM/cc
    ...
    ${PROJECT_SOURCE_DIR}/inc
    ${Geant4_INCLUDE_DIRS}
    ${ROOT_INCLUDE_DIRS}
)

file(GLOB sources 
    ...
    ${PROJECT_SOURCE_DIR}/SiPM/src/*.cc
    ...
    ${PROJECT_SOURCE_DIR}/src/*.cc)

file(GLOB headers 
    ...
    ${PROJECT_SOURCE_DIR}/SiPM/inc/*.hh
    ...
    ${PROJECT_SOURCE_DIR}/inc/*.hh
)
```