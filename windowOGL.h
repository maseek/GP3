#ifndef _WINDOWOGL_H
#define _WINDOWOGL_H

class windowOGL 
{
public:
    windowOGL();

    bool initOGL();
	void renderOGL(float rotAngle);
    void shutdown();

    void onResize(int width, int height);

};

#endif
