#ifndef __FILEMANAGER_HXX__
#define __FILEMANAGER_HXX__

class TextureFile;
class ShaderFile;

class FileManager
{
public:
	FileManager();
	~FileManager();
	TextureFile *GetTextureFile(const string &name);

private:
	// sorry that map can't pass the compile stage
	vector<string> m_filenames;
	vector<TextureFile *> m_files;
	vector<string> m_path;
};

class ShaderFileManager
{
public:
	ShaderFileManager();
	~ShaderFileManager();
	ShaderFile *GetShaderFile(const string &name);

private:
	// sorry that map can't pass the compile stage
	vector<string> m_filenames;
	vector<ShaderFile *> m_files;
	vector<string> m_path;
};


#endif /* __FILEMANAGER_HXX__ */