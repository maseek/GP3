#include "cTriangle.h"

cTriangle::cTriangle( )
{
	m_rotAngle = 0.f;
}

float cTriangle::getRotAngle( )
{
	return m_rotAngle;
}

void cTriangle::setRotAngle( float rotAngle )
{
	m_rotAngle = rotAngle;
}

void cTriangle::prepareTri( float dt )
{
	const float SPEED = 15.f;
	m_rotAngle += SPEED * dt;
	if ( m_rotAngle > 360.f )
	{
		m_rotAngle -= 360.f;
	}
}