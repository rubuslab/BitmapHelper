#include <stdio.h>

//
// Java 是大端字节序，第1byte认为是多字节整数的最高位（如DWORD）。32bits DWORD整数内容为‘ARGB’（'ARGB'从高位到低位表示），该整数内容在Java侧的字节流为“ARGB”。
// C++  是小端字节序，第1byte认为是多字节整数的最低位（如DWORD）。32bits DWORD整数内容为‘ARGB’（'ARGB'从高位到低位表示），该整数内容在C++ 侧的字节流为“BGRA”。
//
// https://www.jianshu.com/p/f39bbd2a2b4f
//

// https://stackoverflow.com/questions/27581750/android-capture-screen-to-surface-of-imagereader

#pragma pack(push, 1)
struct ARGB {
	unsigned char a;
	unsigned char r;
	unsigned char g;
	unsigned char b;
};

struct BGRA {
	unsigned char g;
	unsigned char r;
	unsigned char a;
	unsigned char b;
	BGRA(const ARGB& argb):
		g(argb.g),
		r(argb.r),
		a(argb.a),
		b(argb.b)
	    {
	}
};
#pragma pack(pop)

int main(int argc, char* argv[]) {
	printf("usage: %s source_java_ARGB_data_file\n\n", argv[0]);
	if (argc < 2) {
		printf("please set source ARGB data file.\n");
		return 1;
	}

	const char* src_file = argv[1];
	char filename[512] = {0};
	sprintf(filename, "%s_C_BGRA_bytes", src_file);
	const char* dst_file = (const char*)filename;

	FILE* src_fd = fopen(src_file, "rb");
	FILE* dst_fd = fopen(dst_file, "wb");
	ARGB argb;
	while (fread(&argb, sizeof(argb), 1, src_fd) == 1) {
		BGRA bgra(argb);
		int w = fwrite(&bgra, sizeof(bgra), 1, dst_fd);
	}
	fclose(src_fd);
	fclose(dst_fd);
	printf("convert to C BGRA: %s Success.\n", dst_file);

	return 0;
}