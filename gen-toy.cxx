#include <xcdf/utility/XCDFUtility.h>
#include <xcdf/utility/Expression.h>

#include <cstdlib>

int main(){
  XCDFFile outfile("toy.xcd","w");
  XCDFFloatingPointField x = outfile.AllocateFloatingPointField("x",0);
  XCDFFloatingPointField y = outfile.AllocateFloatingPointField("y",0);
  XCDFFloatingPointField z = outfile.AllocateFloatingPointField("z",0);

  for(unsigned i = 0 ; i < 1000 ; i++){
    x<<-1 + 2 * ((double)rand()/(double)RAND_MAX);
    y<<-1 + 2 * ((double)rand()/(double)RAND_MAX);
    z<<-1 + 2 * ((double)rand()/(double)RAND_MAX);
    outfile.Write();
  }
}
