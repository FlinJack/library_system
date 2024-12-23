
#define _CRT_SECURE_NO_WARNINGS//.See online help for details.library_system	D : \desktop\ALL\ALLwork\library_system\library_system\library.c	154
#include<stdio.h>

//图书结构体
typedef struct {
    int id;
    char name[100];
    char author[100];
    int is_borrowed; //0可借，1不可借
} Book;

//用户结构体
typedef struct {
    int id;
    char name[100];
    int borrowed_book_id; //用户借图书的id
} User;

//声明函数
void load_books();
void load_users();
void save_books();
void save_users();
void admin_system();
void user_system();
void add_book();
void view_books();
void borrow_book();
void return_book();
void delete_book();
void press_to_continue();  // 新增的等待用户输入继续的函数
int get_valid_int_input(); // 输入验证函数

//全局变量储存图书和用户信息
Book books[200];
User users[200];
int book_count = 0;
int user_count = 0;

//主函数
int main() {
    load_books();
    load_users();
    int choice;
    a:
    printf("=======图书管理系统=======\n");
    printf("输入1进入管理员系统\n");
    printf("输入2进入用户系统\n");
    scanf("%d", &choice);
    if (choice == 1) {
        admin_system();
    }
    else if (choice == 2) {
        user_system();
    }
    else {
        printf("无效输入\n");
    }
    return 0;
}

//加载图书信息
void load_books() {
    char filepath[] = "D:\\c_work\\books.txt";  // 可以改成从配置文件读取路径
    FILE* file = fopen(filepath, "r");
    if (!file) {
        printf("无法打开文件 %s，请检查路径和权限！\n", filepath);
        return;
    }
    while (fscanf(file, "%d,%99[^,],%99[^,],%d\n",
        &books[book_count].id,
        books[book_count].name,
        books[book_count].author,
        &books[book_count].is_borrowed) == 4) {
        book_count++;
    }
    fclose(file);
}

//加载用户信息
void load_users() {
    FILE* file = fopen("D:\\c_work\\users.txt", "r");
    if (!file) {
        printf("无法打开文件 D:\\c_work\\users.txt ，请检查路径和权限！\n");
        return;
    }

    char line[256]; // 临时存储一行数据
    while (fgets(line, sizeof(line), file)) {
        if (user_count >= 100) {
            printf("用户数量达到上限！\n");
            break;
        }

        // 使用 sscanf 提取字段
        if (sscanf(line, "%d,%99[^,],%d",
            &users[user_count].id,
            users[user_count].name,
            &users[user_count].borrowed_book_id) == 3) {
            user_count++;
        }
        else {
            printf("解析出错，跳过这一行：%s\n", line);
        }
    }

    fclose(file);
    printf("用户信息加载完成 一共加载了 %d 个用户。\n", user_count);
}

//保存图书信息
void save_books() {
    FILE* file = fopen("D:\\c_work\\books.txt", "w");
    if (file) {
        for (int i = 0; i < book_count; i++) {
            fprintf(file, "%d,%s,%s,%d\n",
                books[i].id,
                books[i].name,
                books[i].author,
                books[i].is_borrowed);
        }
        fclose(file);
    }
}

//保存用户信息
void save_users() {
    FILE* file = fopen("D:\\c_work\\users.txt", "w");
    if (file) {
        for (int i = 0; i < user_count; i++) {
            fprintf(file, "%d,%s,%d\n",
                users[i].id,
                users[i].name,
                users[i].borrowed_book_id);
        }
        fclose(file);
    }
}

//管理员系统
void admin_system() {
    int choice;
    while (1) {  // 使用循环让管理员多次进行操作
        printf("======= 管理员系统 =======\n");
        printf("1. 添加图书\n");
        printf("2. 查看图书\n");
        printf("3. 删除图书\n");
        printf("4. 返回\n");
        printf("请输入选择: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1: add_book(); break;
        case 2: view_books(); break;
        case 3: delete_book(); break;
        case 4: return;
        default: printf("无效输入\n"); break;
        }
        press_to_continue();  // 操作后按回车继续
    }
}

//用户系统
void user_system() {
    int choice;
    while (1) {  // 使用循环让用户多次进行操作
        printf("======= 用户系统 =======\n");
        printf("1. 借阅图书\n");
        printf("2. 归还图书\n");
        printf("3. 返回\n");
        printf("请输入选择: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1: borrow_book(); break;
        case 2: return_book(); break;
        case 3: return;
        default: printf("无效输入\n"); break;
        }
        press_to_continue();  // 操作后按回车继续
    }
}

//添加图书
void add_book() {
    int id;
    printf("请输入图书编号：");
    scanf("%d", &id);
    getchar(); // 清除换行符

    // 检查图书编号是否已经存在
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == id) {
            printf("该图书编号已存在，请重新输入一个不同的编号！\n");
            return;  // 图书编号已存在，退出函数
        }
    }

    // 如果编号不存在，继续输入书名和作者
    printf("请输入书名：");
    fgets(books[book_count].name, 100, stdin);
    books[book_count].name[strcspn(books[book_count].name, "\n")] = '\0'; // 清除换行符

    printf("请输入作者：");
    fgets(books[book_count].author, 100, stdin);
    books[book_count].author[strcspn(books[book_count].author, "\n")] = '\0'; // 清除换行符

    books[book_count].id = id;
    books[book_count].is_borrowed = 0; // 默认可借
    book_count++;

    save_books();
    printf("图书添加成功！\n");
}

//删除图书
void delete_book() {
    int book_id = 0;
    printf("请输入要删除的图书编号: ");
    scanf("%d", &book_id);

    int found = 0;
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == book_id) {
            if (books[i].is_borrowed == 1) {
                printf("该图书正在被借阅，无法删除！\n");
                return; // 图书正在借阅，无法删除
            }
            found = 1;
            for (int j = i; j < book_count - 1; j++) {
                books[j] = books[j + 1];
            }
            book_count--;
            printf("图书删除成功！\n");
            break;
        }
    }
    if (!found) {
        printf("未找到该图书！\n");
    }
}

//查看所有图书
void view_books() {
    for (int i = 0; i < book_count; i++) {
        printf("图书编号：%d,图书名：%s,作者：%s,是否可借：%s\n",
            books[i].id,
            books[i].name,
            books[i].author,
            books[i].is_borrowed ? "不可借" : "可借");
    }
}

//借阅图书
void borrow_book() {
    int user_id, book_id;
    printf("请输入用户 ID：");
    user_id = get_valid_int_input();
    printf("请输入想借阅的图书 ID：");
    book_id = get_valid_int_input();

    int found_user = 0, found_book = 0;
    for (int i = 0; i < user_count; i++) {
        if (users[i].id == user_id) {
            found_user = 1;
            if (users[i].borrowed_book_id != 0) {
                printf("用户已经借了一本书，无法再借！当前已借阅的图书 ID 是：%d\n",
                    users[i].borrowed_book_id);
                return;
            }
            break;
        }
    }
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == book_id) {
            found_book = 1;
            if (books[i].is_borrowed) {
                printf("该图书已经被借阅，无法借阅！\n");
                return;
            }
            books[i].is_borrowed = 1;
            for (int j = 0; j < user_count; j++) {
                if (users[j].id == user_id) {
                    users[j].borrowed_book_id = book_id;
                    printf("借阅成功！\n");
                    save_books();
                    save_users();
                    return;
                }
            }
        }
    }

    if (!found_user) {
        printf("未找到该用户！\n");
    }
    if (!found_book) {
        printf("未找到该图书！\n");
    }
}

//归还图书
void return_book() {
    int user_id, book_id;
    printf("请输入用户 ID：");
    user_id = get_valid_int_input();
    printf("请输入归还的图书 ID：");
    book_id = get_valid_int_input();

    int found_user = 0, found_book = 0;
    for (int i = 0; i < user_count; i++) {
        if (users[i].id == user_id) {
            found_user = 1;
            if (users[i].borrowed_book_id != book_id) {
                printf("该用户并未借阅此书！\n");
                return;
            }
            break;
        }
    }

    for (int i = 0; i < book_count; i++) {
        if (books[i].id == book_id) {
            found_book = 1;
            books[i].is_borrowed = 0;
            printf("归还成功！\n");

            for (int j = 0; j < user_count; j++) {
                if (users[j].id == user_id) {
                    users[j].borrowed_book_id = 0;
                    save_books();
                    save_users();
                    return;
                }
            }
        }
    }

    if (!found_user) {
        printf("未找到该用户！\n");
    }
    if (!found_book) {
        printf("未找到该图书！\n");
    }
}

//等待用户按回车继续
void press_to_continue() {
    printf("操作完成，按回车继续...\n");
    getchar();  // 清除输入缓冲区中的换行符
    getchar();  // 等待用户按回车
}

//输入整数并确保合法
int get_valid_int_input() {
    int input;
    while (scanf("%d", &input) != 1) {
        while (getchar() != '\n');  // 清空输入缓冲区
        printf("无效输入，请重新输入: ");
    }
    return input;
}
