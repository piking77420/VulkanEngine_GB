
#include <Core/Core.h>
#include "Engine.hpp"

int main()
{
	Engine* engine = new Engine();
	engine->EngineRun();
	delete engine;
	return 0;
}