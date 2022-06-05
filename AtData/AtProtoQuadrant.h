#ifndef AtPROTOQUADRANT_H
#define AtPROTOQUADRANT_H

#include "AtHit.h"

#include <Rtypes.h>
#include <TH1.h>
#include <TObject.h>

#include <cstddef> // for size_t
#include <vector>

class TBuffer;
class TClass;
class TMemberInspector;

class AtProtoQuadrant : public TObject {
public:
   AtProtoQuadrant() = default;
   AtProtoQuadrant(Int_t QuadrantID);
   AtProtoQuadrant(std::vector<AtHit> HitArray, Int_t QuadrantID);
   AtProtoQuadrant(std::vector<AtHit> HitArray, Int_t QuadrantID, Double_t PhiQ);
   ~AtProtoQuadrant() = default;

   void SetEventID(Int_t evtid);
   void AddHit(AtHit *hit);
   void SetHitArray(std::vector<AtHit> *hitArray);
   void SetQuadrantID(Int_t QuadrantID);
   void SetPhiQ(Double_t PhiQ);
   void SetPhiDistribution(TH1D *PhiD);
   void AddPhiVal(Double_t phival);

   Int_t GetQuadrantID();
   Double_t GetPhiQ();
   Int_t GetEventID();
   std::size_t GetNumHits();
   AtHit *GetHit(Int_t hitNo);
   std::vector<AtHit> *GetHitArray();
   TH1D *GetPhiDistribution();
   std::vector<Double_t> *GetPhiArray();
   std::size_t GetNumPhiVal();

protected:
   std::vector<AtHit> fHitArrayQ; // Collection of hits in that quadrant
   Double_t fPhiQ{0.0};           // Phi angle on the quadrant
   Int_t fQuadrantID{}; // Quadrant ID : 1 (0-90) - 2 (90 - 180) -  3 (180-270) - 4 (270-360) - 0 (Central Pad) [Defined
                        // in the Phi Task]
   // TODO: A AtTrack object must be collected here...Future stuff
   Int_t fEventID{-1};
   TH1D fPhiDistr;
   std::vector<Double_t> fPhiDistrArray;

   ClassDef(AtProtoQuadrant, 1);
};

#endif
