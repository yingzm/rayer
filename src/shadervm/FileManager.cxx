#include "common.hxx"
#include "FileManager.hxx"
#include "TextureFile.hxx"
#include "ShaderFile.hxx"

FileManager::FileManager()
{
	const char *texpath = getenv("RAYERTEXTURES");

	if (texpath!=NULL) {
		m_path.push_back(texpath);
	}
}

FileManager::~FileManager()
{
	int n=m_files.size();
	for (int i=0; i<n; ++i) {
		TextureFile *tf = m_files[i];
		delete tf;
	}
	m_filenames.erase(m_filenames.begin(), m_filenames.end());
	m_files.erase(m_files.begin(), m_files.end());
}

TextureFile *FileManager::GetTextureFile(const string &name)
{
	for (unsigned int i=0; i<m_filenames.size(); ++i) {
		if (name.compare(m_filenames[i])==0) {
			return m_files[i];
		}
	}

	TextureFile *ptf = new TextureFile;

	int n = m_path.size();
	for (int j=-1; j<n; ++j) {
		string pathname;
		if (j!=-1) {
			pathname = m_path[j];
			pathname.append("\\");
		}
		pathname+=name;
		if (ptf->Load(pathname.c_str())>=0) {
			m_filenames.push_back(name);
			m_files.push_back(ptf);

			return ptf;
		}
		else {
			continue;
		}
	}

	// load failed
	printf("!<TextureFileManager::GetTextureFile> Failed to load texture %s\n",
		name.c_str());
	delete ptf;
	return NULL;
}

ShaderFileManager::ShaderFileManager()
{
	const char *texpath = getenv("RAYERSHADERS");

	if (texpath!=NULL) {
		m_path.push_back(texpath);
	}
}

ShaderFileManager::~ShaderFileManager()
{
	int n=m_files.size();
	for (int i=0; i<n; ++i) {
		ShaderFile *sf = m_files[i];
		delete sf;
	}
	m_filenames.erase(m_filenames.begin(), m_filenames.end());
	m_files.erase(m_files.begin(), m_files.end());
}

ShaderFile *ShaderFileManager::GetShaderFile(const string &name)
{
	for (unsigned int i=0; i<m_filenames.size(); ++i) {
		if (name.compare(m_filenames[i])==0) {
			return m_files[i];
		}
	}

	ShaderFile *ptf = new ShaderFile;
	int n = m_path.size();
	for (int j=-1; j<n; ++j) {
		string pathname;
		if (j!=-1) {
			pathname = m_path[j];
			pathname.append("\\");
		}
		pathname+=name;
		pathname+=string(".slo");
		if (ptf->Load(pathname)>=0) {
			m_filenames.push_back(name);
			m_files.push_back(ptf);

			return ptf;
		}
		else {
			continue;
		}
	}

	delete ptf;
	return NULL;

}
