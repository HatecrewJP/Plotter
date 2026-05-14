#include "malloc.h"
#include "math.h"
#include "complex.h"
#include "stdio.h"

#include "assert.h"
#include "raylib.h"
#include "raymath.h"

#define E 2.7182818284f


static _Fcomplex E32C = {E,0};
static _Fcomplex PI32C = {(float)PI,0};


typedef enum CCS_TYPE{
    CCS_NONE,
    CCS_COMPLEX,
    CCS_REAL,
    //
    CCS_COUNT
}CCS_TYPE;


_Fcomplex f(float t){
    float A = 1.5f;
    float T = 2.0f / 3.0f;
    float F = 1.0f / T;
    
    _Fcomplex Exponent ={0, 2 * PI  * F * t + PI};
    
    _Fcomplex Value = cpowf(E32C,Exponent);
    Value._Val[0] *= A;
    Value._Val[1] *= A;
    Value._Val[0] += 0.0f;
    return Value;
}

float fImag(float t){
    float A = 1.5f;
    float T = 2.0f / 3.0f;
    float F = 1.0f / T;
    
    float Value = A * sinf(2*PI * F *t + PI);
    
    return Value;
}

float fReal(float t){
    float A = 1.5f;
    float T = 2.0f / 3.0f;
    float F = 1.0f / T;
    
    float Value = A * cosf(2*PI * F *t + PI);
    
    return Value;
}

_Fcomplex g(float t){
    _Fcomplex Result = {t,t};
    return Result;
}

float _g(float t){
    return t;
}




#define VALUES_PER_UNIT (UnitSize/2)
void PlotComplexFunction(_Fcomplex(*ToPlot)(float t),
                         Vector2* *PointsOut, 
                         int *PointCountOut, 
                         Rectangle Rect,
                         int UnitSize,
                         float dt)
{
    int x = (int)Rect.x;
    int y = (int)Rect.y;
    int w = (int)Rect.width;
    int h = (int)Rect.height;
    
    float t = 0;
    float UnitCount = (float)w/UnitSize;
    int PointCount = (int)(UnitCount * VALUES_PER_UNIT);
    Vector2 *Result = malloc(PointCount * sizeof(*Result));
    for(int Index = 0; Index < PointCount;Index++){
        t = ((float)Index / (float)(VALUES_PER_UNIT))-((float)(w/2)/(float)UnitSize);
        _Fcomplex Value = ToPlot(t+dt);
        float Real = Value._Val[0];
        float Imag = Value._Val[1];
        Vector2 Point = {t,-Imag};
        Point.x *= UnitSize;
        Point.y *= UnitSize;
        Point.x += x+(w/2);
        Point.y += y+(h/2);
        Result[Index] = Point; 
    }
    *PointsOut = Result;
    *PointCountOut = PointCount;
}
void PlotComplexFunction2(_Fcomplex(*ToPlot)(float t), 
                          Vector2* *PointsOut, 
                          int *PointCountOut, 
                          Rectangle Rect,
                          int UnitSize,
                          float dt)
{
    int x = (int)Rect.x;
    int y = (int)Rect.y;
    int w = (int)Rect.width;
    int h = (int)Rect.height;
    
    float t = 0;
    float UnitCount = (float)w/UnitSize;
    int PointCount = (int)(UnitCount * VALUES_PER_UNIT);
    Vector2 *Result = malloc(PointCount * sizeof(*Result));
    
    for(int Index = 0; Index < PointCount;Index++){
        t = ((float)Index / (float)VALUES_PER_UNIT)-(float)(w/2);
        _Fcomplex Value = ToPlot(t+dt);
        float Real = Value._Val[0];
        float Imag = Value._Val[1];
        Vector2 Point =  {Real,-Imag};
        Point.x *= UnitSize;
        Point.y *= UnitSize;
        Point.x += x+(w/2);
        Point.y += y+(h/2);
        Result[Index] = Point; 
    }
    
    *PointsOut = Result;
    *PointCountOut = PointCount;
}


void PlotNormalFunction(float (*ToPlot)(float x), 
                        Vector2* *PointsOut,
                        int *PointCountOut, 
                        Rectangle Rect,
                        int UnitSize,
                        float dt)
{
    int x = (int)Rect.x;
    int y = (int)Rect.y;
    int w = (int)Rect.width;
    int h = (int)Rect.height;
    
    float UnitCount = (float)w/(float)UnitSize;
    int PointCount = (int)(UnitCount * (float)VALUES_PER_UNIT);
    Vector2 *Result = malloc(PointCount * sizeof(*Result));
    
    for(int Index = 0; Index < PointCount;Index++){
        float t = ((float)Index / (float)VALUES_PER_UNIT)-((float)(w/2)/(float)UnitSize);
        float Value = ToPlot(t+dt)*(-1);
        
        Vector2 Point = {t,Value};
        
        Point.x *= UnitSize;
        Point.y *= UnitSize;
        Point.x += x + (w/2);
        Point.y += y+(h/2);
        Result[Index] = Point; 
        
    } 
    
    *PointsOut = Result;
    *PointCountOut = PointCount;
}


void DisplayPoints(Vector2* *Points, int PointCount, Color color, Rectangle ScissorRect){
    int x = (int)ScissorRect.x;
    int y = (int)ScissorRect.y;
    int w = (int)ScissorRect.width;
    int h = (int)ScissorRect.height;
    
    BeginScissorMode(x,y,w,h);
    
    DrawSplineLinear(*Points,PointCount,2.0f,color);
    EndScissorMode();
    free(*Points);
    *Points = 0;
}

#define TEXT_SIZE 80
void DrawCartesianCoordinateSystem(Rectangle Rect, int UnitSize, CCS_TYPE Type){
    int TopLeftX = (int)Rect.x;
    int TopLeftY = (int)Rect.y;
    int Width = (int)Rect.width;
    int Height = (int)Rect.height;
    float UnitCount = (float)Width/(float)UnitSize;
    BeginScissorMode(TopLeftX,TopLeftY,Width,Height);
    
    
    
    int MidX = TopLeftX + (Width/2);
    int MidY = TopLeftY + (Height/2);
    
    for(float c = (float)((int)(-UnitCount/2 - 1)); c <= (float)((int)(UnitCount/2 + 1)); c+= 0.25f){
        Vector2 s = {(float)(MidX + c * UnitSize), (float)(TopLeftY)};
        Vector2 e = {(float)(MidX + c * UnitSize), (float)(TopLeftY + Height)};
        DrawLineEx(s,e,0.5f,LIGHTGRAY);
    }
    
    for(float c = (float)((int)(-UnitCount/2 -1)); c <= (float)((int)(UnitCount/2 + 1)); c+=0.25f){
        Vector2 s = {(float)(TopLeftX)        , (float)(MidY + c * UnitSize)};
        Vector2 e = {(float)(TopLeftX + Width), (float)(MidY + c * UnitSize)};
        DrawLineEx(s,e,0.5f,LIGHTGRAY);
    }
    
    
    
    for(int c = -(int)UnitCount/2; c <= (int)UnitCount / 2; c+= 1){
        Vector2 s = {(float)(MidX + c * UnitSize), (float)(TopLeftY)};
        Vector2 e = {(float)(MidX + c * UnitSize), (float)(TopLeftY + Height)};
        DrawLineEx(s,e,0.75f,DARKGRAY);
    }
    
    for(int c = -(int)UnitCount/2; c <= (int)UnitCount/2; c+=1){
        Vector2 s = {(float)(TopLeftX)        , (float)(MidY + c * UnitSize)};
        Vector2 e = {(float)(TopLeftX + Width), (float)(MidY + c * UnitSize)};
        DrawLineEx(s,e,0.75f,DARKGRAY);
    }
    
    //coordinate system
    DrawLine(TopLeftX,TopLeftY+Height/2,TopLeftX+Width,TopLeftY + Height/2,BLACK);
    DrawLine(TopLeftX + Width/2,TopLeftY,TopLeftX + Width/2,TopLeftY + Height,BLACK);
    
    
    for(int c = -(int)UnitCount/2; c <= (int)UnitCount / 2; c+= 1){
        Vector2 s = {(float)(MidX + c * UnitSize), (float)(TopLeftY)};
        Vector2 e = {(float)(MidX + c * UnitSize), (float)(TopLeftY + Height)};
        DrawLineEx(s,e,0.75f,DARKGRAY);
    }
    
    for(int c = -(int)UnitCount/2; c <= (int)UnitCount/2; c+=1){
        Vector2 s = {(float)(TopLeftX)        , (float)(MidY + c * UnitSize)};
        Vector2 e = {(float)(TopLeftX + Width), (float)(MidY + c * UnitSize)};
        DrawLineEx(s,e,0.75f,DARKGRAY);
    }
    
    
    
    for(int c = -(int)UnitCount/2; c <= UnitCount/2; c+=1){
        int PosX = MidX + c * UnitSize;
        int PosY = TopLeftY + (Height/2);
        
        Vector2 s = {(float)PosX,(float)PosY + 10};
        Vector2 e = {(float)PosX,(float)PosY - 10};
        
        
        
        if(c < 0){
            char Buffer[256] ={0};
            snprintf(Buffer,256,"%d",c);
            DrawText(Buffer,PosX- 5*TEXT_SIZE/16,PosY + TEXT_SIZE/4, TEXT_SIZE/2, BLACK);
            DrawLineEx(s,e,3,BLACK);
        }
        if(c > 0){
            char Buffer[256] ={0};
            snprintf(Buffer,256,"%d",c);
            DrawText(Buffer,PosX - TEXT_SIZE/16 ,PosY + TEXT_SIZE/4, TEXT_SIZE/2, BLACK);
            DrawLineEx(s,e,3,BLACK);
        }
    }
    
    
    for(int c = -(int)UnitCount/2; c <= (int)UnitCount/2; c+=1){
        int PosX = TopLeftX + (Width/2);
        int PosY =  MidY + c * UnitSize;
        
        Vector2 s = {(float)PosX - 10,(float)PosY};
        Vector2 e = {(float)PosX + 10,(float)PosY};
        
        
        if(c < 0){
            char Buffer[256] ={0};
            snprintf(Buffer,256,"%d",-c);
            DrawText(Buffer,PosX - 2*TEXT_SIZE/4,PosY - TEXT_SIZE/ 4, TEXT_SIZE/2, BLACK);
            DrawLineEx(s,e,3,BLACK);
        }
        if(c > 0){
            char Buffer[256] ={0};
            snprintf(Buffer,256,"%d",-c);
            DrawText(Buffer,PosX - 3*TEXT_SIZE /4,PosY - TEXT_SIZE/ 4, TEXT_SIZE/2, BLACK);
            DrawLineEx(s,e,3,BLACK);
        }
    }
    
    
    
    DrawRectangleLinesEx(Rect, 3, GRAY);
    if(Type == CCS_COMPLEX){
        DrawText("Base:(1,i)",TopLeftX + 10, TopLeftY + 10,TEXT_SIZE/2,ORANGE);
    }
    if(Type== CCS_REAL){
        DrawText("Base:(1,1)",TopLeftX+ 10, TopLeftY + 10,TEXT_SIZE/2,ORANGE);
    }
    
    EndScissorMode();
}

void DrawPolarCoordinateSystem(Rectangle Rect){
    int TopLeftX = (int)Rect.x;
    int TopLeftY = (int)Rect.y;
    int Width = (int)Rect.width;
    int Height = (int)Rect.height;
    
    
    //coordinate system
    DrawLine(TopLeftX,TopLeftY+Height/2,TopLeftX+Width,TopLeftY + Height/2,BLACK);
    DrawLine(TopLeftX + Width/2,TopLeftY,TopLeftX + Width/2,TopLeftY + Height,BLACK);
    
    //TODO: Draw System
    
    
    DrawRectangleLinesEx(Rect, 3, GRAY);
    
}

typedef struct circle{
    union{
        Vector2 Pos;
        struct{
            float x;
            float y;
        };
    };
    float Radius;
}circle;

typedef struct slider{
    Rectangle Rect;
    Rectangle Line;
    circle Circle;
    int State;
    float Value;
    float ValueMin;
    float ValueMax;
}slider;

void UpdateSlider(int MouseX, 
                  int MouseY,
                  int LMBPressed,
                  int LMBReleased,
                  Vector2 MouseDelta,
                  slider *Slider
                  )
{
    
    
    int IsInCircleX =((MouseX - Slider->Circle.x) * (MouseX - Slider->Circle.x)) < (Slider->Circle.Radius * Slider->Circle.Radius);
    int IsInCircleY =((MouseY - Slider->Circle.y) * (MouseY - Slider->Circle.y)) < (Slider->Circle.Radius * Slider->Circle.Radius);
    int IsInCircle = IsInCircleX && IsInCircleY;
    
    if(LMBPressed && IsInCircle){
        Slider->State = 1;
    }
    if(LMBReleased){
        Slider->State = 0;
    }
    if(Slider->State){
        
        float NewPosX = (float) MouseX;
        if(NewPosX < Slider->Rect.x + Slider->Circle.Radius){
            NewPosX = Slider->Rect.x + Slider->Circle.Radius;
        }
        if(NewPosX > Slider->Rect.x + Slider->Rect.width - Slider->Circle.Radius){
            NewPosX = Slider->Rect.x + Slider->Rect.width - Slider->Circle.Radius;
        }
        Slider->Circle.x = NewPosX;
        
    }
    float RelPosX = (Slider->Circle.x - Slider->Rect.x);
    float Percentage = RelPosX / Slider->Rect.width;
    Slider->Value = (Slider->ValueMax - Slider->ValueMin) * Percentage + Slider->ValueMin;
    
    
}


static int IsInitialized;

static int UnitSize1;
static int UnitSize2;


static slider Slider1;

static slider Slider2;


void UpdateApp(float *_dt){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawFPS(0,0);
    if(IsKeyDown(KEY_LEFT)){
        *_dt+= 0.05f;
    }
    if(IsKeyDown(KEY_RIGHT)){
        *_dt-=0.05f;
    }
    if(IsKeyDown(KEY_R)){
        *_dt=0.0f;
    }
    
    Rectangle Rect1 = {10,10,800,800};
	Rectangle Rect2 = {900,10,800,800};
    
    if(!IsInitialized){
        Slider1.State = 0;
        Slider1.Value = 0.5f;
        Slider1.ValueMin = 0.0f;
        Slider1.ValueMax = 1.0f;
        Slider1.Rect = (Rectangle){10,820,300,50};
        Slider1.Line =(Rectangle){
            Slider1.Rect.x,
            Slider1.Rect.y+(Slider1.Rect.height/2)-2,
            Slider1.Rect.width,
            4
        };
        
        Slider1.Circle.x = Slider1.Line.x +Slider1.Line.width/2;
        Slider1.Circle.y = Slider1.Line.y+0.5f;
        Slider1.Circle.Radius = 10.0f;
        
        Slider2.State = 0;
        Slider2.Value = 0.5f;
        Slider2.ValueMin = 0.0f;
        Slider2.ValueMax = 1.0f;
        Slider2.Rect = (Rectangle){900,820,300,50};
        Slider2.Line = (Rectangle){
            Slider2.Rect.x,
            Slider2.Rect.y+(Slider2.Rect.height/2)-2,
            Slider2.Rect.width,
            4
        };
        Slider2.Circle.x = Slider2.Line.x+Slider2.Line.width/2;
        Slider2.Circle.y = Slider2.Line.y+0.5f;
        Slider2.Circle.Radius = 10.0f;
        
        UnitSize1 = (int)((150.0f - 60.0f)* 0.5f + 60);
        UnitSize2 = (int)((150.0f - 60.0f)* 0.5f + 60);
        
        IsInitialized = 1;
    }
    
    int PointCount = 0;
    Vector2 *GraphPoints = 0;
    float dt = *_dt; 
    
    PlotComplexFunction(&f,&GraphPoints,&PointCount ,Rect1,UnitSize1,dt);
    DisplayPoints(&GraphPoints,PointCount ,RED,Rect1);
    
    
    
    PlotNormalFunction(&fReal,&GraphPoints,&PointCount,Rect2,UnitSize2,dt);
    DisplayPoints(&GraphPoints,PointCount,GREEN,Rect2);
    
    PlotNormalFunction(&fImag,&GraphPoints,&PointCount,Rect2,UnitSize2,dt);
    DisplayPoints(&GraphPoints,PointCount,VIOLET,Rect2);
    
    
    PlotComplexFunction2(&f,&GraphPoints,&PointCount,Rect1,UnitSize1,dt);
    DisplayPoints(&GraphPoints,PointCount,BLUE,Rect1);
    
    PlotNormalFunction(&_g,&GraphPoints,&PointCount,Rect2,UnitSize2,dt);
    DisplayPoints(&GraphPoints,PointCount,RED,Rect2);
    
    
    DrawCartesianCoordinateSystem(Rect1,UnitSize1,CCS_COMPLEX);
    DrawCartesianCoordinateSystem(Rect2,UnitSize2,CCS_REAL);
    DrawRectangleRec(Slider1.Rect,GRAY);
    DrawRectangleLinesEx(Slider1.Rect,2,BLACK);
    DrawRectangleRec(Slider1.Line,BLACK);
    DrawCircleV(Slider1.Circle.Pos,Slider1.Circle.Radius,RED);
    
    
    DrawRectangleRec(Slider2.Rect,GRAY);
    DrawRectangleLinesEx(Slider2.Rect,2,BLACK);
    DrawRectangleRec(Slider2.Line,BLACK);
    DrawCircleV(Slider2.Circle.Pos,Slider2.Circle.Radius,RED);
    
    
    int MouseX = GetMouseX();
    int MouseY = GetMouseY();
    
    Vector2 MouseDelta = GetMouseDelta();
    
    int MouseLeftPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    int MouseLeftReleased = IsMouseButtonReleased(MOUSE_BUTTON_LEFT);
    
    UpdateSlider(MouseX,MouseY,MouseLeftPressed,MouseLeftReleased,MouseDelta,&Slider1);
    UpdateSlider(MouseX,MouseY,MouseLeftPressed,MouseLeftReleased,MouseDelta,&Slider2);
    
    UnitSize1 = (int)((200 - 60)* Slider1.Value + 60);
    UnitSize2 = (int)((200 - 60)* Slider2.Value + 60);
    
    char Buffer[256] ={0};
    snprintf(Buffer,256,"Value 1: %f",Slider1.Value);
    DrawText(Buffer,10,950,20, BLACK);
    snprintf(Buffer,256,"Value 2: %f",Slider2.Value);
    DrawText(Buffer,10,990,20, BLACK);
    
    
    EndDrawing();
}