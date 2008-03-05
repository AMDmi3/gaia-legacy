#include <GL/gl.h>

#include "Layer.h"
#include "LayerMeta.h"
#include "Region.h"

namespace gaia {

class TestLayer1 : public Layer {
public:
	TestLayer1() {}
	virtual ~TestLayer1() {}

	virtual void RenderRegion(Region *rgn);
};
REGISTER_GAIA_LAYER(TestLayer1, LayerMeta::LAYERGROUP_TRANSPARENT, true);

void TestLayer1::RenderRegion(Region *rgn) {
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINE_LOOP);
	glVertex3f(rgn->v[0].x, rgn->v[0].y, rgn->v[0].z);
	glVertex3f(rgn->v[1].x, rgn->v[1].y, rgn->v[1].z);
	glVertex3f(rgn->v[2].x, rgn->v[2].y, rgn->v[2].z);
	glVertex3f(rgn->v[3].x, rgn->v[3].y, rgn->v[3].z);
	glEnd();
}

} // namespace gaia
