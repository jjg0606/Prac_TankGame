#include "ProgramCore.h"
#include "TankGameScene.h"

using namespace std;

//Code here to Main
void ProgramCore::Main()
{
	ChangeScene(new TankGameScene());
}
