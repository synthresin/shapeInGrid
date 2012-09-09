#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "cinder/gl/Texture.h"
#include "cinder/svg/Svg.h"
#include "cinder/svg/SvgGl.h"
#include "cinder/cairo/Cairo.h"

#define GRID_NUM 10

using namespace ci;
using namespace ci::app;
using namespace std;

class shapeInGridApp : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void mouseUp (MouseEvent event);
	void update();
	void draw();
    void redraw();
    
    float   mGridSizeX;
    float   mGridSizeY;
    Perlin  mPerlin;
    Rand    mRand;
    
    bool    mDraw;
    
    // additional
    
    svg::DocRef     mDoc;
    gl::Texture     mTex;
};

gl::Texture renderSvgToTexture(svg::DocRef doc, Vec2f size)
{
    cairo::SurfaceImage srf(size.x, size.y, true);
    cairo::Context ctx( srf );
    ctx.render( *doc );
    srf.flush();
    return gl::Texture( srf.getSurface() );
}


void shapeInGridApp::setup()
{
    setWindowSize(600, 600);
    gl::enableAlphaBlending();
    mDoc = svg::Doc::create( loadResource("logoFrag1.svg") );
    
    
    
    mGridSizeX = (float)getWindowWidth()/ GRID_NUM;
    mGridSizeY = (float)getWindowHeight()/ GRID_NUM;

    mTex = renderSvgToTexture( mDoc, Vec2f(mGridSizeX, mGridSizeY) );
    printf("xsize : %f, ysize : %f" , mGridSizeX, mGridSizeY);
    //gl::clear(ColorA(0.85,0.92,0.88));
    gl::clear(ColorA(1,1,1));
}

void shapeInGridApp::mouseDown( MouseEvent event )
{
    mDraw = true;
}

void shapeInGridApp::mouseUp( MouseEvent event )
{
    mDraw = false;
}

void shapeInGridApp::update()
{
}

void shapeInGridApp::draw()
{
	if (mDraw) {
        redraw();
    }
}

void shapeInGridApp::redraw()
{
	//gl::clear(ColorA(0.85,0.92,0.88));
    gl::clear(ColorA(1,1,1));
    gl::draw(*mDoc);
    float multiplier = 100.8746f;
    for(int yGrid = 0; yGrid < GRID_NUM; yGrid++) {
        for (int xGrid = 0; xGrid < GRID_NUM; xGrid++) {
            
            int gridMode;
            //int gridMode = mRand.nextInt(0,3);
            float noise =( mPerlin.fBm( Vec2f(xGrid * multiplier, yGrid * multiplier)) + 1.0f )  /2;
            if(noise >= 0 && noise < 0.43f) {
                gridMode = 0;
            } else if(noise >= 0.43f && noise < 0.48f) {
                gridMode = 1;
            } else gridMode = 2;
            float opacity = mRand.nextFloat();
            
            float xInit = mGridSizeX * xGrid;
            float yInit = mGridSizeY * yGrid;
            
            if( mTex ) {
                gl::color( Color::white() );
                gl::draw( mTex, Rectf( Vec2f(xInit, yInit), Vec2f(xInit + mGridSizeX, yInit + mGridSizeY) ) );
            }
            switch (gridMode) {
                case 0:
                    glLineWidth(7.0f);
                    gl::color(0.86f,0.38f,0.47f, opacity);
                    gl::drawSolidCircle(Vec2f(xInit, yInit), 3.5f);
                    gl::drawLine(Vec2f(xInit, yInit), Vec2f(xInit + mGridSizeX, yInit + mGridSizeY) );
                    gl::drawSolidCircle(Vec2f(xInit + mGridSizeX, yInit + mGridSizeY),3.5f);
                    break;
                    
                case 1:
                    glLineWidth(3.0f);
                    gl::color(ColorA(0.46f, 0.76f, 0.83f, opacity));
                    gl::drawLine(Vec2f(xInit+mGridSizeX, yInit), Vec2f(xInit, yInit + mGridSizeY) );
                    break;
                    
                default:
                    break;
            }
            
            
        }
    }
}

CINDER_APP_BASIC( shapeInGridApp, RendererGl )
