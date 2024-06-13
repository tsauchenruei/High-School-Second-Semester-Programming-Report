#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
// 設定邊界 
#define MAP_WIDTH 40
#define MAP_HEIGHT 20
#define PIC_WIDTH 80

// 設定各種數據 
char *pic[PIC_WIDTH][MAP_HEIGHT];
int map[MAP_WIDTH][MAP_HEIGHT];
int usergps[2] = {20, 10};
int monsterGps[2] = {-1, -1}; // 怪物初始位置為無效位置，只有在第二地圖才放置有效位置
int data = 0;
int difficulty = 0;
int door_opened = 0; // 們是否有開過 
HANDLE monsterThreadHandle; // 怪物執行續 
int gameRunning = 1; // 遊戲狀態 
#define WHITE "\x1B[0;37m"
#define KDRK "\x1B[0;31m"
// 邊框初始化 
void initializeMapBorders() {
    int x, y;
    for (x = 0; x < MAP_WIDTH; x++) {
        for (y = 0; y < MAP_HEIGHT; y++) {
            map[x][y] = 0; // 清除地圖所有資料 
        }
    }
    
    //設邊界 
    for (x = 0; x < MAP_WIDTH; x++) {
        map[x][0] = 1;
        map[x][MAP_HEIGHT - 1] = 1;
    }
    for (y = 0; y < MAP_HEIGHT; y++) {
        map[0][y] = 1;
        map[MAP_WIDTH - 1][y] = 1;
    }
}

// 初始化地圖圖形
void initializeMapGraphics() {
    initializeMapBorders();
    int x, y;
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < PIC_WIDTH; x++) {
            pic[x][y] = (char *)malloc(3 * sizeof(char));
            strcpy(pic[x][y], "  ");
        }
    }

    for (x = 0; x < MAP_WIDTH; x++) {
        strcpy(pic[x * 2][0], "牆");
        strcpy(pic[x * 2][MAP_HEIGHT - 1], "牆");
    }
    for (y = 0; y < MAP_HEIGHT; y++) {
        strcpy(pic[0][y], "牆");
        strcpy(pic[(MAP_WIDTH - 1) * 2][y], "牆");
    }
}

// 列印地圖(玩家 怪物) 
void printMap() {
    system("cls");
    int x, y;
    
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            if (x == usergps[0] && y == usergps[1]) {
                printf("我");
            } else if (x == monsterGps[0] && y == monsterGps[1] && monsterGps[0] != -1 && monsterGps[1] != -1) {
            	printf(KDRK"怪");
            	printf(WHITE"");
            } else {
                printf("%s", pic[x * 2][y]);
            }
        }
        printf("\n");
    }
}

// 列印地圖資料功能
void printMapData() {
    int x, y;
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            printf("%d", map[x][y]);
        }
        printf("\n");
    }
}

// 不可過
void setGeneralData(char *symbol, int x, int y) {
    map[x][y] = 1;
    strncpy(pic[x * 2][y], symbol, 2);
}

// 故事線 
void setTriggerData(char *symbol, int x, int y) {
    map[x][y] = 2;
    strncpy(pic[x * 2][y], symbol, 2);
}

// 門
void setDoor(char *symbol, int x, int y) {
    map[x][y] = 3;
    strncpy(pic[x * 2][y], symbol, 2);
}

//床 
void setbed(char *symbol, int x, int y) {
    map[x][y] = 4;
    strncpy(pic[x * 2][y], symbol, 2);
}
//壁爐 
void setfireplace(char *symbol, int x, int y) {
    map[x][y] = 5;
    strncpy(pic[x * 2][y], symbol, 2);
}
// 是否在壁爐旁 
int isNearfireplace() {
    int x = usergps[0];
    int y = usergps[1];
    return (map[x - 1][y] == 5 || map[x + 1][y] == 5 || map[x][y - 1] == 5 || map[x][y + 1] == 5);
}
// 是否在床旁 
int isNearbed() {
    int x = usergps[0];
    int y = usergps[1];
    return (map[x - 1][y] == 4 || map[x + 1][y] == 4 || map[x][y - 1] == 4 || map[x][y + 1] == 4);
}
// 確認是否在門旁 
int isNearDoor() {
    int x = usergps[0];
    int y = usergps[1];
    return (map[x - 1][y] == 3 || map[x + 1][y] == 3 || map[x][y - 1] == 3 || map[x][y + 1] == 3);
}

// 是否在物件旁 
int isNearInteractable() {
    int x = usergps[0];
    int y = usergps[1];
    return (map[x - 1][y] == 2 || map[x + 1][y] == 2 || map[x][y - 1] == 2 || map[x][y + 1] == 2);
}
void interactWithbed() {
    int x = usergps[0];
    int y = usergps[1];
    
    if (map[x - 1][y] == 4) {
        printf("一個普通的床，但...\n");
    } else if (map[x + 1][y] == 4) {
        printf("一個普通的床，但...\n");
    } else if (map[x][y - 1] == 4) {
        printf("一個普通的床，但...\n");
    } else if (map[x][y + 1] == 4) {
        printf("一個普通的床，但...\n");
    }
}
void interactWithfireplace() {
    int x = usergps[0];
    int y = usergps[1];
    
    if (map[x - 1][y] == 5) {
        printf("你是有多缺愛\n我不缺男/女友(y) 我很缺男/女友(n)\n");
        char choice = getch();
        if (choice == 'y' || choice == 'Y') {
            printf("讓我先哭一會兒by:曹\n");
            Sleep(3);
            printf("恭喜你難度x2\n");
            difficulty = 2;
        } else if (choice == 'n' || choice == 'N') {
            printf("因為你和沒髮程式設計師一樣 \nso 難度驟降by:曹\n");
            difficulty = 1;
        }
    } else if (map[x + 1][y] == 5) {
        printf("你是有多缺愛\n我不缺男/女友(y) 我很缺男/女友(n)\n");
        char choice = getch();
        if (choice == 'y' || choice == 'Y') {
            printf("讓我先哭一會兒by:曹\n");
            Sleep(3);
            printf("恭喜你難度x2\n");
            difficulty = 2;
        } else if (choice == 'n' || choice == 'N') {
            printf("因為你和沒髮程式設計師一樣 \nso 難度驟降by:曹\n");
            difficulty = 1;
        }
    } else if (map[x][y - 1] == 5) {
        printf("你是有多缺愛\n我不缺男/女友(y) 我很缺男/女友(n)\n");
        char choice = getch();
        if (choice == 'y' || choice == 'Y') {
            printf("讓我先哭一會兒by:曹\n");
            Sleep(3);
            printf("恭喜你難度x2\n");
            difficulty = 2;
        } else if (choice == 'n' || choice == 'N') {
            printf("因為你和沒髮程式設計師一樣 \nso 難度驟降by:曹\n");
            difficulty = 1;
        }
    } else if (map[x][y + 1] == 5) {
        printf("你是有多缺愛\n我不缺男/女友(y) 我很缺男/女友(n)\n");
        char choice = getch();
        if (choice == 'y' || choice == 'Y') {
            printf("讓我先哭一會兒by:曹\n");
            Sleep(3);
            printf("恭喜你難度x2\n");
            difficulty = 2;
        } else if (choice == 'n' || choice == 'N') {
            printf("因為你和沒髮程式設計師一樣 \nso 難度驟降by:曹\n");
            difficulty = 1;
        }
    }
}

// 與物件互動
void interactWithObject() {
    int x = usergps[0];
    int y = usergps[1];
    if (map[x - 1][y] == 2) {
        printf("一個普通的裂縫，但像是人為造成的\n");
    } else if (map[x + 1][y] == 2) {
        printf("一個普通的裂縫，但像是人為造成的\n");
    } else if (map[x][y - 1] == 2) {
        printf("一個普通的裂縫，但像是人為造成的\n");
    } else if (map[x][y + 1] == 2) {
        printf("一個普通的裂縫，但像是人為造成的\n");
    }
    
}

// 門傳送 
void interactWithDoor() {
    printf("要離開嗎\n是        否\n");
    char choice;
    while (1) {
        choice = getch();
        if (choice == 'y' || choice == 'Y') {
            printf("你離開了這個地圖\n");
            data = 1;
            break;
        } else if (choice == 'n' || choice == 'N') {
            break;
        }
    }
}

// 移動怪物 
void moveMonster() {
    if (usergps[0] < monsterGps[0]) {
        monsterGps[0]--;
    } else if (usergps[0] > monsterGps[0]) {
        monsterGps[0]++;
    }
    if (usergps[1] < monsterGps[1]) {
        monsterGps[1]--;
    } else if (usergps[1] > monsterGps[1]) {
        monsterGps[1]++;
    }
    printMap();
}

// 是否被抓 
int checkGameOver() {
    return (usergps[0] == monsterGps[0] && usergps[1] == monsterGps[1]);
}

// 怪物執行續 
DWORD WINAPI monsterThread(LPVOID param) {
    time_t t1, t2;
    t1 = time(NULL);
    int timedata = (difficulty == 1) ? 2000 : 500;
    int timedata2 = 0;
    srand(time(NULL));
    while (gameRunning) {
        if (monsterGps[0] != -1 && monsterGps[1] != -1) {
            moveMonster();
            if (checkGameOver()) {
                system("cls");
                printf("遊戲失敗！你被怪物抓住了。\n");
                t2 = time(NULL);
                gameRunning = 0;
                printf("你逃了%d秒", (int)(t2 - t1));
                break;
            }
        }
        Sleep(timedata);
        timedata2++;
        if (timedata2 > 30) {
            timedata = timedata / 2;
            timedata2 = 0;
            setTriggerData("物", rand() % 38 + 2, rand() % 18 + 2);
            setTriggerData("物", rand() % 38 + 2, rand() % 18 + 2);
        }
    }
    return 0;
}

// 遊戲 
void gameLoop(void (*initMapFunc)(), void (*nextMapFunc)()) {
    char input;
    initMapFunc();
    printMap();  // 輸出地圖
    

    while (gameRunning) {
        input = getch();  // 取得方向鍵 
        if (input == 'w' || input == 'W') {
            if (usergps[1] > 1 && map[usergps[0]][usergps[1] - 1] == 0) {
                usergps[1]--;
            }
        } else if (input == 's' || input == 'S') {
            if (usergps[1] < MAP_HEIGHT - 2 && map[usergps[0]][usergps[1] + 1] == 0) {
                usergps[1]++;
            }
        } else if (input == 'a' || input == 'A') {
            if (usergps[0] > 1 && map[usergps[0] - 1][usergps[1]] == 0) {
                usergps[0]--;
            }
        } else if (input == 'd' || input == 'D') {
            if (usergps[0] < MAP_WIDTH - 2 && map[usergps[0] + 1][usergps[1]] == 0) {
                usergps[0]++;
            }
        } else if (input == 'n' || input == 'N') {
            if (isNearDoor()) {
                if (door_opened == 0) {
                    printf("鑰匙插了進去\n你打開了門\n");
                    door_opened = 1;
                }
                interactWithDoor();
                if (data == 1) {
                    nextMapFunc();  // Load next map
                    monsterThreadHandle = CreateThread(NULL, 0, monsterThread, NULL, 0, NULL);
    				if (monsterThreadHandle == NULL) {
        				printf("Failed to create monster thread.\n");
        				return;
    				}
                    data = 0;  // 更新地圖 
                }
            } else if (isNearInteractable()) {
                interactWithObject();
                system("pause");
            }else if (isNearbed()) {
                interactWithbed();
                system("pause");
            }else if (isNearfireplace()) {
                interactWithfireplace();
                system("pause");
            }
        }
        
        printMap();  // 更新地圖 
    }
	
    // 怪執行續開始 
    WaitForSingleObject(monsterThreadHandle, INFINITE);
    CloseHandle(monsterThreadHandle);
}

// 遊戲開始點********************************************************************************************** 
void startGameContent() {
    usergps[0] = 9;
    usergps[1] = 9;
    initializeMapGraphics();
    setTriggerData("：", 10, 9);
    setTriggerData("這", 11, 9);
    setTriggerData("裡", 12, 9);
    setTriggerData("...", 13, 9);
    setTriggerData("是", 14, 9);
    setTriggerData("哪", 15, 9);
    setTriggerData("裡", 16, 9);
    setTriggerData("？", 17, 9);
    printMap();
    
    system("pause");
    setTriggerData("想", 10, 9);
    setTriggerData("不", 11, 9);
    setTriggerData("起", 12, 9);
    setTriggerData("來", 13, 9);
    setTriggerData("了", 14, 9);
    printMap();
    system("pause");
    setTriggerData("感", 10, 9);
    setTriggerData("覺", 11, 9);
    setTriggerData("忘", 12, 9);
    setTriggerData("了", 13, 9);
    setTriggerData("什", 14, 9);
    setTriggerData("麼", 15, 9);
    setTriggerData("重", 16, 9);
    setTriggerData("要", 17, 9);
    setTriggerData("的", 18, 9);
    setTriggerData("事", 19, 9);
    setTriggerData("情", 20, 9);
    printMap();
    system("pause");
    setTriggerData("等", 10, 9);
    setTriggerData("等", 11, 9);
    setTriggerData("...", 12, 9);
    setTriggerData("我", 13, 9);
    setTriggerData("想", 14, 9);
    setTriggerData("起", 15, 9);
    setTriggerData("了", 16, 9);
    setTriggerData("移", 17, 9);
    setTriggerData("動", 18, 9);
    setTriggerData("方", 19, 9);
    setTriggerData("式", 20, 9);
    printMap();
    system("pause");
    setTriggerData("：", 10, 9);
    setTriggerData("Ｗ", 11, 9);
    setTriggerData("上", 12, 9);
    setTriggerData("Ｓ", 13, 9);
    setTriggerData("下", 14, 9);
    setTriggerData("Ａ", 15, 9);
    setTriggerData("左", 16, 9);
    setTriggerData("Ｄ", 17, 9);
    setTriggerData("右", 18, 9);
    setTriggerData("　", 19, 9);
    setTriggerData("　", 20, 9);
    printMap();
    system("pause");
    setTriggerData("：", 10, 9);
    setTriggerData("Ｎ", 11, 9);
    setTriggerData("可", 12, 9);
    setTriggerData("以", 13, 9);
    setTriggerData("跟", 14, 9);
    setTriggerData("道", 15, 9);
    setTriggerData("具", 16, 9);
    setTriggerData("互", 17, 9);
    setTriggerData("動", 18, 9);
    printMap();
    system("pause");
}

// 初始第一章地圖 
void initializeMap1() {
    usergps[0] = 9;
    usergps[1] = 9;
    monsterGps[0] = -1; // 怪物位置設為無效
    monsterGps[1] = -1;
    initializeMapGraphics();
    setTriggerData("裂", 0, 3);
    setTriggerData("縫", 0, 4);
    setTriggerData("裂", 34, 0);
    setTriggerData("縫", 35, 0);
    setGeneralData("桌", 1, 9);
    setGeneralData("子", 1, 10);
    setGeneralData("鏡", 38, 10);
    setGeneralData("子", 38, 11);
    setDoor("門", 35, 18);
    setbed("床", 37, 1);
    setfireplace("壁", 17, 1);
    setfireplace("爐", 18, 1);
}

// 初始第二地圖 
void initializeMap2() {
    usergps[0] = 9;
    usergps[1] = 9;
    monsterGps[0] = 1; // 怪物初始位置
    monsterGps[1] = 1;
    initializeMapGraphics();
    setTriggerData("物", 10, 10);
    setTriggerData("品", 11, 10);
}

// Main function
int main(int argc, char *argv[]) {
    startGameContent(); 
    gameLoop(initializeMap1, initializeMap2); 
    return 0;
}

