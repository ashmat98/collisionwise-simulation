#include <iostream>
#include <vector>
#include <Eigen/Dense>
using namespace std;
using namespace Eigen;
 
int main()
{
  MatrixXd m(2,2);
  m(0,0) = 3;
  m(1,0) = 2.5;
  m(0,1) = -1;
  m(1,1) = m(1,0) + m(0,1);

  Vector3d a = {1.3,-2.1,3};

  std::cout << a.array() << std::endl;
  cout<<floor(a.array())<<endl;

  vector<int> V;
  int q = 4;
  V.push_back(q);
  auto VV = &V;
  *VV

  return 0;
}