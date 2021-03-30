#include "GLDebugDraw.h"
#include "Helpers/Util.h"

GLDebugDrawer::GLDebugDrawer()
{
}	

GLDebugDrawer::~GLDebugDrawer()
{
}

void GLDebugDrawer::Init()
{	
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
}

void GLDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	Line line;
	line.start_pos = Util::btVec3_to_glmVec3(from);
	line.end_pos = Util::btVec3_to_glmVec3(to);
	line.start_color = Util::btVec3_to_glmVec3(color);
	line.end_color = Util::btVec3_to_glmVec3(color);
	lines.push_back(line);
}

void GLDebugDrawer::drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
{
	std::cout << "DIE: " << from.getX() << "\n";
}
void GLDebugDrawer::setDebugMode(int debugMode) {
	m_debugMode = debugMode;
}
void GLDebugDrawer::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {}
void GLDebugDrawer::reportErrorWarning(const char* warningString) {std::cout << "ERRoR\n";}
void GLDebugDrawer::draw3dText(const btVector3& location, const char* textString) {}
