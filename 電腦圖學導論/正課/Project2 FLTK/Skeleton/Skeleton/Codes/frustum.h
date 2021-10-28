#ifndef _FRUSTUM_H_
#define _FRUSTUM_H_
#include "Edge.h"
#include "Vertex.h"
#include <utility>
class frustum{
public:
	frustum() {}
	frustum(const frustum& copy) {
		side[0] = copy.side[0];
		side[1] = copy.side[1];
		side[2] = copy.side[2];
		side[3] = copy.side[3];
	}
	frustum(Vertex a, Vertex b, Vertex c, Vertex d) {
		side[0] = a;
		side[1] = b;
		side[2] = c;
		side[3] = d;
	}
	void set(Vertex a, Vertex b, Vertex c, Vertex d) {
		side[0] = a;
		side[1] = b;
		side[2] = c;
		side[3] = d;
	}
	Vertex side[4];
	Edge which_edge(int which) {//which = 0~3
		if (which == 3) {
			return Edge(side + 3, side);
		}
		return Edge(side+which, side + which +1);
	}
	LineSeg which_line(int which) {//which = 0~3
		if (which == 3) {
			return LineSeg(side[3].posn[0], side[3].posn[1], side[0].posn[0], side[0].posn[1]);
		}
		return  LineSeg(side[which].posn[0], side[which].posn[1], side[which+1].posn[0], side[which+1].posn[1]);
	}
	
};
#endif // !_FRUSTUM_H
