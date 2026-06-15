/* date = June 12th 2026 9:44 pm */

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define E 2.7182818284f


static _Fcomplex E32C = {E,0};
static _Fcomplex PI32C = {(float)PI,0};



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

_Fcomplex f2(_Fcomplex z){
    float A = 1.5f;
    float T = 2.0f / 3.0f;
    float F = 1.0f / T;
    
    _Fcomplex Exponent ={0, 2 * PI  * F * z._Val[0] + PI};
    
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

float eps(float t){
    if(t >= 0) return 1;
    return 0;
}

float _g(float t){
    return (-t-2) * eps(t-2);
}

float _g2(float t){
    return _g(t+2);
}

#endif //FUNCTIONS_H
