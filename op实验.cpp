#include <iostream>
#include <vector>
#include <string>
#include<stdlib.h>
#include <cstdio>
#include <deque>
#include <time.h>

using namespace std;

int input_num() {
    string num;
    int t = 1;
    cin >> num;
    if (num[0]=='0'||num[0]=='-') {
        t = 0;
    }
    for (size_t i = 0; i < num.length()&&t==0; i++)
    {
        if (num[i] >= '0' && num[i] <= '9')
            continue;
        else
        {
            t = 0;
            break;
        }
    }

    if (t == 0) {
        cout<<"输入不合法，请重新输入" << endl;
        input_num();
    }

    return stoi(num);
}

int search(int n, deque<int> &storage) {
    for (int i = 0; i < storage.size(); i++) {
        if (n==storage[i]) {
            return 1;
        }
    }
    return 0;
}

int search(int n, vector<int>& storage) {
    for (int i = 0; i < storage.size(); i++) {
        if (n == storage[i]) {
            return 1;
        }
    }
    return 0;
}

int Random(int start, int end) {
    int dis = end - start;
    return rand() % dis + start;
}

int main()
{
    int page_num;
    int storage_cube_num;
    int num1;
    string exit;
    cout<<"欢迎使用本程序，本程序用于模拟页面置换算法的实现"<< endl;
    cout << "输入 1：执行算法" << endl;
    cout << "输入 0：退出" << endl;
    cout << "请输入" << endl;
    cin >> exit;

    while (exit[0] != '1' && exit[0] != '0'||exit.size()!=1) {
        cout << "输入不合法，请重新输入" << endl;
        cin >> exit;
    }

    while (exit[0] == '1') {
        cout << "-----------------------------------" << endl;


        cout << "请输入页面数" << endl;
        page_num= input_num();


        cout << "请输入内存块数量" << endl;
        storage_cube_num= input_num();

        cout << "请输入页面序列的大小" << endl;
        num1 = input_num();
        
        deque<int> storage(storage_cube_num);
        vector<int> page_list(num1);
        double change = 0;

        for (int i = 0; i < num1;i++) {//生成随机页面序列 范围1~Page_num
            page_list[i] = Random(1, page_num+1);
        }

        cout << "随机序列如下" << endl;
        for (int i = 0; i < num1; i++) {
            cout << page_list[i] << " ";
        }
        cout << endl;
        //FIFO
        cout << "FIFO（先进先出）算法执行结果" << endl;
        cout << "页号 ";
        cout << "内存块";

        for (int i = 0; i < storage_cube_num; i++) {
            cout << " ";
        }
        cout << "是否缺页 ";
        cout << "替换掉的页号" << endl;
        for (int i = 0; i < num1; i++) {
            bool s = 0;
            int num = 0;
            if (storage.size()< storage_cube_num) {//内存未满
                storage.push_back(page_list[i]);
                change++;
                s = 1;
            }
            else {
                if (!search(page_list[i],storage)) {//若发生缺页，则将队列顶的内容POP掉
                    change++;
                    num = storage[0];
                    storage.pop_front();
                    storage.push_back(page_list[i]);
                    s = 1;
                }             
            }
            cout<< page_list[i] <<"   ";
            for (int j = 0; j < storage_cube_num;j++) {
               if(j< storage.size()|| storage[j])
                  cout << " " << storage[j];
               else {
                   cout << " -";
               }
            }

            for (int j = 0; j < storage_cube_num; j++) {
                cout << "  ";
            }
            cout<< boolalpha<<s;
            if (num) {
                cout <<"     " << num << endl;
            }
            else {
                cout << "     -"<<endl;
            }
        }
        cout << endl;
        cout << "FIFO算法 缺页次数: "<<change << "   缺页率: "<<change/num1 << endl;
        cout << endl;
        //OPT
        cout << "OPT（理想型淘汰算法）算法执行结果" << endl;
        change = 0;
        cout << "页号 ";
        cout << "内存块";
        storage.clear();
        for (int i = 0; i < storage_cube_num; i++) {
            cout << " ";
        }
        cout << "是否缺页 ";
        cout << "替换掉的页号" << endl;
        for (int i = 0; i < num1; i++) {
            bool s = 0;
            int num = 0;
            if (storage.size() < storage_cube_num&&!search(page_list[i], storage)) {//内存未满
                storage.push_back(page_list[i]);
                change++;
                s = 1;
            }
            else {
                if (!search(page_list[i], storage)) {//若发生缺页，则寻找将来最晚或者永不使用的页面
                    change++;
                    s = 1;
                    int pre_index = 0;
                    if (i==num1-1) {
                        num = storage[storage_cube_num - 1];
                        storage[storage_cube_num-1] = page_list[i];                       
                    }
                    else {
                        vector<int> re;
                        for (int j = i + 1; j < num1; j++) {
                            if (re.size()< storage_cube_num&&search(page_list[j], storage)&&!search(page_list[j], re)) {
                                re.push_back(page_list[j]);
                            }
                        }
                        if (re.size() < storage_cube_num) {//如果存在将来永不使用的页面
                            for (int j = 0; j < storage_cube_num; j++) {
                                if (!search(storage[j],re)) {
                                    num = storage[j];
                                    pre_index = j;
                                    break;
                                }
                            }
                            for (int j = pre_index; j < storage_cube_num - 1; j++) {
                                storage[j] = storage[j + 1];
                            }
                            storage[storage_cube_num - 1] = page_list[i];
                        }
                        else {
                            num = re[re.size() - 1];//否则选取最晚使用的页面
                            for (int j = 0; j < storage_cube_num; j++) {
                                if (storage[j] == num) {
                                    pre_index = j;
                                    break;
                                }
                            }
                            for (int j = pre_index; j < storage_cube_num - 1; j++) {
                                storage[j] = storage[j + 1];
                            }
                            storage[storage_cube_num - 1] = page_list[i];
                        }
                    }
                }
            }
            cout << page_list[i] << "   ";
            for (int j = 0; j < storage_cube_num; j++) {
                if (j < storage.size())
                    cout << " " << storage[j];
                else {
                    cout << " -";
                }
            }

            for (int j = 0; j < storage_cube_num; j++) {
                cout << "  ";
            }
            cout << boolalpha << s;
            if (num) {
                cout << "     " << num << endl;
            }
            else {
                cout << "     -" << endl;
            }
        }
        cout << endl;
        cout << "OPT算法 缺页次数: " << change << "   缺页率: " << change / num1 << endl;
        cout << endl;

        //LRU
        cout << "LRU（最近最久未使用）算法执行结果" << endl;
        change = 0;
        storage.clear();
        cout << "页号 ";
        cout << "内存块";
        for (int i = 0; i < storage_cube_num; i++) {
            cout << " ";
        }
        cout << "是否缺页 " ;
        cout << "替换掉的页号" << endl;
        for (int i = 0; i < num1; i++) {
            bool s = 0;
            int num = 0;
            if (storage.size() < storage_cube_num && !search(page_list[i], storage)) {//内存未满
                storage.push_back(page_list[i]);
                change++;
                s = 1;
            }
            else {
                if (!search(page_list[i], storage)) {//若发生缺页，则向前寻找最晚或者永不使用的界面
                    change++;
                    s = 1;
                    vector<int> re;
                    int pre_index = 0;
                    for (int j = i - 1; j >= 0; j--) {
                        if (re.size() < storage_cube_num && search(page_list[j], storage) && !search(page_list[j], re)) {
                            re.push_back(page_list[j]);
                        }
                    }

                    if (re.size() < storage_cube_num) {//如果存在历史前永不使用的页面
                        for (int j = 0; j < storage_cube_num; j++) {
                            if (!search(storage[j], re)) {
                                num = storage[j];
                                pre_index = j;
                                break;
                            }
                        }
                        for (int j = pre_index; j < storage_cube_num - 1; j++) {
                            storage[j] = storage[j + 1];
                        }
                        storage[storage_cube_num - 1] = page_list[i];
                    }
                    else {
                        num = re[re.size() - 1];//否则选取历史访问记录最晚使用的页面
                        for (int j = 0; j < storage_cube_num; j++) {
                            if (storage[j] == num) {
                                pre_index = j;
                                break;
                            }
                        }
                        for (int j = pre_index; j < storage_cube_num - 1; j++) {
                            storage[j] = storage[j + 1];
                        }
                        storage[storage_cube_num - 1] = page_list[i];
                    }

                }
            }
            cout << page_list[i] << "   ";
            for (int j = 0; j < storage_cube_num; j++) {
                if (j < storage.size())
                    cout << " " << storage[j];
                else {
                    cout << " -";
                }
            }

            for (int j = 0; j < storage_cube_num; j++) {
                cout << "  ";
            }
            cout << boolalpha << s;
            if (num) {
                cout << "     " << num << endl;
            }
            else {
                cout << "     -" << endl;
            }
        }
        cout << endl;
        cout << "LRU算法 缺页次数: " << change << "   缺页率: " << change / num1 << endl;
        cout << endl;


        cout << "-----------------------------------" << endl;
        cout << "算法模拟完毕，请输入操作" << endl;
        cout << "输入 1：执行算法" << endl;
        cout << "输入 0：退出" << endl;
        cout << "请输入" << endl;
        cin >> exit;
        while (exit[0] != '1' && exit[0] != '0' || exit.size() != 1) {
            cout << "输入不合法，请重新输入" << endl;
            cin >> exit;
        }

    }
    cout << "已退出程序" << endl;
    return 0;
}