#include <stdio.h>
#include <windows.h>
#include <iostream.h>
#include <fstream.h>
#include <direct.h>


char		**LibDirs, **IncDirs;
int			NumLib, NumInc;
ofstream	Out;

bool	GetDirs(void);
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
	TestInc("gif_lib.h",	NULL,	"IL_NO_GIF",	false);
	TestInc("lcms.h",		"lcms",	"IL_NO_LCMS",	false);

	Out << endl;

	TestInc("gl.h",			"gl",	"ILUT_USE_OPENGL",	true);
	TestInc("d3d8.h",		NULL,	"ILUT_USE_DIRECTX8",true);
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



	printf("\n");

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
		Out << "#define " << Directive << endl;
	else
		Out << "//#define " << Directive << endl;

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

