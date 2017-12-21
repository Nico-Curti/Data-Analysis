#include "kabsch.hpp"
#include <iomanip>

int main(int argc, char *argv[])
{
	Point true_structure	("tst/pattern/3d_points_1.xyz"),
		  guessed_structure ("tst/pattern/3d_points_2.xyz");
	kabsch procrustes(true_structure, guessed_structure);
	std::cout << "rmsd = " << std::setprecision(5) << std::fixed << procrustes.get_rmsd() << std::endl;
	return 0;
}
