/*
==========================================================================
cTriangle.h
==========================================================================
*/

#ifndef _TRIANGLE_H
#define _TRIANGLE_H

class cTriangle
{
public:
	cTriangle();
	float getRotAngle();
	void setRotAngle(float rotAngle);
	void prepareTri(float dt);

private:
	float m_rotAngle;
};

#endif