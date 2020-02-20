/*
 * C++��ȡ�ļ��ĸ��ַ��������Ƚϸ����������ٶȡ�
 * �ο���http://insanecoding.blogspot.com/2011/11/how-to-read-in-file-in-c.html
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
 * ʹ��C���Ժ�����ȡ�ļ���
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
 * ʹ��C++IO����ȡ�ļ�
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
 * ʹ��C++ rdbuf��ʽ��ȡ�ļ���
 * rdbuf��������������Ļ�����ָ�룬����������ͨ������<<������ȡ�����������ݡ�
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
 * ʹ��C++ copy������ȡ�ļ����ݡ�
 * ���ַ�ʽ�ĵײ㴦��ʽ���ǣ��Ƚ����ݶ��뻺������Ȼ�����ַ����Ƶ�Ŀ���ڴ����������˵���ǱȽ�����һ�ַ�ʽ��
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
 * ʹ��C++ string��assign������ȡ�ļ����ݡ�
 * ԭ����copy������ͬ�����������ٶ���copyӦ�ò�ࡣ
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
 * ʹ��C++ iterator������ȡ�ļ����ݡ�
 * ԭ����copy������ͬ�����������ٶ���copyӦ�ò�ࡣ
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