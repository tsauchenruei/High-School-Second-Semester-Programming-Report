#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
// �]�w��� 
#define MAP_WIDTH 40
#define MAP_HEIGHT 20
#define PIC_WIDTH 80

// �]�w�U�ؼƾ� 
char *pic[PIC_WIDTH][MAP_HEIGHT];
int map[MAP_WIDTH][MAP_HEIGHT];
int usergps[2] = {20, 10};
int monsterGps[2] = {-1, -1}; // �Ǫ���l��m���L�Ħ�m�A�u���b�ĤG�a�Ϥ~��m���Ħ�m
int data = 0;
int difficulty = 0;
int door_opened = 0; // �̬O�_���}�L 
HANDLE monsterThreadHandle; // �Ǫ������� 
int gameRunning = 1; // �C�����A 
#define WHITE "\x1B[0;37m"
#define KDRK "\x1B[0;31m"
// ��ت�l�� 
void initializeMapBorders() {
    int x, y;
    for (x = 0; x < MAP_WIDTH; x++) {
        for (y = 0; y < MAP_HEIGHT; y++) {
            map[x][y] = 0; // �M���a�ϩҦ���� 
        }
    }
    
    //�]��� 
    for (x = 0; x < MAP_WIDTH; x++) {
        map[x][0] = 1;
        map[x][MAP_HEIGHT - 1] = 1;
    }
    for (y = 0; y < MAP_HEIGHT; y++) {
        map[0][y] = 1;
        map[MAP_WIDTH - 1][y] = 1;
    }
}

// ��l�Ʀa�Ϲϧ�
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
        strcpy(pic[x * 2][0], "��");
        strcpy(pic[x * 2][MAP_HEIGHT - 1], "��");
    }
    for (y = 0; y < MAP_HEIGHT; y++) {
        strcpy(pic[0][y], "��");
        strcpy(pic[(MAP_WIDTH - 1) * 2][y], "��");
    }
}

// �C�L�a��(���a �Ǫ�) 
void printMap() {
    system("cls");
    int x, y;
    
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            if (x == usergps[0] && y == usergps[1]) {
                printf("��");
            } else if (x == monsterGps[0] && y == monsterGps[1] && monsterGps[0] != -1 && monsterGps[1] != -1) {
            	printf(KDRK"��");
            	printf(WHITE"");
            } else {
                printf("%s", pic[x * 2][y]);
            }
        }
        printf("\n");
    }
}

// �C�L�a�ϸ�ƥ\��
void printMapData() {
    int x, y;
    for (y = 0; y < MAP_HEIGHT; y++) {
        for (x = 0; x < MAP_WIDTH; x++) {
            printf("%d", map[x][y]);
        }
        printf("\n");
    }
}

// ���i�L
void setGeneralData(char *symbol, int x, int y) {
    map[x][y] = 1;
    strncpy(pic[x * 2][y], symbol, 2);
}

// �G�ƽu 
void setTriggerData(char *symbol, int x, int y) {
    map[x][y] = 2;
    strncpy(pic[x * 2][y], symbol, 2);
}

// ��
void setDoor(char *symbol, int x, int y) {
    map[x][y] = 3;
    strncpy(pic[x * 2][y], symbol, 2);
}

//�� 
void setbed(char *symbol, int x, int y) {
    map[x][y] = 4;
    strncpy(pic[x * 2][y], symbol, 2);
}
//���l 
void setfireplace(char *symbol, int x, int y) {
    map[x][y] = 5;
    strncpy(pic[x * 2][y], symbol, 2);
}
// �O�_�b���l�� 
int isNearfireplace() {
    int x = usergps[0];
    int y = usergps[1];
    return (map[x - 1][y] == 5 || map[x + 1][y] == 5 || map[x][y - 1] == 5 || map[x][y + 1] == 5);
}
// �O�_�b�ɮ� 
int isNearbed() {
    int x = usergps[0];
    int y = usergps[1];
    return (map[x - 1][y] == 4 || map[x + 1][y] == 4 || map[x][y - 1] == 4 || map[x][y + 1] == 4);
}
// �T�{�O�_�b���� 
int isNearDoor() {
    int x = usergps[0];
    int y = usergps[1];
    return (map[x - 1][y] == 3 || map[x + 1][y] == 3 || map[x][y - 1] == 3 || map[x][y + 1] == 3);
}

// �O�_�b����� 
int isNearInteractable() {
    int x = usergps[0];
    int y = usergps[1];
    return (map[x - 1][y] == 2 || map[x + 1][y] == 2 || map[x][y - 1] == 2 || map[x][y + 1] == 2);
}
void interactWithbed() {
    int x = usergps[0];
    int y = usergps[1];
    
    if (map[x - 1][y] == 4) {
        printf("�@�Ӵ��q���ɡA��...\n");
    } else if (map[x + 1][y] == 4) {
        printf("�@�Ӵ��q���ɡA��...\n");
    } else if (map[x][y - 1] == 4) {
        printf("�@�Ӵ��q���ɡA��...\n");
    } else if (map[x][y + 1] == 4) {
        printf("�@�Ӵ��q���ɡA��...\n");
    }
}
void interactWithfireplace() {
    int x = usergps[0];
    int y = usergps[1];
    
    if (map[x - 1][y] == 5) {
        printf("�A�O���h�ʷR\n�ڤ��ʨk/�k��(y) �ګܯʨk/�k��(n)\n");
        char choice = getch();
        if (choice == 'y' || choice == 'Y') {
            printf("���ڥ����@�|��by:��\n");
            Sleep(3);
            printf("���ߧA����x2\n");
            difficulty = 2;
        } else if (choice == 'n' || choice == 'N') {
            printf("�]���A�M�S�v�{���]�p�v�@�� \nso �����J��by:��\n");
            difficulty = 1;
        }
    } else if (map[x + 1][y] == 5) {
        printf("�A�O���h�ʷR\n�ڤ��ʨk/�k��(y) �ګܯʨk/�k��(n)\n");
        char choice = getch();
        if (choice == 'y' || choice == 'Y') {
            printf("���ڥ����@�|��by:��\n");
            Sleep(3);
            printf("���ߧA����x2\n");
            difficulty = 2;
        } else if (choice == 'n' || choice == 'N') {
            printf("�]���A�M�S�v�{���]�p�v�@�� \nso �����J��by:��\n");
            difficulty = 1;
        }
    } else if (map[x][y - 1] == 5) {
        printf("�A�O���h�ʷR\n�ڤ��ʨk/�k��(y) �ګܯʨk/�k��(n)\n");
        char choice = getch();
        if (choice == 'y' || choice == 'Y') {
            printf("���ڥ����@�|��by:��\n");
            Sleep(3);
            printf("���ߧA����x2\n");
            difficulty = 2;
        } else if (choice == 'n' || choice == 'N') {
            printf("�]���A�M�S�v�{���]�p�v�@�� \nso �����J��by:��\n");
            difficulty = 1;
        }
    } else if (map[x][y + 1] == 5) {
        printf("�A�O���h�ʷR\n�ڤ��ʨk/�k��(y) �ګܯʨk/�k��(n)\n");
        char choice = getch();
        if (choice == 'y' || choice == 'Y') {
            printf("���ڥ����@�|��by:��\n");
            Sleep(3);
            printf("���ߧA����x2\n");
            difficulty = 2;
        } else if (choice == 'n' || choice == 'N') {
            printf("�]���A�M�S�v�{���]�p�v�@�� \nso �����J��by:��\n");
            difficulty = 1;
        }
    }
}

// �P���󤬰�
void interactWithObject() {
    int x = usergps[0];
    int y = usergps[1];
    if (map[x - 1][y] == 2) {
        printf("�@�Ӵ��q�����_�A�����O�H���y����\n");
    } else if (map[x + 1][y] == 2) {
        printf("�@�Ӵ��q�����_�A�����O�H���y����\n");
    } else if (map[x][y - 1] == 2) {
        printf("�@�Ӵ��q�����_�A�����O�H���y����\n");
    } else if (map[x][y + 1] == 2) {
        printf("�@�Ӵ��q�����_�A�����O�H���y����\n");
    }
    
}

// ���ǰe 
void interactWithDoor() {
    printf("�n���}��\n�O        �_\n");
    char choice;
    while (1) {
        choice = getch();
        if (choice == 'y' || choice == 'Y') {
            printf("�A���}�F�o�Ӧa��\n");
            data = 1;
            break;
        } else if (choice == 'n' || choice == 'N') {
            break;
        }
    }
}

// ���ʩǪ� 
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

// �O�_�Q�� 
int checkGameOver() {
    return (usergps[0] == monsterGps[0] && usergps[1] == monsterGps[1]);
}

// �Ǫ������� 
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
                printf("�C�����ѡI�A�Q�Ǫ����F�C\n");
                t2 = time(NULL);
                gameRunning = 0;
                printf("�A�k�F%d��", (int)(t2 - t1));
                break;
            }
        }
        Sleep(timedata);
        timedata2++;
        if (timedata2 > 30) {
            timedata = timedata / 2;
            timedata2 = 0;
            setTriggerData("��", rand() % 38 + 2, rand() % 18 + 2);
            setTriggerData("��", rand() % 38 + 2, rand() % 18 + 2);
        }
    }
    return 0;
}

// �C�� 
void gameLoop(void (*initMapFunc)(), void (*nextMapFunc)()) {
    char input;
    initMapFunc();
    printMap();  // ��X�a��
    

    while (gameRunning) {
        input = getch();  // ���o��V�� 
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
                    printf("�_�ʹ��F�i�h\n�A���}�F��\n");
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
                    data = 0;  // ��s�a�� 
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
        
        printMap();  // ��s�a�� 
    }
	
    // �ǰ�����}�l 
    WaitForSingleObject(monsterThreadHandle, INFINITE);
    CloseHandle(monsterThreadHandle);
}

// �C���}�l�I********************************************************************************************** 
void startGameContent() {
    usergps[0] = 9;
    usergps[1] = 9;
    initializeMapGraphics();
    setTriggerData("�G", 10, 9);
    setTriggerData("�o", 11, 9);
    setTriggerData("��", 12, 9);
    setTriggerData("...", 13, 9);
    setTriggerData("�O", 14, 9);
    setTriggerData("��", 15, 9);
    setTriggerData("��", 16, 9);
    setTriggerData("�H", 17, 9);
    printMap();
    
    system("pause");
    setTriggerData("�Q", 10, 9);
    setTriggerData("��", 11, 9);
    setTriggerData("�_", 12, 9);
    setTriggerData("��", 13, 9);
    setTriggerData("�F", 14, 9);
    printMap();
    system("pause");
    setTriggerData("�P", 10, 9);
    setTriggerData("ı", 11, 9);
    setTriggerData("��", 12, 9);
    setTriggerData("�F", 13, 9);
    setTriggerData("��", 14, 9);
    setTriggerData("��", 15, 9);
    setTriggerData("��", 16, 9);
    setTriggerData("�n", 17, 9);
    setTriggerData("��", 18, 9);
    setTriggerData("��", 19, 9);
    setTriggerData("��", 20, 9);
    printMap();
    system("pause");
    setTriggerData("��", 10, 9);
    setTriggerData("��", 11, 9);
    setTriggerData("...", 12, 9);
    setTriggerData("��", 13, 9);
    setTriggerData("�Q", 14, 9);
    setTriggerData("�_", 15, 9);
    setTriggerData("�F", 16, 9);
    setTriggerData("��", 17, 9);
    setTriggerData("��", 18, 9);
    setTriggerData("��", 19, 9);
    setTriggerData("��", 20, 9);
    printMap();
    system("pause");
    setTriggerData("�G", 10, 9);
    setTriggerData("��", 11, 9);
    setTriggerData("�W", 12, 9);
    setTriggerData("��", 13, 9);
    setTriggerData("�U", 14, 9);
    setTriggerData("��", 15, 9);
    setTriggerData("��", 16, 9);
    setTriggerData("��", 17, 9);
    setTriggerData("�k", 18, 9);
    setTriggerData("�@", 19, 9);
    setTriggerData("�@", 20, 9);
    printMap();
    system("pause");
    setTriggerData("�G", 10, 9);
    setTriggerData("��", 11, 9);
    setTriggerData("�i", 12, 9);
    setTriggerData("�H", 13, 9);
    setTriggerData("��", 14, 9);
    setTriggerData("�D", 15, 9);
    setTriggerData("��", 16, 9);
    setTriggerData("��", 17, 9);
    setTriggerData("��", 18, 9);
    printMap();
    system("pause");
}

// ��l�Ĥ@���a�� 
void initializeMap1() {
    usergps[0] = 9;
    usergps[1] = 9;
    monsterGps[0] = -1; // �Ǫ���m�]���L��
    monsterGps[1] = -1;
    initializeMapGraphics();
    setTriggerData("��", 0, 3);
    setTriggerData("�_", 0, 4);
    setTriggerData("��", 34, 0);
    setTriggerData("�_", 35, 0);
    setGeneralData("��", 1, 9);
    setGeneralData("�l", 1, 10);
    setGeneralData("��", 38, 10);
    setGeneralData("�l", 38, 11);
    setDoor("��", 35, 18);
    setbed("��", 37, 1);
    setfireplace("��", 17, 1);
    setfireplace("�l", 18, 1);
}

// ��l�ĤG�a�� 
void initializeMap2() {
    usergps[0] = 9;
    usergps[1] = 9;
    monsterGps[0] = 1; // �Ǫ���l��m
    monsterGps[1] = 1;
    initializeMapGraphics();
    setTriggerData("��", 10, 10);
    setTriggerData("�~", 11, 10);
}

// Main function
int main(int argc, char *argv[]) {
    startGameContent(); 
    gameLoop(initializeMap1, initializeMap2); 
    return 0;
}

