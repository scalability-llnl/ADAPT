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

#include <algorithm>
#include <cmath>


#include "R2.h"

int R2::eval(MergeTree& tree) const
{
  // For all nodes
  for (LocalIndexType i=0;i<tree.size();i++) {
    tree.node(i).metric(eval(i));

  }

  tree.inflate();

  return 1;
}

FunctionType R2::eval(LocalIndexType label) const
{
  if (label == LNULL)
    return this->mDefault;

  std::vector<GlobalIndexType> feature;
  std::vector<FunctionType> values;

  // Collect all vertices belonging to this feature
  mTree->constructFeature(label,feature);

  // If the feature has fewer than 3 vertices than a linear fit
  // is always perfect so we return 1
  if (feature.size() < 3)
    return 1;

  // Otherwise, we determine how well the data is fit as a linear
  // function of volume, i.e. vertex count
  values.resize(feature.size());

  LocalIndexType i;
  FunctionType mean_value = 0;
  FunctionType mean_volume = (FunctionType)(feature.size() / 2.0);

  for (i=0;i<feature.size();i++) {
    values[i] = mData[feature[i]];
    mean_value += values[i];
  }

  mean_value /= values.size();

  FunctionType cov = 0;
  FunctionType stddev_value = 0;
  FunctionType stddev_volume = 0;

  for (i=0;i<feature.size();i++) {
    cov += (values[i] - mean_value)*(i  - mean_volume);
    stddev_value += (values[i] - mean_value)*(values[i] - mean_value);
    stddev_volume += (i  - mean_volume)*(i  - mean_volume);
  }

  stddev_value = sqrt(stddev_value);
  stddev_volume = sqrt(stddev_volume);

  return (FunctionType)std::min((double)1,pow(cov / (stddev_value*stddev_volume),2));
}




