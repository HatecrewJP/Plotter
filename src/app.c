#include "malloc.h"
#include "math.h"
#include "complex.h"
#include "stdio.h"

#include "assert.h"
#include "raylib.h"
#include "raymath.h"

#include "functions.h"

typedef enum CCS_TYPE{
    CCS_NONE,
    CCS_COMPLEX,
    CCS_REAL,
    //
    CCS_COUNT
}CCS_TYPE;





#define VALUES_PER_UNIT (UnitSize/2)
static void PlotComplexFunction(_Fcomplex(*ToPlot)(float t),
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

static void PlotComplexFunction2(_Fcomplex(*ToPlot)(float t), 
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


static void PlotNormalFunction(float (*ToPlot)(float x), 
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


static void DisplayPoints(Vector2* *Points, int PointCount, Color color, Rectangle ScissorRect){
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
static void DrawCartesianCoordinateSystem(Rectangle Rect, int UnitSize, int Type){
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

static void DrawPolarCoordinateSystem(Rectangle Rect){
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
    float HitBoxRadius;
}slider;

typedef enum SliderState{
    SLIDER_STATIC,
    SLIDER_HOVER,
    SLIDER_DRAG,
    
    
    SLIDER_STATE_COUNT
}SliderState;

static void UpdateSlider(int MouseX, 
                         int MouseY,
                         int LMBDown,
                         Vector2 MouseDelta,
                         slider *Slider
                         )
{
    
    
    int IsInCircleX =((MouseX - Slider->Circle.x) * (MouseX - Slider->Circle.x)) < (Slider->HitBoxRadius * Slider->HitBoxRadius);
    int IsInCircleY =((MouseY - Slider->Circle.y) * (MouseY - Slider->Circle.y)) < (Slider->HitBoxRadius * Slider->HitBoxRadius);
    int IsInCircle = IsInCircleX && IsInCircleY;
    
    
    
    if(LMBDown && IsInCircle){
        Slider->State = 1;
    }
    if(!LMBDown&& IsInCircle){
        Slider->State = 2;
    }
    if(!LMBDown && !IsInCircle) {
        Slider->State = 0;
    }
    
    if(Slider->State == 1){
        
        float NewPosX = (float) MouseX;
        if(NewPosX < Slider->Rect.x + Slider->Circle.Radius){
            NewPosX = Slider->Rect.x + Slider->Circle.Radius;
        }
        if(NewPosX > Slider->Rect.x + Slider->Rect.width - Slider->Circle.Radius){
            NewPosX = Slider->Rect.x + Slider->Rect.width - Slider->Circle.Radius;
        }
        Slider->Circle.x = NewPosX;
        
    }
    float RelPosX = (Slider->Circle.x - Slider->Rect.x - Slider->Circle.Radius);
    float Percentage = RelPosX / (Slider->Rect.width - 2 * Slider->Circle.Radius);
    Slider->Value = (Slider->ValueMax - Slider->ValueMin) * Percentage + Slider->ValueMin;
    
    
}



static int IsInitialized;
static int UnitSize1;
static int UnitSize2;
static slider Slider1;
static slider Slider2;

static slider CreateSlider(float x, float y, float width, float height, 
                           float ValMin, float ValMax, float ValInit, 
                           float HitBoxAdjustment)
{
    slider Result = {0};
    Result.State = SLIDER_STATIC;
    Result.Value = ValInit;
    Result.ValueMin = ValMin;
    Result.ValueMax = ValMax;
    Result.Rect = (Rectangle){x,y,width,height};
    Result.Line =(Rectangle){
        Result.Rect.x,
        Result.Rect.y+(Result.Rect.height/2)-2,
        Result.Rect.width,
        4
    };
    Result.Circle.x = Result.Line.x +Result.Line.width/2;
    Result.Circle.y = Result.Line.y+0.5f;
    Result.Circle.Radius = Result.Rect.height / 2 * 0.75f;
    Result.HitBoxRadius = Result.Circle.Radius + HitBoxAdjustment;
    return Result;
}

int DrawButton(int MouseX, int MouseY, int MouseLeftDown, 
               const char *Text, 
               int PosX, int PosY, int Height,
               int Margin, int Padding){
    int IsPressed = 0;
    float TextWidth = (float)MeasureText("Reset",Height / 2);
    DrawRectangle(PosX,PosY,
                  (int)(TextWidth + 2*Padding + 2*Margin),
                  Height/2 + 2 *Padding + 2 * Margin,
                  DARKGRAY);
    DrawRectangleLinesEx((Rectangle){
                             (TextWidth + 2*Padding + 2*Margin),
                             (float)(Height/2 + 2 *Padding + 2 * Margin),
                         }
                         ,2, BLACK);
    if(PosX+ Padding <= MouseX 
       && MouseX <= PosX + Padding + TextWidth + 2 * Margin  
       && PosY + Padding <= MouseY 
       && MouseY <= PosY + TextWidth + Padding + Height / 2 + 2*Margin)
    {
        if(MouseLeftDown){
            DrawRectangle(PosX + Padding,PosY + Padding,
                          (int)(TextWidth + 2 * Margin),
                          Height/2 + 2*Margin,
                          WHITE);
            DrawRectangleLinesEx((Rectangle){
                                     (float)PosX + Padding,
                                     (float)PosY + Padding,
                                     TextWidth + 2*Margin,
                                     (float)(Height/2 + 2 * Margin)
                                 }
                                 ,2, BLACK);
            IsPressed = 1;
        }
        else{
            DrawRectangle(PosX + Padding,PosY + Padding,
                          (int)(TextWidth + 2 * Margin),
                          Height/2 + 2*Margin,
                          LIGHTGRAY);
            DrawRectangleLinesEx((Rectangle){
                                     (float)PosX + Padding,
                                     (float)PosY + Padding,
                                     TextWidth + 2*Margin,
                                     (float)(Height/2 + 2 * Margin)
                                 }
                                 ,2, BLACK);
        }
    }
    else{
        DrawRectangle(PosX + Padding,PosY + Padding,
                      (int)(TextWidth + 2 * Margin),
                      Height/2 + 2*Margin,
                      GRAY);
        DrawRectangleLinesEx((Rectangle){
                                 (float)PosX + Padding,
                                 (float)PosY + Padding,
                                 TextWidth + 2*Margin,
                                 (float)(Height/2 + 2 * Margin)
                             }
                             ,2, BLACK);
    }
    DrawText(Text,PosX + Padding + Margin,PosY + Padding + Margin,Height / 2,BLACK);
    
    return IsPressed;
}

void UpdateApp(float *_dt){
    if(!IsInitialized){
        // {10,820,300,50}
        Slider1 = CreateSlider(10,820,300,50,0,1,0.5f,5);
        Slider2 = CreateSlider(900,820,300,50,0,1,0.5f,5);
        
        
        UnitSize1 = (int)((150.0f - 60.0f)* 0.5f + 60);
        UnitSize2 = (int)((150.0f - 60.0f)* 0.5f + 60);
        
        IsInitialized = 1;
    }
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
    
    
    int PointCount = 0;
    Vector2 *GraphPoints = 0;
    float dt = *_dt; 
    
    int MouseX = GetMouseX();
    int MouseY = GetMouseY();
    
    Vector2 MouseDelta = GetMouseDelta();
    int MouseLeftDown = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    
    
    PlotComplexFunction(&f,&GraphPoints,&PointCount,Rect1,UnitSize1,dt);
    DisplayPoints(&GraphPoints,PointCount,RED,Rect1);
    
    
    PlotNormalFunction(&_g,&GraphPoints,&PointCount,Rect2,UnitSize2,dt);
    DisplayPoints(&GraphPoints,PointCount,RED,Rect2);
    
    PlotNormalFunction(&_g2,&GraphPoints,&PointCount,Rect2,UnitSize2,dt);
    DisplayPoints(&GraphPoints,PointCount,GREEN,Rect2);
    
    
    DrawCartesianCoordinateSystem(Rect1,UnitSize1,CCS_COMPLEX);
    DrawRectangleRec(Slider1.Rect,GRAY);
    DrawRectangleLinesEx(Slider1.Rect,2,BLACK);
    DrawRectangleRec(Slider1.Line,BLACK);
    if(Slider1.State == 0){
        DrawCircleV(Slider1.Circle.Pos,Slider1.Circle.Radius,RED);
    } else {
        DrawCircleV(Slider1.Circle.Pos,Slider1.Circle.Radius,GREEN);
    }
    UpdateSlider(MouseX,MouseY,MouseLeftDown,MouseDelta,&Slider1);
    UnitSize1 = (int)((200 - 60)* Slider1.Value + 60);
    
    
    DrawCartesianCoordinateSystem(Rect2,UnitSize2,CCS_REAL);
    DrawRectangleRec(Slider2.Rect,GRAY);
    DrawRectangleLinesEx(Slider2.Rect,2,BLACK);
    DrawRectangleRec(Slider2.Line,BLACK);
    if(Slider2.State == 0){
        DrawCircleV(Slider2.Circle.Pos,Slider1.Circle.Radius,RED);
    } else {
        DrawCircleV(Slider2.Circle.Pos,Slider1.Circle.Radius,GREEN);
    }
    
    UpdateSlider(MouseX,MouseY,MouseLeftDown,MouseDelta,&Slider2);
    UnitSize2 = (int)((200 - 60)* Slider2.Value + 60);
    
    
    int IsPressed = DrawButton(MouseX,MouseY,MouseLeftDown,
                               "Reset",
                               10,880,60,
                               5,5);
    if(IsPressed){
        Slider1.Circle.Pos.x = Slider1.Line.x + Slider1.Line.width/2;
        Slider1.Circle.Pos.y = Slider1.Line.y + Slider1.Line.height/2;
    }
    
    IsPressed = DrawButton(MouseX,MouseY,MouseLeftDown,
                           "Reset",
                           900,880,60,
                           5,5);
    if(IsPressed){
        Slider2.Circle.Pos.x = Slider2.Line.x + Slider2.Line.width/2;
        Slider2.Circle.Pos.y = Slider2.Line.y + Slider2.Line.height/2;
    }
    
    
    
    
    
    
    
    char Buffer[256] ={0};
    (void)snprintf(Buffer,256,"Value 1: %f",Slider1.Value);
    DrawText(Buffer,10,950,20, BLACK);
    snprintf(Buffer,256,"Value 2: %f",Slider2.Value);
    DrawText(Buffer,10,990,20, BLACK);
    EndDrawing();
}