/*******************************************************************************
* Copyright (c) 2015, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* Written by Peer-Timo Bremer bremer5@llnl.gov
* LLNL-CODE-665196
* All rights reserved.
* 
* This file is part of ADAPT. For details, see
* https://github.com/scalability-llnl/ADAPT. Please also read the
* additional BSD notice below. Redistribution and use in source and
* binary forms, with or without modification, are permitted provided
* that the following conditions are met:
* 
* - Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the disclaimer below.
* 
* - Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the disclaimer (as noted below) in
*    the documentation and/or other materials provided with the
*    distribution.
* 
* - Neither the name of the LLNS/LLNL nor the names of its contributors
*    may be used to endorse or promote products derived from this software
*    without specific prior written permission.
* 
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LAWRENCE
* LIVERMORE NATIONAL SECURITY, LLC, THE U.S. DEPARTMENT OF ENERGY OR
* CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
* EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
* PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
* LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING ￼ IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
* 
* Additional BSD Notice 
* 
* 1. This notice is required to be provided under our contract with the
* U.S. Department of Energy (DOE). This work was produced at Lawrence
* Livermore National Laboratory under Contract No. DE-AC52-07NA27344
* with the DOE. 
* 
* 2. Neither the United States Government nor Lawrence Livermore
* National Security, LLC nor any of their employees, makes any warranty,
* express or implied, or assumes any liability or responsibility for the
* accuracy, completeness, or usefulness of any information, apparatus,
* product, or process disclosed, or represents that its use would not
* infringe privately-owned rights. 
* 
* 3. Also, reference herein to any specific commercial products,
* process, or services by trade name, trademark, manufacturer or
* otherwise does not necessarily constitute or imply its endorsement,
* recommendation, or favoring by the United States Government or
* Lawrence Livermore National Security, LLC. The views and opinions of
* authors expressed herein do not necessarily state or reflect those of
* the United States Government or Lawrence Livermore National Security,
* LLC, and shall not be used for advertising or product endorsement
* purposes.
********************************************************************************/

#ifndef NEIGHBORHOOD_H_
#define NEIGHBORHOOD_H_

#include <cstdio>
#include <stdint.h>

#include "Definitions.h"

class Neighborhood
{
public:

  class iterator {
  public:

    friend class Neighborhood;
    //! Allow the begin() function to use the special constructor
    //friend iterator Neighborhood::begin(GlobalIndexType origin);

    //! Allow the end() function to use the special constructor
    //friend iterator Neighborhood::end(GlobalIndexType origin);

    iterator();
    iterator(const iterator& it);
    ~iterator() {}

    iterator& operator=(const iterator& it);
    bool operator==(const iterator& it) {return !(*this != it);}
    bool operator!=(const iterator& it);

    iterator& operator++(int i);

    GlobalIndexType operator*() {return mOrigin + mOffsets[mCurrent];}

    bool end() const {return mCount == mCurrent;}

 private:
    iterator(GlobalIndexType v, GlobalIndexType dim[3],int8_t* neighbors,SignedGlobalIndexType* offsets, uint8_t count);

    bool inside(uint8_t);

    int8_t* mNeighbors;
    SignedGlobalIndexType* mOffsets;

    GlobalIndexType mOrigin;
    GlobalIndexType mCoords[3];

    uint8_t mCount;
    uint8_t mCurrent;

    GlobalIndexType mDim[3];
  };

  Neighborhood(GlobalIndexType dim[3]);

  ~Neighborhood() {}

  iterator begin(GlobalIndexType origin);
  iterator end(GlobalIndexType origin);

protected:

  int8_t* mNeighbors;
  SignedGlobalIndexType* mOffsets;
  uint8_t mCount;

  GlobalIndexType mDim[3];

  void computeOffsets();
};


#endif /* NEIGHBORHOOD_H_ */
