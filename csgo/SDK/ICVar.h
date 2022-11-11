#pragma once
#include "CConVar.h"
#include "IAppSystem.h"

using CVarDLLIdentifier_t = int;
class CConBase;
class CConVar;
class CConCmd;
class ICVar : public IAppSystem
{
public:
	virtual CVarDLLIdentifier_t	AllocateDLLIdentifier() = 0;
	virtual void RegisterConCommand(CConVar* pCommandBase, int iDefaultValue = 1) = 0;
	virtual void UnregisterConCommand(CConVar* pCommandBase) = 0;
	virtual void UnregisterConCommands(CVarDLLIdentifier_t id) = 0;
	virtual const char* GetCommandLineValue(const char* szVariableName) = 0;
	virtual CConBase* FindCommandBase(const char* szName) = 0;
	virtual const CConBase* FindCommandBase(const char* szName) const = 0;
	virtual CConVar* FindVar(const char* szVariableName) = 0;
	virtual const CConVar* FindVar(const char* szVariableName) const = 0;
	virtual CConCmd* FindCommand(const char* szName) = 0;
	virtual const CConCmd* FindCommand(const char* szName) const = 0;
	virtual void InstallGlobalChangeCallback(ChangeCallbackFn callback) = 0;
	virtual void RemoveGlobalChangeCallback(ChangeCallbackFn callback) = 0;
	virtual void CallGlobalChangeCallbacks(CConVar* pVar, const char* szOldString, float flOldValue) = 0;
	virtual void InstallConsoleDisplayFunc(void* pDisplayFunc) = 0;
	virtual void RemoveConsoleDisplayFunc(void* pDisplayFunc) = 0;
	virtual void ConsoleColorPrintf(const Color& color, const char* pFormat, ...) const = 0;
	virtual void ConsolePrintf(const char* pFormat, ...) const = 0;
	virtual void ConsoleDPrintf(const char* pFormat, ...) const = 0;
	virtual void RevertFlaggedConVars(int nFlag) = 0;
};