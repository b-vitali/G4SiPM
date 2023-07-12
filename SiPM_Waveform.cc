#include <iostream>
#include <cmath>
#include "TCanvas.h"
#include "TRandom3.h"
#include "TGraph.h"
#include "TMath.h"
#include <stdlib.h>
#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TChain.h"
#include "TStyle.h"
#include "THStack.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"

#include "TNtuple.h"

#include "MyPrint.cpp"

//? Setup
TString filename = "output";
int     NEvents  = 2;
bool    save     = true;
bool    print    = false;
bool    verbose  = false;
double  PDE      = 0.4;
double  thr      = -1;
double  tmax     = 30;
double  tmin     = -10;
double  DarkRate = 100e3;
double  DarkProb = DarkRate*1e-6/tmax;

/*
    This class is to store per each event all the gammas which arrived to SiPMs
    It contains ev, sipmno, position (x,y,z) and time
*/
class Data {
    public:           
        Data() {}     // Empty Constructor
        Data(int fev, int fsipm, double fx, double fy, double fz, double ft) { // Constructor
            ev = fev;
            sipm = fsipm;
            x = fx;
            y = fy;
            z = fz;
            t = ft;
        }     
        int ev, sipm;
        double x,y,z,t;
};

//? Ask Giovanni
double Gp[5] = {8.88913032e-01,  6.46867235e+01,  4.16687779e-01, 212.49027149107357, 1.5};

double C(double x,double a,double b){
	return b * sqrt(TMath::Pi()/2) * exp(b*b/2/a/a - x/a)*(TMath::Erf((a*x - b*b)/(a*b*sqrt(2))) + 1);
}

double OneWave(double x){
	return Gp[3]*(C(x - Gp[4], Gp[0], Gp[2]) - C(x - Gp[4], Gp[0]*Gp[1]/(Gp[0]+Gp[1]), Gp[2]));
}

//? Sum a vector of functions std::vector<TF1*> fFuncList; 
struct SumTF1 { 
    SumTF1(const std::vector<TF1 *> & flist) : fFuncList(flist) {}
   
    double operator() (const double * x, const double *p) {
        double result = 0;
        for (unsigned int i = 0; i < fFuncList.size(); ++i) 
           result += -fFuncList[i]->EvalPar(x,p); 
        return result; 
    } 
   
    std::vector<TF1*> fFuncList; 
};

//? Function to add DarkNoise to the waveforms
std::vector<TF1 *> AddDark(std::vector<TF1 *> v){
    TRandom3 r;
    r.SetSeed();
    double t;
    //if(print) running_print(TString::Format("Add Dark with probability : %f", prob));

    float whole, fractional;
    fractional = std::modf(DarkProb, &whole);

    //if(print) cout<<whole<<" "<<fractional<<endl;

    r.Uniform(tmin,DarkProb);
    for(int i = 0; i < whole; i++){
        t = r.Uniform(tmin,tmax);
        v.push_back(new TF1("f",TString::Format("OneWave(x-%f)", t), tmin, tmax));
    }
    if(r.Rndm() < fractional) v.push_back(new TF1("f",TString::Format("OneWave(x-%f)", t), tmin, tmax));
    return v;
}


bool compareData(Data* d1, Data* d2)
{
    return (d1->sipm < d2->sipm);
}

//? Funcion to generate the waveforms
void Waves(vector<TF1* > * waves, TNtuple* T_new, std::vector<Data *> data, double thr){

    double Tx,Ty,Tz,Tt,Tcharge,Tamplitude;
    int Tev, Tsipm;

    // Sort the pair according to increasing SiPMNo
    sort(data.begin(), data.end(), compareData);

    // Empty vector for the resulting functions
    vector<TF1 *> v_fsum;
    std::vector<TF1 *> v; 

    TF1* f_tmp;

    // To implement the PDE
    TRandom3 r;
    r.SetSeed();
    double p;

    // Variables needed for the loops
    int channel = -1;
    int index = -1;
    int ngamma = 0;
    int SUMngamma = 0;

    // Main Loop : Loop on the std::vector<Data *>
    for(int i = 0; i < data.size(); i++){

        // If it is a new channel conclude the previous, clear v and increase index
        if(channel != data[i]->sipm) {

            // If you want print and verbose you get the number of gamma per SiPM
            if(print && verbose && i>0) {
                if(print) running_print(TString::Format("SiPMNo : %d; ngamma : %d", data[i-1]->sipm, ngamma));
            }
            SUMngamma += ngamma;
            ngamma = 0;
            
            channel = data[i]->sipm;
            if(index > -1) {
                v = AddDark(v);
                f_tmp = new TF1(TString::Format("f_ch%d", data[i]->sipm),SumTF1(v),tmin,tmax,0);
                Tamplitude = f_tmp->GetMinimum();
                if(Tamplitude < thr){
                    if(save) v_fsum.push_back(f_tmp);
                    // new TCanvas;
                    // v_fsum[index]->Draw();
                    Tev = data[i]->ev;
                    Tsipm = data[i]->sipm;
                    Tx = data[i]->x;
                    Ty = data[i]->y;
                    Tz = data[i]->z;
                    Tt = data[i]->t;
                    Tcharge = f_tmp->Integral(tmin,tmax);
                    T_new->Fill(Tev, Tsipm, Tx, Ty, Tz, Tt, Tcharge, Tamplitude);
                }
            }
            v.clear();
            index += 1;
        }

        // PDE SiPM
        p = r.Rndm();
        ngamma += 1;
        if(p < PDE){
            v.push_back(new TF1("f",TString::Format("OneWave(x-%f)", data[i]->t), tmin, tmax));
        }

        if(!print) print_progress_bar((float)i/(data.size()-1), TString::Format("Ev %d SiPM %d", data[i]->ev, channel));
    }

    // Keep track of all the photons analized
    SUMngamma += ngamma;

    // Return the generated waveforms
    *waves = v_fsum;

    // Some printout to keep stuff organized (if print==true)
    if(print) line_print();
    if(print) running_print(TString::Format("ngamma processed : %d;", SUMngamma));
    if(print) running_print(TString::Format("Number of SiPMs : %d", index));
    if(print) running_print(TString::Format("Number of waves registered: %zu", waves->size()));

}

TBrowser *OpenBrowser() { return new TBrowser; }

int CreateWaveforms(TString Tree, TFile * _file1){
    //? Open the file and import the TTree
    TFile *_file0 = TFile::Open("build/"+filename+".root");
    if(!_file0) {
        cout<< "file non existing?"<<endl;
        return 0;
    }
    TTree *T = _file0->Get<TTree>(Tree);
    cout<<Tree<<endl;

    //? Variables needed for the output
    vector<TF1*> * waves = new vector<TF1*>;
    double charge;

    TString treename= Tree + "_hits";
    TNtuple *T_new = new TNtuple(treename,"","ev:sipm:x:y:z:t:charge:amplitude");
    
    //? Variables to read the necessary branches
    std::vector<int> *Event   =0;
    std::vector<int> *SiPMNo  =0;
    std::vector<double> *X =0;
    std::vector<double> *Y =0;
    std::vector<double> *Z =0;
    std::vector<double> *Time =0;

    T->SetBranchAddress("fEvent",&(Event));
    T->SetBranchAddress("fSiPMNo",&(SiPMNo));
    T->SetBranchAddress("fPosSiPMInX",&(X));
    T->SetBranchAddress("fPosSiPMInY",&(Y));
    T->SetBranchAddress("fPosSiPMInZ",&(Z));
    T->SetBranchAddress("fTimeIn",&(Time));

    std::vector<Data *> data; 
    int ev;
        
    //? Loop on the entries (each contains vectors of time and sipmNo)
    if(NEvents > T->GetEntries()) NEvents = T->GetEntries();
    for(int i = 0; i < NEvents; i++){ //i< T->GetEntries()
        if(print) start_print(TString::Format(Tree + " Event N : %d", i));
        
        //? Get the i-entry of the TTree
        T->GetEntry(i);

        if(print) running_print(TString::Format("Number of gamma registered: %zu", (*SiPMNo).size()));
        //? Put info (for the i event) in array of Data
        data.reserve((*SiPMNo).size());
        for(int j=0; j<(*SiPMNo).size(); j++){
            data.push_back( new Data( (*Event)[j], (*SiPMNo)[j], (*X)[j], (*Y)[j], (*Z)[j], (*Time)[j]));
        }


        //? Call the Waves routine which gives you an array of TF1
        Waves(waves, T_new, data, thr);

        //? Insert the TF1 in folders accordin to the Ev number
        if(save){
            ev = (*Event)[i];                                     //! ?? NANI?!?!
            TString dirname = TString::Format("Ev_%d_", i)+Tree;
            _file1->mkdir(dirname);
            _file1->cd(dirname);
            for(int j = 0; j < waves->size(); j++){
                waves->at(j)->Write("");
            }
        }
        data.clear();
        if(print) finish_print(TString::Format(Tree + " Event N : %d", i));
    }
    _file1->cd("");
    cout<<"\n";
    T_new->Write();
    return 1;
}

int SiPM_Waveform(){
    cout<<endl;
    start_print(TString::Format("Waveform analysis setup"));

    running_print(                "File name   : " + filename);
    running_print(TString::Format("N Events    : %d", NEvents));
    running_print(TString::Format("Save waves  : %d", save));
    running_print(TString::Format("Debug       : %d", print));
    running_print(TString::Format("Verbose     : %d", verbose));
    running_print(TString::Format("PDE         : %.2f", PDE));
    running_print(TString::Format("Threshold   : %.2f", thr));
    running_print(TString::Format("Time window : [%.0f, %.0f] ns", tmin, tmax));
    running_print(TString::Format("Dark rate   : %.0e", DarkRate));
    running_print(TString::Format("Dark prob   : %.0e", DarkProb));
    line_print();
    running_print("Proceede (1/0)?");
    
    bool choice;
    cin>>choice;

    finish_print(TString::Format("Waveform analysis setup"));

    //? Create the file in which we will store the waveforms
    TString  newfile = "build/"+filename+"_wfs"+".root";
	TFile *_file1 = TFile::Open(newfile,"recreate");
    if(choice) CreateWaveforms("SiPM_out", _file1);
    line_print();
    if(choice) CreateWaveforms("SiPM_in", _file1);

    OpenBrowser();
    return 1;
}

void binary(){
    TRandom3 r;
    r.SetSeed();
    short int a = r.Uniform(0,8);   // N bit which SciFi
    short int b = r.Uniform(0,2);   // 1 bit Up/Down Left/Right
    short int c = r.Uniform(0,128); // 8 bit which SiPM

    cout<<a<<" "<<b<<" "<<c<<" "<<endl<<endl;

    int bUD=1;
    int bSiPM=8;

    short int q = 0;
    cout<<"q = "<<q<<endl;
    q = a;
    cout<<"q = "<<q<<endl;
    q = q<<bUD;
    cout<<"q = "<<q<<endl;
    q = q+b;
    cout<<"q = "<<q<<endl;
    q = q<<bSiPM;
    cout<<"q = "<<q<<endl;
    q = q+c;
    cout<<"q = "<<q<<endl;


    short int pUD = 0b1;            // 1 bit Up/Down Left/Right
    short int pSiPM = 0b11111111;   // 8 bit which SiPM

    c = q&pSiPM;
    b = q>>bSiPM&pUD;
    a = (q>>bSiPM)>>bUD;

    cout<<endl<<a<<" "<<b<<" "<<c<<" "<<endl;

}