//#include <iostream>
//#include <string>

//using namespace std;


//string longNumOfString(string str) {
    //int l = 0, r = 0;

    //string res = "", digit = "";
    //int record = 0, cur = 0;
    //while (r < str.size()) {
        //if (isdigit(str[r])) {
            //digit += str[r++];
            //cur++;
        //}
        //else {
            //l = ++r;
            //if (cur >= record) {
                //record = cur;
                //res = digit;
            //}
            //digit = "";
            //cur = 0;
        //}
    //}
    //if (cur >= record) {
        //record = cur;
        //res = digit;
    //}

    //return res + "," + to_string(record);
//}
  
  

//int main() {

    ////string str = "abcd123d125ss12";
    ////cout << longNumOfString(str) << endl;
    
    //string str;
    //while (cin >> str)
        //cout << longNumOfString(str) << endl;
//}
//
#include <iostream>
#include <vector>
#include <string>
using namespace std;
//大数相乘
/** *num1 乘数1 *num2 乘数2 *return 结果字符串 */
string BigMutiple(string num1, string num2){

    string res="";
    //两个数的位数
    int m = num1.size(), n = num2.size();

    //一个i位数乘以一个j位数，结果至少是i+j-1位数
    vector<long long> tmp(m + n - 1);

    //每一位进行笛卡尔乘法
    for (int i = 0; i < m; i++){
         int a = num1[i] - '0';
        for (int j = 0; j < n; j++){
            int b = num2[j] - '0';
            tmp[i + j] += a*b;
        }
    }
    //进行进位处理，注意左侧是大右侧是小
    int carry = 0;
    for (int i = tmp.size() - 1; i >= 0; i--){
        int t = tmp[i] + carry;
        tmp[i] = t % 10;
        carry = t / 10;
    }
    //若遍历完仍然有进位
    while (carry != 0){
        int t = carry % 10;
        carry /= 10;
        tmp.insert(tmp.begin(), t);
    }
    //将结果存入到返回值中
    for (auto a : tmp){
        res = res + to_string(a);
    }
    if(res.size()>0&&res[0]=='0')return "0";
    return res;

}

////测试函数
//int main(){
    //string num1, num2;
    //while (cin >> num1 >> num2){
        //cout << BigMutiple(num1, num2) << endl;
    //}
    //return 0;
//}
int main(){
	string num1, num2;
	cin >> num1 >> num2;
	int flag = 0;

	if (num1[0] == '-'){
		num1.erase(0, 1);
		flag++;
	}

	if (num2[0] == '-'){
		num2.erase(0, 1);
		flag++;
	}

	if (flag == 1){
		cout << '-' << BigMutiple(num1, num2) << endl;
	}
	else{
		cout << BigMutiple(num1, num2) << endl;
	}
	return 0;
}
