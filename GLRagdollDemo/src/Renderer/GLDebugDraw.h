#pragma once
#include "bullet/src/btBulletDynamicsCommon.h"
#include "bullet/src/LinearMath/btVector3.h"
#include "Header.h"

class GLDebugDrawer : public btIDebugDraw
{
	int m_debugMode; 

public:
	unsigned int VAO;
	unsigned int VBO;
	std::vector<Line> lines;
	std::vector<Vertex> vertices;
		
public:
	GLDebugDrawer();
	virtual ~GLDebugDrawer();
	virtual void    drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor);
	virtual void    drawLine(const btVector3& from, const btVector3& to, const btVector3& color);
	virtual void    drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);
	virtual void    reportErrorWarning(const char* warningString);
	virtual void    draw3dText(const btVector3& location, const char* textString);
	virtual void    setDebugMode(int debugMode);
	virtual int     getDebugMode() const { return m_debugMode; }
	void Init();
};
