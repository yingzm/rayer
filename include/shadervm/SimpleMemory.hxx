#ifndef __SIMPLEMEMORY_HXX__
#define __SIMPLEMEMORY_HXX__

class SimpleMemory
{
public:
	SimpleMemory();
	~SimpleMemory();

	void Reserve(unsigned int size);
	char *GetMem(int size);

private:
	unsigned int m_size;
	unsigned int m_maxsize;
	char *m_mem;
	void ReserveMore(unsigned int size);
};

inline char *SimpleMemory::GetMem(int size)
{
	if (size!=0)
		ReserveMore(size);

        return m_mem;
}

#endif 