#ifndef DATUMREFRESOLVER_H
#define DATUMREFRESOLVER_H

#include <vector>
#include "ProSelection.h"
using namespace std;

namespace isis{
class DatumRefResolver
{
private:
	void HandleDatumPlane(ProSelection feat, vector<ProSelection> &);
	void HandleDatumAxis(ProSelection feat, vector<ProSelection> &);
	void HandleDatumCSys(ProSelection feat, vector<ProSelection> &);
	void HandleDatumPoint(ProSelection feat, vector<ProSelection> &);
	void AddIfMissing(ProSelection selection, vector<ProSelection> &result);
	void Resolve(ProSelection datum, vector<ProSelection> &result);
	ProSelection sel;
public:
	DatumRefResolver(ProSelection datum);
	void Resolve(vector<ProSelection> &result);
	static bool Equals(ProAsmcomppath &path1, ProAsmcomppath &path2);
	static bool Equals(ProModelitem &item1, ProModelitem &item2);
};
}

#endif