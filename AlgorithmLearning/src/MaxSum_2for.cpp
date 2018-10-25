#include<stdio.h>  
#include<string.h>  
#include<cmath>  
#include<iostream>  
using namespace std;

int arr[1000 + 5];
int main(){
	//freopen("input", "r", stdin);
	int n = 0;
	while (1 == scanf("%d", &n)){
		int leftSub = 0, rightSub = 0;
		int sum = 0, maxSum = 0;
		
		for (int i = 0; i < n; ++i){
			scanf("%d", arr + i);
		}
		for (int i = 0; i < n; ++i){
			sum = 0;
			for (int j = i; j < n; ++j){
				sum += arr[j];
				if (sum > maxSum){
					maxSum = sum;
					leftSub = i;
					rightSub = j;
				}
			}
		}
		printf("From=%d,To=%d\nMaxSum=%d\n", leftSub + 1, rightSub + 1, maxSum > 0 ? maxSum : 0);
	}
}
