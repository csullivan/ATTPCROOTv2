#include "ATPSATask.hh"
#include "ATPSA.hh"
#include "ATPSASimple.hh"
#include "ATPSASimple2.hh"
#include "ATPSAProto.hh"


// FAIRROOT classes
#include "FairRootManager.h"
#include "FairRun.h"
#include "FairRuntimeDb.h"

#include <iostream>
#ifdef _OPENMP
#include <omp.h>
#endif

ClassImp(ATPSATask);

ATPSATask::ATPSATask()
{
  fLogger = FairLogger::GetLogger();
  fPar = NULL;

  fIsPersistence = kFALSE;
  fIsBGPK = kFALSE;
  fIsPeakFinder = kFALSE;
  fIsMaxFinder = kFALSE;
  fIsBaseCorr = kFALSE;
  fIsTimeCorr = kFALSE;

  fEventHArray = new TClonesArray("ATEvent");

  fPSAMode = 2;
}

ATPSATask::~ATPSATask()
{
}

void ATPSATask::SetPSAMode(Int_t value)               { fPSAMode = value; }
void ATPSATask::SetPersistence(Bool_t value)          { fIsPersistence = value; }
void ATPSATask::SetThreshold(Double_t threshold)      { fThreshold = threshold; }
void ATPSATask::SetBackGroundPeakFinder(Bool_t value) { fIsBGPK = value;}
void ATPSATask::SetPeakFinder()                       { fIsPeakFinder= kTRUE;fIsMaxFinder= kFALSE;}
void ATPSATask::SetMaxFinder()                        { fIsMaxFinder= kTRUE;fIsPeakFinder= kFALSE;}
void ATPSATask::SetBaseCorrection(Bool_t value)       { fIsBaseCorr = value;}
void ATPSATask::SetTimeCorrection(Bool_t value)       { fIsTimeCorr = value;}


InitStatus
ATPSATask::Init()
{
  FairRootManager *ioMan = FairRootManager::Instance();
  if (ioMan == 0) {
    fLogger -> Error(MESSAGE_ORIGIN, "Cannot find RootManager!");
    return kERROR;
  }

  fRawEventArray = (TClonesArray *) ioMan -> GetObject("ATRawEvent");
  if (fRawEventArray == 0) {
    fLogger -> Error(MESSAGE_ORIGIN, "Cannot find ATRawEvent array!");
    return kERROR;
  }

  if (fPSAMode == 0) {
    fLogger -> Info(MESSAGE_ORIGIN, "Using ATPSASimple!");

    fPSA = new ATPSASimple();
  } else if (fPSAMode == 1) {
    fLogger -> Info(MESSAGE_ORIGIN, "Using ATPSASimple2!");

    fPSA = new ATPSASimple2();

  } else if (fPSAMode == 2) {
    fLogger -> Info(MESSAGE_ORIGIN, "Using ATPSAProto!");
    fPSA = new ATPSAProto();
  }

  fPSA -> SetThreshold((Int_t)fThreshold);
  fPSA -> SetBaseCorrection(fIsBaseCorr);
  fPSA -> SetTimeCorrection(fIsTimeCorr);

   if(fIsBGPK){
	 fLogger -> Info(MESSAGE_ORIGIN, "Suppression of background in Peak Finder Enabled");
         fPSA -> SetBackGroundSuppression();
   }

   if(fIsPeakFinder){
	  fLogger -> Info(MESSAGE_ORIGIN, " Peak Finder enabled for hit pattern reconstruction");
    fPSA -> SetPeakFinder();
  }else if(fIsMaxFinder){
    fLogger -> Info(MESSAGE_ORIGIN, " Maximum Finder enabled for hit pattern reconstruction");
    fPSA -> SetMaxFinder();
  }else if(!fIsMaxFinder && !fIsPeakFinder){
    fLogger -> Error(MESSAGE_ORIGIN, " Please select a method for hit pattern reconstruction");
    return kERROR;
  }

  ioMan -> Register("ATEventH", "ATTPC", fEventHArray, fIsPersistence);

  return kSUCCESS;
}

void
ATPSATask::SetParContainers()
{
  FairRun *run = FairRun::Instance();
  if (!run)
    fLogger -> Fatal(MESSAGE_ORIGIN, "No analysis run!");

  FairRuntimeDb *db = run -> GetRuntimeDb();
  if (!db)
    fLogger -> Fatal(MESSAGE_ORIGIN, "No runtime database!");

  fPar = (ATDigiPar *) db -> getContainer("ATDigiPar");
  if (!fPar)
    fLogger -> Fatal(MESSAGE_ORIGIN, "ATDigiPar not found!!");
}

void
ATPSATask::Exec(Option_t *opt)
{


  fEventHArray -> Delete();

  if (fRawEventArray -> GetEntriesFast() == 0)
    return;

  ATRawEvent *rawEvent = (ATRawEvent *) fRawEventArray -> At(0);
  //std::cout << "  Event Number :  " << rawEvent -> GetEventID() << " Valid pads : " << rawEvent -> GetNumPads() << std::endl;

  ATEvent *event = (ATEvent *) new ((*fEventHArray)[0]) ATEvent();
  //event -> SetEventID(event -> GetEventID());
    event -> SetEventID(rawEvent -> GetEventID());

  if (!(rawEvent -> IsGood()))
    event -> SetIsGood(kFALSE);
  else {
    fPSA -> Analyze(rawEvent, event);
    event -> SetIsGood(kTRUE);
  }
}
