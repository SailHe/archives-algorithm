#include<stdio.h>
#include<string.h>

int mainForArgcTest(int argc, char *argv[]){
	if (argc < 2) {
		fprintf(stderr, "请输入ip参数, 如:test 192.168.1.1.\n");
		return 1;
	}
	//后面对ip有效性进行判断
	printf("ip=%s\n", argv[1]);

	return 0;
}

// 为url中的&添加单引号 -> '&'
int main(int argc, char const *argv[]) {
	//int main(){
		// int argc = 1;
		// char const *argv[] = { "&123&45&" };
	int result = -1;
	// main_ForTest();
	if (argc != 2) {
		fprintf(stderr, "请输入url参数, 如:appName bala&bala.com\n");
		result = -1;
	}
	else {
		result = 0;
		auto url = argv[1];
		int len = strlen(url);
		putchar(len > 0 && url[0] == '&' ? '\'' : '\0');
		for (int i = 0; i < len; ++i) {
			bool rhsQuete = (i > 0 && url[i - 1] == '&');
			bool lhsQuete = (i < len - 1 && url[i + 1] == '&');
			if (rhsQuete) {
				putchar('\'');
				putchar(url[i]);
			}
			else if (lhsQuete) {
				putchar(url[i]);
				putchar('\'');
			}
			else {
				putchar(url[i]);
			}
		}
		putchar(len > 0 && url[len - 1] == '&' ? '\'' : '\0');
	}
	return result;
}

