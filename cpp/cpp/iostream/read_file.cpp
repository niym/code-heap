/*
 * C++读取文件的各种方法，并比较各个方法的速度。
 * 参考：http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
 */
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <iterator>
#include <chrono>


/**
 * 使用C语言函数读取文件。
 */
std::string get_file_contents_by_c(const char *filename)
{
	std::FILE* fp = std::fopen(filename, "rb");
	if (fp)
	{
		std::string contents;
		std::fseek(fp, 0, SEEK_END);
		contents.resize(std::ftell(fp));
		std::rewind(fp);
		std::fread(&contents[0], 1, contents.size(), fp);
		std::fclose(fp);
		return contents;
	}
	return "";
}

/**
 * 使用C++IO流读取文件
 */
std::string get_file_contents_by_cpp(const char *filename)
{
	std::ifstream ifs(filename, std::ios::binary);
	if (ifs)
	{
		std::string contents;
		ifs.seekg(0, ifs.end);
		contents.resize(ifs.tellg());
		ifs.seekg(0, ifs.beg);
		ifs.read(&contents[0], contents.size());
		ifs.close();
		return contents;
	}
	return "";
}

/**
 * 使用C++ rdbuf方式读取文件。
 * rdbuf函数返回流对象的缓冲区指针，输入流对象通过重载<<操作读取缓冲区的数据。
 */
std::string get_file_contents_by_rdbuf(const char *filename)
{
	std::ifstream ifs(filename, std::ios::binary);
	if (ifs)
	{
		std::ostringstream oss;
		oss << ifs.rdbuf();
		ifs.close();
		return oss.str();
	}
	return "";
}

/**
 * 使用C++ copy函数读取文件数据。
 * 这种方式的底层处理方式就是：先将数据读入缓冲区，然后逐字符复制到目标内存区域。相对来说，是比较慢的一种方式。
 */
std::string get_file_contents_by_stdcopy(const char* filename)
{
	std::ifstream ifs(filename, std::ios::binary);
	if (ifs)
	{
		std::string contents;
		ifs.seekg(0, ifs.end);
		contents.resize(ifs.tellg());
		ifs.seekg(0, ifs.beg);
		
		std::copy(std::istreambuf_iterator<char>(ifs),
			std::istreambuf_iterator<char>(),
			std::back_inserter(contents));

		ifs.close();
		return contents;
	}
	return "";
}

/**
 * 使用C++ string的assign函数读取文件数据。
 * 原理与copy基本相同，所以运行速度与copy应该差不多。
 */
std::string get_file_contents_by_assign(const char* filename)
{
	std::ifstream ifs(filename, std::ios::binary);
	if (ifs)
	{
		std::string contents;
		ifs.seekg(0, ifs.end);
		contents.resize(ifs.tellg());
		ifs.seekg(0, ifs.beg);

		contents.assign(std::istreambuf_iterator<char>(ifs),
			std::istreambuf_iterator<char>());

		ifs.close();
		return contents;
	}
	return "";
}

/**
 * 使用C++ iterator函数读取文件数据。
 * 原理与copy基本相同，所以运行速度与copy应该差不多。
 */
std::string get_file_contents_by_iterator(const char* filename)
{
	std::ifstream ifs(filename, std::ios::binary);
	if (ifs)
	{
		return std::string(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>());
	}
	return "";

	
}

#define TEST_READ_FILE_SPEED(FUNC, FILENAME) \
do\
{\
	auto start_time = std::chrono::high_resolution_clock::now();\
	(FUNC)((FILENAME));\
	auto end_time = std::chrono::high_resolution_clock::now();\
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);\
	std::cout << #FUNC" takes " << duration.count() << "ms" << std::endl;\
} while (0)

int main()
{
	TEST_READ_FILE_SPEED(get_file_contents_by_assign, "...");
	TEST_READ_FILE_SPEED(get_file_contents_by_iterator, "...");
	TEST_READ_FILE_SPEED(get_file_contents_by_cpp, "...");
	TEST_READ_FILE_SPEED(get_file_contents_by_c, "...");
	TEST_READ_FILE_SPEED(get_file_contents_by_rdbuf, "...");
	TEST_READ_FILE_SPEED(get_file_contents_by_stdcopy, "...");
	return 0;
}