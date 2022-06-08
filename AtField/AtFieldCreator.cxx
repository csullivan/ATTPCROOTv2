/********************************************************************************
 *    Copyright (C) 2014 GSI Helmholtzzentrum fuer Schwerionenforschung GmbH    *
 *                                                                              *
 *              This software is distributed under the terms of the             *
 *         GNU Lesser General Public Licence version 3 (LGPL) version 3,        *
 *                  copied verbatim in the file "LICENSE"                       *
 ********************************************************************************/
// -------------------------------------------------------------------------
// -----                    AtConstField header file                  -----
// -----                Created 25/03/14  by M. Al-Turany              -----
// -------------------------------------------------------------------------

#include "AtFieldCreator.h"

#include "AtConstField.h"
#include "AtFieldPar.h"

#include <FairField.h>
#include <FairFieldFactory.h>
#include <FairParSet.h>
#include <FairRunAna.h>
#include <FairRuntimeDb.h>

#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

static AtFieldCreator gAtFieldCreator;

AtFieldCreator::AtFieldCreator() : FairFieldFactory(), fFieldPar(nullptr)
{
   fCreator = this;
}

AtFieldCreator::~AtFieldCreator() = default;

void AtFieldCreator::SetParm()
{
   FairRunAna *Run = FairRunAna::Instance();
   FairRuntimeDb *RunDB = Run->GetRuntimeDb();
   fFieldPar = dynamic_cast<AtFieldPar *>(RunDB->getContainer("AtFieldPar"));
}

FairField *AtFieldCreator::createFairField()
{
   FairField *fMagneticField = nullptr;

   if (!fFieldPar) {
      cerr << "-E-  No field parameters available!" << endl;
   } else {
      // Instantiate correct field type
      Int_t fType = fFieldPar->GetType();
      if (fType == 0)
         fMagneticField = new AtConstField(fFieldPar);
      else
         cerr << "-W- FairRunAna::GetField: Unknown field type " << fType << endl;
      cout << "New field at " << fMagneticField << ", type " << fType << endl;
      // Initialise field
      if (fMagneticField) {
         fMagneticField->Init();
         // fMagneticField->Print();
      }
   }
   return fMagneticField;
}

ClassImp(AtFieldCreator)
