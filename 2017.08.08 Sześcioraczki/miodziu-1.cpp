#include <bits/stdc++.h>
using namespace std;

const char * t[] = {
	"142353",
	"531414",
	"425232",
	"314145",
	"253521"
};

typedef pair<int, int> PII;
#define mp make_pair

set<set<PII> > shapes;

void adjustShape(set<PII> &shape) {
	int x = shape.begin()->first;
	int y = shape.begin()->second;
	for (set<PII>::iterator it = shape.begin(); it != shape.end(); ++it) {
		if (it->first < x) { x = it->first; y = it->second; }
		else if (it->first == x && it->second < y) { y = it->second; }
	}
	set<PII> sh;
	for (set<PII>::iterator it = shape.begin(); it != shape.end(); ++it) {
		sh.insert(mp(it->first - x, it->second - y));
	}
	shape = sh;
}

void rotate(set<PII> &shape) {
	set<PII> sh;
	for (set<PII>::iterator it = shape.begin(); it != shape.end(); ++it) {
		sh.insert(mp(it->second, -it->first));
	}
	adjustShape(sh);
	shape = sh;
}

void flip(set<PII> &shape) {
	set<PII> sh;
	for (set<PII>::iterator it = shape.begin(); it != shape.end(); ++it) {
		sh.insert(mp(it->first, -it->second));
	}
	adjustShape(sh);
	shape = sh;
}

void baseShape(set<PII> &shape) {
	set<set<PII> > all;
	for (int i=0; i<4; ++i) {
		all.insert(shape);
		rotate(shape);
	}
	flip(shape);
	for (int i=0; i<4; ++i) {
		all.insert(shape);
		rotate(shape);
	}
	shape = *all.begin();
}

map<set<PII>, int> shapeId;
int nextShapeId;

void addShape(set<PII> shape) {
	adjustShape(shape);
	shapes.insert(shape);
	set<PII> base(shape);
	baseShape(base);
	if (shapeId.count(base) == 0)
		shapeId[base] = ++nextShapeId;
}

int getId(set<PII> shape) {
	baseShape(shape);
	return shapeId[shape];
}

void genShapes(set<PII> shape) {
	if (shape.size() == 5) {
		addShape(shape);
		return;
	}
	set<PII> sh(shape);
	for (set<PII>::iterator it = shape.begin(); it != shape.end(); ++it) {
		for (int i=it->first - 1; i<=it->first + 1; ++i)
		for (int j=it->second - 1; j<=it->second + 1; ++j)
		{
			if (i != it->first && j != it->second) continue;
			if (i == it->first && j == it->second) continue;
			PII p = mp(i, j);
			if (sh.count(p) > 0) continue;
			sh.insert(p);
			genShapes(sh);
			sh.erase(p);
		}
	}
}

void genShapes() {
	set<PII> shape;
	shape.insert(mp(0, 0));
	genShapes(shape);
}

const int ni = 5;
const int nj = 6;

int tt[ni][nj];

int nextId;
int solCnt;
set<int> usedIds;
void gen(int i, int j) {
	if (j == nj) return gen(i+1, 0);
	if (i == ni) {
		solCnt++;
		for (int i=0; i<ni; ++i) {
			for (int j=0; j<nj; ++j) printf("%c", 'A' + tt[i][j] - 1);
			printf("\n");
		}
		printf("\n");
		return;
	}
	if (tt[i][j] != 0) return gen(i, j+1);
	for (set<set<PII> >::iterator it = shapes.begin(); it != shapes.end(); ++it) {
		int id = getId(*it);
		if (usedIds.count(id) > 0) continue;
		bool ok = true;
		set<int> s;
		for (set<PII>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
			int ii = i + it2->first;
			int jj = j + it2->second;
			if (ii < 0 || jj < 0 || ii >= ni || jj >= nj) { ok = false; break; }
			if (tt[ii][jj] != 0) { ok = false; break; }
			if (s.count(t[ii][jj]) > 0) { ok = false; break; }
			s.insert(t[ii][jj]);
		}
		if (!ok) continue;
		usedIds.insert(id);
		++nextId;
		for (set<PII>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
			int ii = i + it2->first;
			int jj = j + it2->second;
			tt[ii][jj] = nextId;
		}
		gen(i, j+1);
		for (set<PII>::iterator it2 = it->begin(); it2 != it->end(); ++it2) {
			int ii = i + it2->first;
			int jj = j + it2->second;
			tt[ii][jj] = 0;
		}
		--nextId;
		usedIds.erase(id);
	}
}

void gen() {
	nextId = 0;
	solCnt = 0;
	gen(0, 0);
}

int main() {
	genShapes();
	gen();
	printf("sonCnt: %d\n", solCnt);
	return 0;
}
