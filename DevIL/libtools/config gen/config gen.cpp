#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <direct.h>

using namespace std;

char		**LibDirs, **IncDirs;
int			NumLib, NumInc;
ofstream	Out;

bool	GetDirs(void);
bool	TestVC7(void);
bool	TestVC7_1(void);
void	TestInc(char *FileName, char *SubDir, char *Directive, bool IfFound);
void	CleanUp(void);
void	Compare(void);


int main(int argc, char **argv)
{
	char CurDir[1024];

	if (argc <= 1) {
		cout << "Please specify where to generate the config.h file." << endl;
		return 1;
	}
	chdir(argv[1]);
	getcwd(CurDir, 1024);

	if (!GetDirs())
		return false;

	Out.open("config.h.temp");
	Out << "#ifndef __CONFIG_H__" << endl << "#define __CONFIG_H__" << endl << endl;

	TestInc("jpeglib.h",	NULL,	"IL_NO_JPG",	false);
	TestInc("libmng.h",		NULL,	"IL_NO_MNG",	false);
	TestInc("png.h",		NULL,	"IL_NO_PNG",	false);
	TestInc("tiff.h",		NULL,	"IL_NO_TIF",	false);
	TestInc("lcms.h",		"lcms",	"IL_NO_LCMS",	false);

	Out << endl;

	TestInc("gl.h",			"gl",	"ILUT_USE_OPENGL",	true);
	TestInc("d3d8.h",		NULL,	"ILUT_USE_DIRECTX8",true);
	TestInc("d3d9.h",		NULL,	"ILUT_USE_DIRECTX9",true);
	TestInc("sdl.h",		NULL,	"ILUT_USE_SDL",		true);
	TestInc("windows.h",	NULL,	"ILUT_USE_WIN32",	true);

	Out << endl << "#endif /* __CONFIG_H__ */" << endl;
	Out.close();

	CleanUp();
	chdir(CurDir);
	Compare();

	return 0;
}



bool GetDirs()
{
	LONG	Result;
	HKEY	Key;
	char	Inc[2048], Lib[2048], Temp[2048];
	DWORD	IncLen=2048, LibLen=2048, i;
	char	*Token;

	if (TestVC7_1() || TestVC7())
		return true;

	Result = RegOpenKey(HKEY_CURRENT_USER, 
		TEXT("Software\\Microsoft\\DevStudio\\6.0\\Build System\\Components\\Platforms\\Win32 (x86)\\Directories"),
		&Key);
	if (Result != ERROR_SUCCESS)
		return false;

	Result = RegQueryValueEx(Key,
		TEXT("Include Dirs"),
		NULL,
		NULL,
		(LPBYTE)Inc,
		&IncLen);
	if (Result != ERROR_SUCCESS) {
		RegCloseKey(Key);
		return false;
	}

	Result = RegQueryValueEx(Key,
		TEXT("Library Dirs"),
		NULL,
		NULL,
		(LPBYTE)Lib,
		&LibLen);
	if (Result != ERROR_SUCCESS) {
		RegCloseKey(Key);
		return false;
	}

	RegCloseKey(Key);


	i = 0;
	strcpy(Temp, Inc);
	Token = strtok(Temp, ";");
	while (Token != NULL) {
		i++;
		Token = strtok(NULL, ";");
	}
	IncDirs = new char *[i];
	NumInc = i;

	i = 0;
	strcpy(Temp, Inc);
	Token = strtok(Temp, ";");
	while (Token != NULL) {
		IncDirs[i++] = strdup(Token);
		Token = strtok(NULL, ";");
	}


	strcpy(Temp, Lib);
	i = 0;
	Token = strtok(Temp, ";");
	while (Token != NULL) {
		i++;
		Token = strtok(NULL, ";");
	}
	LibDirs = new char *[i];
	NumLib = i;

	i = 0;
	strcpy(Temp, Lib);
	Token = strtok(Temp, ";");
	while (Token != NULL) {
		LibDirs[i++] = strdup(Token);
		Token = strtok(NULL, ";");
	}


	return true;
}


bool TestVC7()
{
	LONG	Result;
	HKEY	Key;
	char	VCInstallDir[2048], Inc[2048], Temp[2048];
	DWORD	InstLen=2048, IncLen=2048, i;
	char	*Token;

	Result = RegOpenKey(HKEY_LOCAL_MACHINE, 
		TEXT("SOFTWARE\\Microsoft\\VisualStudio\\7.0\\Setup\\VC"),
		&Key);
	if (Result != ERROR_SUCCESS)
		return false;

	Result = RegQueryValueEx(Key,
		TEXT("ProductDir"),
		NULL,
		NULL,
		(LPBYTE)VCInstallDir,
		&InstLen);
	if (Result != ERROR_SUCCESS) {
		RegCloseKey(Key);
		return false;
	}
	RegCloseKey(Key);


	Result = RegOpenKey(HKEY_LOCAL_MACHINE, 
		TEXT("SOFTWARE\\Microsoft\\VisualStudio\\7.0\\VC\\VC_OBJECTS_PLATFORM_INFO\\Win32\\Directories"),
		&Key);
	if (Result != ERROR_SUCCESS)
		return false;

	Result = RegQueryValueEx(Key,
		TEXT("Include Dirs"),
		NULL,
		NULL,
		(LPBYTE)Inc,
		&IncLen);
	if (Result != ERROR_SUCCESS) {
		RegCloseKey(Key);
		return false;
	}
	RegCloseKey(Key);


	i = 0;
	strcpy(Temp, Inc);
	Token = strtok(Temp, ";");
	while (Token != NULL) {
		i++;
		Token = strtok(NULL, ";");
	}
	IncDirs = new char *[i];
	NumInc = i;

	i = 0;
	strcpy(Temp, Inc);
	Token = strtok(Temp, ";");
	while (Token != NULL) {
		IncDirs[i++] = strdup(Token);
		Token = strtok(NULL, ";");
	}


	// Replace $(VCInstallDir) with InstDir.
	for (i = 0; i < (DWORD)NumInc; i++) {
		if (!strncmp("$(VCInstallDir)", IncDirs[i], strlen("$(VCInstallDir)"))) {
			strcpy(Temp, VCInstallDir);
			strcat(Temp, IncDirs[i] + strlen("$(VCInstallDir)"));
			free(IncDirs[i]);
			IncDirs[i] = strdup(Temp);
		}
	}

	return true;
}


bool TestVC7_1()
{
	LONG	Result;
	HKEY	Key;
	char	VCInstallDir[2048], Inc[2048], Temp[2048];
	DWORD	InstLen=2048, IncLen=2048, i;
	char	*Token;

	Result = RegOpenKey(HKEY_LOCAL_MACHINE, 
		TEXT("SOFTWARE\\Microsoft\\VisualStudio\\7.1\\Setup\\VC"),
		&Key);
	if (Result != ERROR_SUCCESS)
		return false;

	Result = RegQueryValueEx(Key,
		TEXT("ProductDir"),
		NULL,
		NULL,
		(LPBYTE)VCInstallDir,
		&InstLen);
	if (Result != ERROR_SUCCESS) {
		RegCloseKey(Key);
		return false;
	}
	RegCloseKey(Key);


	Result = RegOpenKey(HKEY_LOCAL_MACHINE, 
		TEXT("SOFTWARE\\Microsoft\\VisualStudio\\7.1\\VC\\VC_OBJECTS_PLATFORM_INFO\\Win32\\Directories"),
		&Key);
	if (Result != ERROR_SUCCESS)
		return false;

	Result = RegQueryValueEx(Key,
		TEXT("Include Dirs"),
		NULL,
		NULL,
		(LPBYTE)Inc,
		&IncLen);
	if (Result != ERROR_SUCCESS) {
		RegCloseKey(Key);
		return false;
	}
	RegCloseKey(Key);


	i = 0;
	strcpy(Temp, Inc);
	Token = strtok(Temp, ";");
	while (Token != NULL) {
		i++;
		Token = strtok(NULL, ";");
	}
	IncDirs = new char *[i];
	NumInc = i;

	i = 0;
	strcpy(Temp, Inc);
	Token = strtok(Temp, ";");
	while (Token != NULL) {
		IncDirs[i++] = strdup(Token);
		Token = strtok(NULL, ";");
	}


	// Replace $(VCInstallDir) with InstDir.
	for (i = 0; i < (DWORD)NumInc; i++) {
		if (!strncmp("$(VCInstallDir)", IncDirs[i], strlen("$(VCInstallDir)"))) {
			strcpy(Temp, VCInstallDir);
			strcat(Temp, IncDirs[i] + strlen("$(VCInstallDir)"));
			free(IncDirs[i]);
			IncDirs[i] = strdup(Temp);
		}
	}

	return true;
}


void TestInc(char *FileName, char *SubDir, char *Directive, bool IfFound)
{
	WIN32_FIND_DATA	FindData;
	HANDLE	Handle;
	int		i;

	for (i = 0; i < NumInc; i++) {
		chdir(IncDirs[i]);
		chdir(SubDir);

		Handle = FindFirstFile(FileName, &FindData);
		if (Handle == INVALID_HANDLE_VALUE)
			continue;

		// Found the file, so return.
		FindClose(Handle);
		if (!IfFound)
			Out << "//#define " << Directive << endl;
		else
			Out << "#define " << Directive << endl;
		return;
	}

	if (IfFound)
		Out << "//#define " << Directive << endl;
	else
		Out << "#define " << Directive << endl;

	return;
}


void CleanUp()
{
	int i;

	for (i = 0; i < NumInc; i++) {
		free(IncDirs[i]);
	}
	for (i = 0; i < NumLib; i++) {
		free(LibDirs[i]);
	}

	delete []IncDirs;
	delete []LibDirs;

	return;
}


void Compare()
{
	FILE *config, *temp;

	config = fopen("config.h", "rt");
	if (!config) {
		rename("config.h.temp", "config.h");
		return;
	}

	temp = fopen("config.h.temp", "rt");

	while (!feof(temp)) {
		if (fgetc(temp) != fgetc(config)) {
			fclose(config);
			fclose(temp);
			remove("config.h");
			rename("config.h.temp", "config.h");
			return;
		}
	}

	fclose(config);
	fclose(temp);
	remove("config.h.temp");

	return;
}

