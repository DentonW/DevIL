#ifdef  _DEBUG
#define IL_DEBUG
#endif//_DEBUG

#include <il\il.h>
#include <windows.h>
#include <direct.h>
#include <string>
using namespace std;

char *ImageExtArray[] =
{
	"jpe", "jpg", "jpeg",
	"bmp",
	"ico",
	"pbm", "pgm", "pnm", "ppm",
	"png",
	"bw", "rgb", "rgba", "sgi",
	"tga", "tif", "tiff",
	"pcx",
	NULL
};


void	ParseDirs(const string &_Dir, char **ExtList, char *ConvExt, bool Recurse);
bool	IsDir(WIN32_FIND_DATA *_Data);
char	*GetExtension(const char *FileName);
bool	CheckExtension(char *Arg, char *Ext);


char	*Ext;
string	NewExt;
int		i, j;


void BatchConv(char *Directory, char *ExtList, char *ConvExt, bool Recurse)
{
	ILuint Id, OrigId;
	ilGenImages(1, &Id);
	OrigId = ilGetInteger(IL_CUR_IMAGE);
	ilBindImage(Id);
	if (ExtList == NULL)
		ParseDirs(string(Directory), ImageExtArray, ConvExt, Recurse);
	else {
		/*char **List = ConvertExtList(ExtList);
		ParseDirs(string(Directory), ConvertExtList(ExtList), ConvExt, Recurse);
		DestroyExtList(List);*/
	}
	ilDeleteImages(1, &Id);
	ilBindImage(OrigId);
	return;
}


void ParseDirs(const string &_Dir, char **ExtList, char *ConvExt, bool Recurse)
{
	HANDLE			Search;
	WIN32_FIND_DATA	FindData;

	_chdir(_Dir.c_str());
	Search = FindFirstFile("*.*", &FindData);

	do {
		if (!strcmp(FindData.cFileName, ".") || !strcmp(FindData.cFileName, ".."))
			continue;
		if (IsDir(&FindData) && Recurse) {
			_chdir(FindData.cFileName);
			string NewDir = _Dir + string("\\");
			NewDir += FindData.cFileName;
			ParseDirs(NewDir, ExtList, ConvExt, Recurse);
			_chdir("..");
		}
		Ext = GetExtension(FindData.cFileName);
		if (Ext == NULL)
			continue;
		if (!_stricmp(Ext, ConvExt))  // Already has that extension.
			continue;
		for (j = 0; ExtList[j] != NULL; j++) {
			if (CheckExtension(FindData.cFileName, ExtList[j])) {
				string NewName;
				for (i = 0; i < Ext - FindData.cFileName; i++) {
					NewName += FindData.cFileName[i];
				}
				NewName += ConvExt;
				if (!ilLoadImage(FindData.cFileName))
					break;
				ilSaveImage((char*)NewName.c_str());
				break;
			}
		}
	} while (FindNextFile(Search, &FindData));

	FindClose(Search);
	return;
}


// Is the file actually a directory?
bool IsDir(WIN32_FIND_DATA *_Data)
{
	if (_Data->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		return true;
	return false;
}


char *GetExtension(const char *FileName)
{
	bool PeriodFound = false;
	char *Ext = (char*)FileName;
	long i, Len = (long)strlen(FileName);

	if (FileName == NULL || !Len)  // if not a good filename/extension, exit early
		return NULL;

	Ext += Len;  // start at the end

	for (i = Len; i >= 0; i--) {
		if (*Ext == '.') {  // try to find a period 
			PeriodFound = true;
			break;
		}
		Ext--;
	}

	if (!PeriodFound)  // if no period, no extension
		return NULL;

	return Ext+1;
}


// Simple function to test if a filename has a given extension, disregarding case
bool CheckExtension(char *Arg, char *Ext)
{
	bool	PeriodFound = false;
	char	*Argu = Arg;  // pointer to arg so we don't destroy arg
	unsigned int i;

	if (Arg == NULL || Ext == NULL || !strlen(Arg) || !strlen(Ext))  // if not a good filename/extension, exit early
		return false;

	Argu += strlen(Arg);  // start at the end


	for (i = (int)strlen(Arg); i >= 0; i--) {
		if (*Argu == '.') {  // try to find a period 
			PeriodFound = true;
			break;
		}
		Argu--;
	}

	if (!PeriodFound)  // if no period, no extension
		return false;

	if (!_stricmp(Argu+1, Ext))  // extension and ext match?
		return true;

	return false;  // if all else fails, return IL_FALSE
}
