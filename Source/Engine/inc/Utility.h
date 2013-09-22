#pragma once
#include <Windows.h>
#include <iostream>
void OpenConsole()
{
	AllocConsole();
	// redirect the stdout to the console
	freopen("CONOUT$", "w", stdout);
}