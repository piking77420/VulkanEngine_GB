
#include <Core/Core.h>
#include "Engine.hpp"

int main()
{
	Engine engine;
	engine.EngineInit();
	engine.EngineRun();
	engine.DestroyEngine();
	return 0;
}