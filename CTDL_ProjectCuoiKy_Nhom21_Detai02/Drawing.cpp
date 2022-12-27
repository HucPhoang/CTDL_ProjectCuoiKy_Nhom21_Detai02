#include "graphics.h"
#include<iostream>
#include <fstream>
#pragma comment(lib,"graphics.lib")

using namespace std;

DWORD WIDTHSCREEN = GetSystemMetrics(SM_CXSCREEN);
DWORD HEIGHTSCREEN = GetSystemMetrics(SM_CYSCREEN);

struct NODE
{
    NODE* next;
    int COEF;
    int sign;
    int A, B, C;
    int key;

    void setKey()
    {
        if (A != 0) {
            if (B != 0) {
                if (C != 0) key = 3;
                else key = 2;
            }
            else {
                if (C != 0) key = 4;
                else key = 1;
            }
        }
        else {
            if (B != 0) {
                if (C != 0) key = 6;
                else key = 5;
            }
            else {
                if (C != 0) key = 7;
                else key = 0;
            }
        }
    }
};

struct CLINKED_LIST
{
    NODE* head;
    NODE* tail;

    void init()
    {
        head = new NODE;
        head->COEF = 0;
        head->A = head->B = head->C = 0;
        head->sign = -1;
        head->key = 10;
        head->next = head;
        tail = head;
    }

    int getLength()
    {
        int len = 0;
        NODE* t = head;
        do
        {
            t = t->next;
            len++;
        } while (t != head);
        return len;
    }

    bool isEmpty()
    {
        return getLength() == 1;
    }

    void insert(NODE* data, NODE* curr)
    {
        NODE* newNode = new NODE;
        newNode->COEF = data->COEF;
        newNode->A = data->A;
        newNode->B = data->B;
        newNode->C = data->C;
        newNode->sign = data->sign;
        newNode->key = data->key;

        //insert vao dau danh sach
        if (curr == head) {
            NODE* t = head;
            while (t->next != head) {
                t = t->next;
            }
            t->next = newNode;
            newNode->next = head;
            head = newNode;
            return;
        }

        NODE* t = head;
        while (t->next != curr) {
            t = t->next;
        }

        t->next = newNode;
        newNode->next = curr;
    }

    void deleteNode(NODE* curr)
    {
        if (isEmpty()) return;
        if (curr->sign == -1) return;

        //xoa node dau tien
        if (curr == head) {
            NODE* t = head, * temp = head;
            while (t->next != head) {
                t = t->next;
            }
            t->next = temp->next;
            head = t->next;
            delete(temp);
            return;
        }

        //xoa node bat ki
        NODE* t = head;
        while (t->next != curr) {
            t = t->next;
        }

        t->next = curr->next;
        delete(curr);
    }

    void swapData(NODE* a, NODE* b)
    {
        NODE* temp = new NODE;
        temp->COEF = a->COEF;
        temp->A = a->A;
        temp->B = a->B;
        temp->C = a->C;
        temp->sign = a->sign;
        temp->key = a->key;

        a->COEF = b->COEF;
        a->A = b->A;
        a->B = b->B;
        a->C = b->C;
        a->sign = b->sign;
        a->key = b->key;


        b->COEF = temp->COEF;
        b->A = temp->A;
        b->B = temp->B;
        b->C = temp->C;
        b->sign = temp->sign;
        b->key = temp->key;

        delete(temp);
    }

    void sort()
    {
        NODE* curr = head, * temp = NULL;

        do {
            temp = curr->next;
            while (temp != head) {
                if (curr->key > temp->key) {
                    swapData(curr, temp);
                }
                else if (curr->key == temp->key) {
                    if (curr->A < temp->A) swapData(curr, temp);
                    if (curr->A == temp->A && curr->B < temp->B) swapData(curr, temp);
                    if (curr->A == temp->A && curr->B == temp->B && curr->C < temp->C) swapData(curr, temp);
                }
                temp = temp->next;
            }
            curr = curr->next;
        } while (curr->next != head);
    }
};

CLINKED_LIST P, Q;

void readInput()
{
    P.init();
    Q.init();

    ifstream fi("INPUT.txt");
    int nP, nQ;
    fi >> nP >> nQ;
    for (int i = 0; i < nP; ++i) {
        NODE* node = new NODE;
        fi >> node->COEF >> node->A >> node->B >> node->C;
        node->sign = 1;
        node->setKey();
        P.insert(node, P.tail);
    }

    for (int i = 0; i < nQ; ++i) {
        NODE* node = new NODE;
        fi >> node->COEF >> node->A >> node->B >> node->C;
        node->sign = 1;
        node->setKey();
        Q.insert(node, Q.tail);
    }
    fi.close();
}

void IntToChar(char ch[], int num)
{
    if (num == 0) ch[0] = '0';

    int len = 0;

    int temp = abs(num);
    while (temp) {
        len++;
        temp /= 10;
    }

    int s = 0, e = len - 1;
    if (num < 0) {
        num = -num;
        ch[0] = '-';
        s++;
        e++;
    }

    while (e >= s) {
        ch[e] = (num % 10 + '0');
        num /= 10;
        e--;
    }
}

void drawPolynomial(CLINKED_LIST& list, int srcX, int srcY, char text)
{
    const int recWidth = 80, recHeight = 40;
    NODE* curr = list.head;
    int x = srcX;
    int y = srcY;
    int space = 20;

    setcolor(WHITE);
    outtextxy(srcX / 2 - textwidth(&text) / 2, srcY + recHeight / 2 - textheight(&text) / 2, &text);
    do {
        setcolor(WHITE);
        rectangle(x, y, x + recWidth, y + recHeight);
        rectangle(x, y + recHeight / 2, x + recWidth / 4, y + recHeight);
        rectangle(x + recWidth / 4, y + recHeight / 2, x + 2 * recWidth / 4, y + recHeight);
        rectangle(x + 2 * recWidth / 4, y + recHeight / 2, x + 3 * recWidth / 4, y + recHeight);
        rectangle(x + 3 * recWidth / 4, y + recHeight / 2, x + 4 * recWidth / 4, y + recHeight);

        char chCOEF[15];
        char chA[15];
        char chB[15];
        char chC[15];

        memset(chCOEF, '\0', 15);
        memset(chA, '\0', 15);
        memset(chB, '\0', 15);
        memset(chC, '\0', 15);

        IntToChar(chCOEF, curr->COEF);
        IntToChar(chA, curr->A);
        IntToChar(chB, curr->B);
        IntToChar(chC, curr->C);

        setcolor(YELLOW);
        outtextxy(x + recWidth / 2 - textwidth(chCOEF) / 2, y + recHeight / 4 - textheight(chCOEF) / 2, chCOEF);
        outtextxy(x + recWidth / 8 - textwidth(chA) / 2, y + 3 * recHeight / 4 - textheight(chA) / 2, chA);
        outtextxy(x + 3 * recWidth / 8 - textwidth(chB) / 2, y + 3 * recHeight / 4 - textheight(chB) / 2, chB);
        outtextxy(x + 5 * recWidth / 8 - textwidth(chC) / 2, y + 3 * recHeight / 4 - textheight(chC) / 2, chC);

        setfillstyle(SOLID_FILL, RED);
        setcolor(RED);
        if (curr->next != list.head) {
            bar(x + 7 * recWidth / 8, y + 3 * recHeight / 4, x + 7 * recWidth / 8 + space, y + 3 * recHeight / 4 + 1);
            bar(x + 7 * recWidth / 8 + space, y + recHeight / 2, x + 7 * recWidth / 8 + space + 1, y + 3 * recHeight / 4);
            bar(x + recWidth + space / 2, y + recHeight / 2, x + recWidth + space, y + recHeight / 2 + 1);
            circle(x + recWidth + space, y + recHeight / 2, 1);
            circle(x + recWidth + space, y + recHeight / 2, 2);
            circle(x + recWidth + space, y + recHeight / 2, 3);
        }
        else {
            bar(x + 7 * recWidth / 8, y + 3 * recHeight / 4, x + 7 * recWidth / 8 + space, y + 3 * recHeight / 4 + 1);
            bar(x + 7 * recWidth / 8 + space, y + 3 * recHeight / 4, x + 7 * recWidth / 8 + space + 1, y + 3 * recHeight / 4 + 2 * space);
            bar(srcX + recWidth / 2, srcY + recHeight + 3 * space / 2, x + 7 * recWidth / 8 + space, srcY + recHeight + 3 * space / 2 + 1);
            bar(srcX + recWidth / 2, srcY + recHeight, srcX + recWidth / 2 + 1, srcY + recHeight + 3 * space / 2);
            circle(srcX + recWidth / 2, srcY + recHeight, 1);
            circle(srcX + recWidth / 2, srcY + recHeight, 2);
            circle(srcX + recWidth / 2, srcY + recHeight, 3);
        }

        x += recWidth + space;

        curr = curr->next;
    } while (curr != list.head);
}

void addPolynomials()
{
    NODE* currP = P.head, * currQ = Q.head;
    CLINKED_LIST R;
    R.init();

    while (currP->next != P.head && currQ->next != Q.head) {
        if (currP->key < currQ->key) {
            R.insert(currP, R.tail);
            currP = currP->next;
        }
        else if (currP->key > currQ->key) {
            R.insert(currQ, R.tail);
            currQ = currQ->next;
        }
        else {
            if (currP->A == currQ->A && currP->B == currQ->B && currP->C == currQ->C) {
                NODE* node = new NODE;
                node->COEF = currP->COEF + currQ->COEF;
                node->A = currP->A;
                node->B = currP->B;
                node->C = currP->C;
                node->sign = currP->sign;
                node->key = currP->key;

                if (node->COEF != 0) R.insert(node, R.tail);
            }
            else {
                R.insert(currP, R.tail);
                R.insert(currQ, R.tail);
            }

            currP = currP->next;
            currQ = currQ->next;
        }
    }

    while (currP->next != P.head) {
        R.insert(currP, R.tail);
        currP = currP->next;
    }

    while (currQ->next != Q.head) {
        R.insert(currQ, R.tail);
        currQ = currQ->next;
    }

    R.sort();
    drawPolynomial(R, 50, 350, 'R');
}

void multiplyPolynomials()
{
    NODE* currP = P.head, * currQ = Q.head;
    CLINKED_LIST R;
    R.init();

    while (currP->next != P.head) {
        currQ = Q.head;
        while (currQ->next != Q.head) {
            NODE* node = new NODE;
            node->COEF = currP->COEF * currQ->COEF;
            node->A = currP->A + currQ->A;
            node->B = currP->B + currQ->B;
            node->C = currP->C + currQ->C;
            node->sign = currP->sign;
            node->setKey();

            R.insert(node, R.tail);

            currQ = currQ->next;
        }
        currP = currP->next;
    }

    NODE* i = R.head, * j = R.head->next;
    while (i->next != R.head) {
        j = i->next;
        while (j->next != R.head) {
            if (i->A == j->A && i->B == j->B && i->C == j->C) {
                i->COEF += j->COEF;
                NODE* temp = j->next;
                R.deleteNode(j);
                j = temp;
            }
            else j = j->next;
        }

        if (i->COEF == 0) {
            NODE* temp = i->next;
            R.deleteNode(i);
            i = temp;
        }
        else i = i->next;
    }

    R.sort();
    drawPolynomial(R, 50, 350, 'R');
}

int main()
{
    readInput();
    P.sort();
    Q.sort();

    initwindow(WIDTHSCREEN, HEIGHTSCREEN);

    char userOption = '5';

    do {
        setbkcolor(BLACK);
        cleardevice();
        int len = 5;
        char programName[] = " CHUONG TRINH MO PHONG DA THUC - NHOM 21_08 ";
        char optionsList[][200] = {
                                    " 1. Hien thi da thuc P ",
                                    " 2. Hien thi da thuc Q ",
                                    " 3. Cong hai da thuc ",
                                    " 4. Nhan hai da thuc ",
                                    " 5. Thoat khoi chuong trinh "
        };

        int midX = WIDTHSCREEN / 2;
        int midY = HEIGHTSCREEN / 2;

        setcolor(RED);
        setbkcolor(WHITE);
        outtextxy(midX - textwidth(programName) / 2, 100, programName);

        for (int i = 0, yA = 130; i < len; ++i, yA += 30) {
            outtextxy(midX - textwidth(optionsList[i]) / 2, yA, optionsList[i]);
        }

        userOption = (char)getch();
        if (userOption < '1' || userOption > '5') continue;

        switch (userOption) {
        case '1':
            setbkcolor(BLACK);
            cleardevice();
            drawPolynomial(P, 50, 50, 'P');
            getch();
            break;
        case '2':
            setbkcolor(BLACK);
            cleardevice();
            drawPolynomial(Q, 50, 50, 'Q');
            getch();
            break;
        case '3':
            setbkcolor(BLACK);
            cleardevice();
            drawPolynomial(P, 50, 50, 'P');
            drawPolynomial(Q, 50, 200, 'Q');
            setfillstyle(SOLID_FILL, WHITE);
            bar(0, 300, WIDTHSCREEN, 302);
            addPolynomials();
            getch();
            break;
        case '4':
            setbkcolor(BLACK);
            cleardevice();
            drawPolynomial(P, 50, 50, 'P');
            drawPolynomial(Q, 50, 200, 'Q');
            setfillstyle(SOLID_FILL, WHITE);
            bar(0, 300, WIDTHSCREEN, 302);
            multiplyPolynomials();
            getch();
            break;
        default: break;
        }
    } while (userOption != '5');

    closegraph();
    return 0;
}