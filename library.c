#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 图书结构体
typedef struct {
    int id;
    char name[100];
    char author[100];
    int is_borrowed;  // 1: 已借, 0: 可借
} Book;

// 用户结构体
typedef struct {
    int id;
    char name[100];
    int borrowed_book_id;  // 用户借的图书ID
} User;

// 函数声明
void admin_system();
void user_system();
void add_book();
void view_books();
void borrow_book();
void return_book();
void delete_book();
void load_books();
void load_users();
void save_books();
void save_users();


// 全局变量存储图书和用户信息
Book books[100];
User users[100];
int book_count = 0;
int user_count = 0;

// 主函数
int main() {
    // 加载文件中的数据
    load_books();
    load_users();

    int choice;
    printf("======= 图书管理系统 =======\n");
    printf("1. 进入管理员系统\n");
    printf("2. 进入用户系统\n");
    printf("请输入选择: ");
    scanf("%d", &choice);

    if (choice == 1) {
        admin_system();
    } else if (choice == 2) {
        user_system();
    } else {
        printf("无效选择！\n");
    }

    return 0;
}

// 加载图书信息
void load_books() {
    FILE *file = fopen("books.txt", "r");
    if (file) {
        while (fscanf(file, "%d, %99[^,], %99[^,], %d\n", 
                        &books[book_count].id, 
                        books[book_count].name, 
                        books[book_count].author, 
                        &books[book_count].is_borrowed) != EOF) {
            book_count++;
        }
        fclose(file);
    }
}

// 加载用户信息
void load_users() {
    FILE *file = fopen("users.txt", "r");
    if (file) {
        while (fscanf(file, "%d, %99[^,], %d\n", 
                        &users[user_count].id, 
                        users[user_count].name, 
                        &users[user_count].borrowed_book_id) != EOF) {
            user_count++;
        }
        fclose(file);
    }
}

// 保存图书信息
void save_books() {
    FILE *file = fopen("books.txt", "w");
    if (file) {
        for (int i = 0; i < book_count; i++) {
            fprintf(file, "%d, %s, %s, %d\n", 
                    books[i].id, books[i].name, books[i].author, books[i].is_borrowed);
        }
        fclose(file);
    }
}

// 保存用户信息
void save_users() {
    FILE *file = fopen("users.txt", "w");
    if (file) {
        for (int i = 0; i < user_count; i++) {
            fprintf(file, "%d, %s, %d\n", 
                    users[i].id, users[i].name, users[i].borrowed_book_id);
        }
        fclose(file);
    }
}

// 管理员系统
void admin_system() {
    int choice;
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
        default: printf("无效选择！\n");
    }
}

// 用户系统
void user_system() {
    int choice;
    printf("======= 用户系统 =======\n");
    printf("1. 借阅图书\n");
    printf("2. 归还图书\n");
    printf("3. 查看借阅记录\n");
    printf("4. 返回\n");
    printf("请输入选择: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1: borrow_book(); break;
        case 2: return_book(); break;
        case 3: printf("查看借阅记录功能暂未实现。\n"); break;
        case 4: return;
        default: printf("无效选择！\n");
    }
}

// 添加图书
void add_book() {
    printf("请输入图书编号: ");
    scanf("%d", &books[book_count].id);
    getchar();  // 清除缓冲区的换行符
    printf("请输入图书名称: ");
    fgets(books[book_count].name, 100, stdin);
    printf("请输入图书作者: ");
    fgets(books[book_count].author, 100, stdin);
    books[book_count].is_borrowed = 0;  // 默认图书可借

    book_count++;
    save_books();  // 保存图书信息到文件
    printf("图书添加成功！\n");
}

// 查看所有图书
void view_books() {
    for (int i = 0; i < book_count; i++) {
        printf("图书编号: %d, 名称: %s, 作者: %s, %s\n",
               books[i].id, books[i].name, books[i].author,
               books[i].is_borrowed ? "已借出" : "可借");
    }
}

// 借书
void borrow_book() {
    int user_id, book_id;
    printf("请输入用户ID: ");
    scanf("%d", &user_id);
    printf("请输入想借阅的图书编号: ");
    scanf("%d", &book_id);

    // 查找图书
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == book_id && books[i].is_borrowed == 0) {
            books[i].is_borrowed = 1;

            // 查找用户
            for (int j = 0; j < user_count; j++) {
                if (users[j].id == user_id) {
                    users[j].borrowed_book_id = book_id;
                    save_books();  // 保存图书信息
                    save_users();  // 保存用户信息
                    printf("借阅成功！\n");
                    return;
                }
            }
            printf("用户不存在！\n");
            return;
        }
    }
    printf("图书不可借或不存在！\n");
}

// 还书
void return_book() {
    int user_id, book_id;
    printf("请输入用户ID: ");
    scanf("%d", &user_id);
    printf("请输入归还的图书编号: ");
    scanf("%d", &book_id);

    // 查找用户
    for (int i = 0; i < user_count; i++) {
        if (users[i].id == user_id && users[i].borrowed_book_id == book_id) {
            users[i].borrowed_book_id = 0;

            // 查找图书
            for (int j = 0; j < book_count; j++) {
                if (books[j].id == book_id) {
                    books[j].is_borrowed = 0;
                    save_books();  // 保存图书信息
                    save_users();  // 保存用户信息
                    printf("图书已归还！\n");
                    return;
                }
            }
            printf("图书不存在！\n");
            return;
        }
    }
    printf("用户没有借此图书！\n");
}

// 删除图书
void delete_book() {
    int book_id;
    printf("请输入要删除的图书ID: ");
    scanf("%d", &book_id);

    int found = 0;  // 用来判断图书是否找到
    for (int i = 0; i < book_count; i++) {
        if (books[i].id == book_id) {
            found = 1;
            // 删除图书，后面的元素前移
            for (int j = i; j < book_count - 1; j++) {
                books[j] = books[j + 1];
            }
            book_count--;  // 更新图书数量
            printf("图书已删除！\n");
            break;
        }
    }
    if (!found) {
        printf("未找到该图书！\n");
    }
}

