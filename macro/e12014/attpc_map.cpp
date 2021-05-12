// attpc_map.cpp --- 
// 
// Description: 
// Author: Chenyang Niu
// Email: niu@frib.msu.edu; niu@pku.edu.cn 
// Created: Wed Mar 31 03:01:42 2021 (-0400)
// Last-Updated: Wed Apr 14 02:05:38 2021 (-0400)
//           By: Chenyang Niu
//     Update #: 33


//#include "../../AtMap/AtTpcMap.h"


void print(std::vector <int> const &a) {
   std::cout << "The vector elements are : ";

   for(int i=0; i < a.size(); i++)
   std::cout << a.at(i) << ' ';
}

int attpc_map()
{
  AtTpcMap *test = new AtTpcMap();
  test->GenerateATTPC();
  auto ele = test->GetPadRef(9908);
  //std::cout << ele << std::endl;
  print(ele);

  std::cout << std::endl;

  return 0;
}
