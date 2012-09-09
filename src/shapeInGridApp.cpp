#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class shapeInGridApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
};

void shapeInGridApp::setup()
{
}

void shapeInGridApp::mouseDown( MouseEvent event )
{
}

void shapeInGridApp::update()
{
}

void shapeInGridApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP_BASIC( shapeInGridApp, RendererGl )
