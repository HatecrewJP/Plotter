#include "app.c"

int main(){
    SetConfigFlags(FLAG_MSAA_4X_HINT);
	InitWindow(1920,1080,"Raylib Test");
	int IsRunning = 1;
	SetTargetFPS(60);
	if(!IsWindowReady()) return -1;
	
    float dt = 0;
    
	while(!WindowShouldClose()){
        UpdateApp(&dt);
	}
	
}

