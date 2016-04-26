#ifndef ATEVENTDRAWTASKPROTO_H
#define ATEVENTDRAWTASKPROTO_H


// FairRoot classes
#include "FairTask.h"
#include "FairLogger.h"

// ROOT classes
#include "TEvePointSet.h"
#include "TEveGeoShape.h"
#include "TEveBoxSet.h"
#include "TClonesArray.h"
#include "TVector3.h"
#include "TPaletteAxis.h"

#include "TCanvas.h"
#include "TH2.h"
#include "TH1.h"
#include "TH3.h"
#include "TGraph.h"
#include "TH2Poly.h"

#include "ATEventManagerProto.hh"
//#include "ATRiemannTrack.hh"
//#include "ATRiemannHit.hh"
#include "ATRawEvent.hh"
#include "ATEvent.hh"
#include "ATProtoEvent.hh"
#include "ATHoughSpaceLine.hh"
#include "ATHoughSpaceCircle.hh"
#include "ATHoughSpace.hh"
#include "ATHit.hh"
#include "AtTpcMap.h"
#include "ATProtoQuadrant.hh"
#include <fstream>

#ifndef __CINT__ // Boost
#include <boost/multi_array.hpp>
#endif //__CINT__

class ATEventDrawTaskProto : public FairTask
{
  public :
    ATEventDrawTaskProto();

    virtual ~ATEventDrawTaskProto();

    virtual InitStatus Init();
    virtual void Exec(Option_t* option);
    void Reset();
    void SetHitAttributes(Color_t, Size_t, Style_t);
    void Set3DHitStyleBar();
    void Set3DHitStyleBox();

    static void SelectPad(const char *rawevt);

    void SetProtoMap(TString map) {fMap = map;}

  protected:
    virtual void DrawPadWave();
    virtual void DrawPadPlane();

    void DrawHitPoints();
    void DrawHSpace();
    void DrawProtoSpace();

    void UpdateCvsPadWave();
    void UpdateCvsPadPlane();

     //Basic types

    Int_t fMultiHit;
    Int_t f3DHitStyle;
    Bool_t fUnpackHough;
    Bool_t fIsCircularHough;
    Bool_t fIsLinearHough;
    Bool_t fIsRawData;
    Color_t fHitColor;
    Size_t  fHitSize;
    Style_t fHitStyle;

    // ROOT Objects
    TPaletteAxis *fPadPlanePal;

    TH1F* fMesh;
    TH3F* f3DHist;

    TH1I*  fPadAll[300];
    TH1D* fPhiDistr[5];
    TH1I*  fPadWave;
    TH2Poly* fPadPlane;

    TCanvas* fCvsPadWave;
    TCanvas* fCvsPadPlane;

    TF1 *fHoughLinearFit;
    TString fMap;

    TClonesArray* fHitArray;
    TClonesArray* fRawEventArray;
    TClonesArray* fHoughSpaceArray;
    TClonesArray* fProtoEventArray;

    TEvePointSet* fHitSet;
    TEveBoxSet* fhitBoxSet;

    /// ATTPCROOT objects

    ATEventManagerProto* fEventManager;
    ATHit *fIniHit;
    AtTpcMap *fDetmap;
    ATRawEvent* fRawevent;




    ClassDef(ATEventDrawTaskProto,1);
  };

  #endif