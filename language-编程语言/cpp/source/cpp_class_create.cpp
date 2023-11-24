#include <stdio.h>
#include <stdlib.h>

class father
{
public:
	father()
	{
		printf("father()\r\n");
		throw 1;
	};
	~father()
	{
		printf("~father()\r\n");
	};

	void* operator new(size_t size)
	{
		printf("father new()\r\n");
		return malloc(size);
	};

	void operator delete(void* p)
	{
		printf("father delete()\r\n");
		free(p);
	};

private:

};

class son : public father
{
public:
	son()
	{
		printf("son()\r\n");
	};
	~son()
	{
		printf("~son()\r\n");
	};

	void* operator new(size_t size)
	{
		printf("son new()\r\n");
		return malloc(size);
	};

	void operator delete(void* p)
	{
		printf("son delete()\r\n");
		free(p);
	};

private:

};

int main(void)
{
	try
	{
		printf("hello the world\r\n");
		printf("---------------\r\n");
		printf("---------------\r\n");
		father* f1 = new father;
		delete f1;
		printf("---------------\r\n");
		son* s1 = new son;
		delete s1;
		printf("---------------\r\n");
		printf("---------------\r\n");
		father f2 = father();
		printf("---------------\r\n");
		son s2 = son();
		printf("---------------\r\n");
		printf("---------------\r\n");
		return 0;
	}
	catch (int e)
	{
		printf("err:%d", e);
	}
}
