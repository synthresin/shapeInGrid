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
    void mouseMove( MouseEvent event );
	void update();
	void draw();
    void redraw();
    
    float           mGridSizeX;
    float           mGridSizeY;
    Perlin          mPerlin;
    Rand            mRand;
    
    bool            mDraw;
    
    // additional
    
    svg::DocRef     mDoc;
    svg::DocRef     mDoc2;
    svg::DocRef     mDoc3;
    svg::DocRef     mDoc4;
    

    
    Shape2d         mShape;
    Shape2d         mShape2;
    Shape2d         mShape3;
    Shape2d         mShape4;
    
    Vec2f           mMousePos;
    
    float           mMaxLengthSquared;
    float           mMaxLength;
    
    int mTexVal[GRID_NUM][GRID_NUM];
};

Shape2d getCenteredShape2dfromSVGDoc(svg::DocRef doc) {
    Shape2d s = doc->getShape();
    s.transform( MatrixAffine2f::makeTranslate( -doc->getSize()/2 ) );
    return s;
}

gl::Texture renderSvgToTexture( svg::DocRef doc )
{
    cairo::SurfaceImage srf(doc->getWidth(), doc->getHeight() , true);
    cairo::Context ctx( srf );
    ctx.render( *doc );
    srf.flush();
    return gl::Texture( srf.getSurface() );
}


void shapeInGridApp::setup()
{
    setWindowSize(600, 600);
    gl::enableAlphaBlending();
    
    // SVG 도큐먼트 받아오기
    mDoc = svg::Doc::create( loadResource("logoFrag1.svg") );
    mDoc2 = svg::Doc::create( loadResource("logoFrag2.svg") );
    mDoc3 = svg::Doc::create( loadResource("logoFrag3.svg") );
    mDoc4 = svg::Doc::create( loadResource("logoFrag4.svg") );
    
    // SVG 도큐먼트에서 Shape 뽑아오기
    mShape = getCenteredShape2dfromSVGDoc(mDoc);
    mShape2 = getCenteredShape2dfromSVGDoc(mDoc2);
    mShape3 = getCenteredShape2dfromSVGDoc(mDoc3);
    mShape4 = getCenteredShape2dfromSVGDoc(mDoc4);
    
    // 그리 크기에 따라 그리드 사이즈 받기
    mGridSizeX = (float)getWindowWidth()/ GRID_NUM;
    mGridSizeY = (float)getWindowHeight()/ GRID_NUM;
    
    printf("xsize : %f, ysize : %f" , mGridSizeX, mGridSizeY);
    //gl::clear(ColorA(0.85,0.92,0.88));
    gl::clear(ColorA(1,1,1));
    
    for(int yGrid = 0; yGrid < GRID_NUM; yGrid++) {
        for (int xGrid = 0; xGrid < GRID_NUM; xGrid++) {
            mTexVal[xGrid][yGrid] = Rand::randInt(0, 4);
        }
    }
    mMaxLengthSquared = getWindowWidth() * getWindowWidth() + getWindowHeight() * getWindowHeight();
    mMaxLength = sqrt(mMaxLengthSquared);
}


void shapeInGridApp::mouseDown( MouseEvent event )
{
    mDraw = true;
}

void shapeInGridApp::mouseUp( MouseEvent event )
{
    mDraw = false;
}

void shapeInGridApp::mouseMove( MouseEvent event )
{
    mMousePos = event.getPos();
}

void shapeInGridApp::update()
{
    if (mDraw)
    for(int yGrid = 0; yGrid < GRID_NUM; yGrid++) {
        for (int xGrid = 0; xGrid < GRID_NUM; xGrid++) {
        mTexVal[xGrid][yGrid] = Rand::randInt(0, 4);
        }
    }
}

void shapeInGridApp::draw()
{
	if (true) {
        redraw();
    }
}

void shapeInGridApp::redraw()
{
	//gl::clear(ColorA(0.85,0.92,0.88));
    gl::clear(ColorA(1,1,1));
    for(int yGrid = 0; yGrid < GRID_NUM; yGrid++) {
        for (int xGrid = 0; xGrid < GRID_NUM; xGrid++) {
            
            
            float xInit = mGridSizeX * xGrid;
            float yInit = mGridSizeY * yGrid;
            
            Vec2f center = Vec2f(xInit + mGridSizeX/2  ,yInit + mGridSizeY/2);
            Vec2f dirVec = mMousePos - center;
            float angle = toDegrees(atan2(dirVec.y, dirVec.x)) + 90;
            //float scale = lmap(dirVec.lengthSquared(), 0.0f, mMaxLengthSquared, 0.5f, 2.0f);
            //float scale = lmap(dirVec.lengthSquared(), mMaxLengthSquared, 0.0f, 2.0f, 0.5f);
            //float scale = lmap(sqrt(dirVec.lengthSquared()), mMaxLength, 0.0f, 1.5f, 0.5f);
            float scale = lmap(sqrt(dirVec.lengthSquared()), mMaxLength, 0.0f, .0001f, 1.0f);

            
            
            gl::pushMatrices();
            
            gl::translate(center);
            gl::scale(scale, scale);
            gl::rotate(angle);
            gl::color(Color(0,0,0));
            if(mTexVal[xGrid][yGrid] == 0) {
                gl::drawSolid(mShape);
            } else if(mTexVal[xGrid][yGrid] == 1) {
                gl::drawSolid(mShape2);
            } else if(mTexVal[xGrid][yGrid] == 2) {
                gl::drawSolid(mShape3);
            } else if(mTexVal[xGrid][yGrid] == 3) {
                gl::drawSolid(mShape4);
            }
            
            gl::popMatrices();
                    
        }
    }
}

CINDER_APP_BASIC( shapeInGridApp, RendererGl )
