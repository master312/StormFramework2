    uint32_t vboId = 0;
    glGenBuffers(1, &vboId);
    if (vboId == 0) {
        LOG(FATAL) << "HEREEEEEEEE";
    }

    for (int i = 0; i < 4; i++) {
        std::cout << plane.getVertices()[i].x << " " << plane.getVertices()[i].y << std::endl;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2), plane.getVertices(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    uint32_t i = 0;
    while(i < 200) {
        video->clear();
        video->begin();
        
        plane.setPosition(plane.getPosition() + Vector2(0.01f, 0.01f));
        plane.transform();

        shader->use();
        glBindBuffer(GL_ARRAY_BUFFER, vboId);

        StormRenderCommandPlane rcp(plane);
        rcp.execute();
        /*glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, 6);*/

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        shader->unuse();

        
        platform->windowSwapBuffers();
        i++;
        SDL_Delay(1);
    }










#include <time.h>
timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

timespec time1, time2;

clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);

std::cout<< "DIFF " << diff(time1,time2).tv_sec << ":" << diff(time1,time2).tv_nsec << std::endl;