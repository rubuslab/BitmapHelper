#include <stdio.h>

int GetFileSize(FILE* stream) {
	int curpos = 0;
	int length = 0;
	curpos = ftell(stream);
	fseek(stream, 0L, SEEK_END);
	length = ftell(stream);
	fseek(stream, curpos, SEEK_SET);
	return length;
}

bool AppendToFile(const char* src_file, const char* dst_file) {
	FILE* dst_fd = fopen(dst_file, "ab+");
	if (dst_fd == NULL) {
		printf("try open dst file: %s Failed.\n", dst_file);
		return false;
	}

	FILE* src_fd = fopen(src_file, "rb");
	if (src_fd == NULL) {
		printf("try open source file: %s Failed.\n", src_file);
		fclose(dst_fd);
		return false;
	}

	int src_len = GetFileSize(src_fd);
	unsigned char* buff = new unsigned char[src_len];
	int r = fread(buff, src_len, 1, src_fd);
	fseek(dst_fd, 0, SEEK_END);
	int w = fwrite(buff, src_len, 1, dst_fd);
	fclose(src_fd);
	fclose(dst_fd);
	delete[] buff;

	bool success = (r == 1 && w == 1);
	return success;
}

int main(int argc, char* argv[]) {
	printf("usage: %s source_file1 source_file2 dst_file\n", argv[0]);
	if (argc < 4) return 1;
	const char* src_file1 = argv[1];
	const char* src_file2 = argv[2];
	const char* dst_file = argv[3];

	// append source file1 to dst file
	bool appended = AppendToFile(src_file1, dst_file);
	if (!appended) {
		printf("Try append source file: %s to dst file failed.\n", src_file1);
		return 1;
	}

	// append source file1 to dst file
	appended = AppendToFile(src_file2, dst_file);
	if (!appended) {
		printf("Try append source file: %s to dst file failed.\n", src_file2);
		return 1;
	}

	printf("\nmerge %s %s to file: %s Successed.\n", src_file1, src_file2, dst_file);
	return 0;
}