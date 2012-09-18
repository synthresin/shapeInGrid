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
    
    gl::Texture     mTex;
    gl::Texture     mTex2;
    gl::Texture     mTex3;
    gl::Texture     mTex4;
    
    std::vector<Vec2f> mPoints;
    std::vector<Vec2f> mPoints2;
    std::vector<Vec2f> mPoints3;
    std::vector<Vec2f> mPoints4;
    
    Shape2d         mShape;
    
    Vec2f           mMousePos;
    
    gl::Texture     currentTexture;
    //
    
    int mTexVal[GRID_NUM][GRID_NUM];
};

std::vector<Vec2f> getPointFromSVG( svg::DocRef doc )
{
    svg::Polygon *p = (svg::Polygon *)doc->getChildren().front();
    return p->getPolyLine().getPoints();
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
    
    
    // 각 폴리곤에서 포인트들을 받아오기
    mPoints = getPointFromSVG(mDoc);
    mPoints2 = getPointFromSVG(mDoc2);
    mPoints3 = getPointFromSVG(mDoc3);
    mPoints4 = getPointFromSVG(mDoc4);
    
    // 그리 크기에 따라 그리드 사이즈 받기
    mGridSizeX = (float)getWindowWidth()/ GRID_NUM;
    mGridSizeY = (float)getWindowHeight()/ GRID_NUM;

    // SVG 도큐먼트에서 텍스쳐 받기
    mTex = renderSvgToTexture( mDoc );
    mTex2 = renderSvgToTexture( mDoc2 );
    mTex3 = renderSvgToTexture( mDoc3 );
    mTex4 = renderSvgToTexture( mDoc4 );
    
    // SVG 도큐먼트에서 세이프 받기
    
    mShape = mDoc->getShape();
    
    printf("xsize : %f, ysize : %f" , mGridSizeX, mGridSizeY);
    //gl::clear(ColorA(0.85,0.92,0.88));
    gl::clear(ColorA(1,1,1));
    
    for(int yGrid = 0; yGrid < GRID_NUM; yGrid++) {
        for (int xGrid = 0; xGrid < GRID_NUM; xGrid++) {
            mTexVal[xGrid][yGrid] = Rand::randInt(0, 4);
        }
    }
    
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
//    for(int yGrid = 0; yGrid < GRID_NUM; yGrid++) {
//        for (int xGrid = 0; xGrid < GRID_NUM; xGrid++) {
//        mTexVal[xGrid][yGrid] = Rand::randInt(0, 4);
//        }
//    }
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
            
            Vec2f center = Vec2f(xInit  ,yInit );
            Vec2f dirVec = mMousePos - center;
            float angle = toDegrees(atan2(dirVec.y, dirVec.x)) + 90;
            
            
            gl::pushMatrices();
            
            gl::translate(center);
            gl::scale(0.5, 0.5);
            gl::rotate(angle);
            gl::color(Color(0,0,0));
            gl::drawSolid(mShape);
            gl::popMatrices();
            
//            if( mTex ) {
//                Vec2f center = Vec2f(xInit + mGridSizeX /2 ,yInit + mGridSizeY / 2 );
//                
//                Vec2f dirVec = mMousePos - center;
//                
//                float angle = toDegrees(atan2(dirVec.y, dirVec.x)) + 90;
//                
//                gl::color( Color::white() );
//                gl::pushMatrices();
//                
//                gl::translate(center);
//                gl::rotate(angle);
//                
//                
//                switch (mTexVal[xGrid][yGrid]) {
//                    case 0:
//                        currentTexture = mTex;
//                        break;
//                        
//                    case 1:
//                        currentTexture = mTex2;
//                        break;
//                        
//                    case 2:
//                        currentTexture = mTex3;
//                        break;
//                        
//                    case 3:
//                        currentTexture = mTex4;
//                        break;
//                        
//                    default:
//                        break;
//                }
//                
//                gl::draw( currentTexture , Rectf( Vec2f(-mGridSizeX/2, -mGridSizeY/2), Vec2f(mGridSizeX/2, mGridSizeY/2) ) );
//                gl::popMatrices();
//                
//            }
            
        }
    }
}

CINDER_APP_BASIC( shapeInGridApp, RendererGl )
