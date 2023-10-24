#include<easyx.h>
#include<math.h>
#include <time.h>
const int diameter = 10, width = 600, height = 400;
int cnt = 0, slpTime = 50;
void drawWall()
{
	setfillcolor(RED);
	for (int i = 5; i < width; i += 10) {
		fillrectangle(i - 5, 0, i + 5, 10);
		fillrectangle(i - 5, height - 10, i + 5, height);
	}
	for (int i = 15; i < height - 5; i += 10) {
		fillrectangle(0, i - 5, 10, i + 5);
		fillrectangle(width - 10, i - 5, width, i + 5);
	}
}
struct Point {
	Point(int _x, int _y) :x(_x), y(_y), next(NULL), pre(NULL) {}
	int x, y;
	Point* pre, * next;
};
bool isAdjacent(Point* p1, Point* p2)
{
	if (p1->y == p2->y && abs(p1->x - p2->x) <= diameter)return true;//圆直径diameter，正方形边长diameter
	if (p1->x == p2->x && abs(p1->y - p2->y) <= diameter)return true;
	return false;
}
double dis(Point* p1, Point* p2)
{
	int delx = p1->x - p2->x, dely = p1->y - p2->y;
	return sqrt(delx * delx + dely * dely);
}
class Food {
public:
	Food(int _x, int _y, int _oriReward = 1, int _diamtr = diameter);
	~Food() { delete pos; }
	virtual void display();
	virtual void adjustCurReward() { return; }
	virtual void adjustETime(int del) { return; }
	virtual bool outOfTime() { return false; }
	void setX(int _x) { pos->x = _x; }
	void setY(int _y) { pos->y = _y; }
	Point* getPos() { return pos; }
	int getCurReward() { return curReward; }
	void setCurReward(int _curReward) { curReward = _curReward; }
	int getOriReward() { return oriReward; }
	int getDiamtr() { return diamtr; }
private:
	Point* pos;
	int curReward, oriReward, diamtr;
};
Food::Food(int _x, int _y, int _oriReward, int _diamtr)
{
	pos = new Point(_x, _y);
	curReward = oriReward = _oriReward;
	diamtr = _diamtr;
}
void Food::display()
{
	setfillcolor(GREEN);
	fillcircle(pos->x, pos->y, diamtr / 2);
}
class BigFood :public Food {
public:
	BigFood(int _x, int _y, int _oriReward = 10, int _diamtr = 2 * diameter, int _eTime = 5000) :Food(_x, _y, _oriReward, _diamtr) { eTime = _eTime; }
	~BigFood() {}
	void display();
	void setETime(int _eTime) { eTime = _eTime; }
	void adjustCurReward() { setCurReward(eTime / 5000.0 * getOriReward()); }
	void adjustETime(int del) { eTime -= del; }
	bool outOfTime() { return eTime <= 0; }
private:
	int eTime;//初始值5000ms
};
void BigFood::display()
{
	Point* pos = getPos();
	setfillcolor(GREEN);
	fillcircle(pos->x, pos->y, getDiamtr() / 2);
	fillrectangle(width / 2 - 25, height - 20, width / 2 - 25 + 50 * eTime / 5000, height - 10);
	setfillcolor(WHITE);
	fillrectangle(width / 2 - 25 + 50 * eTime / 5000, height - 20, width / 2 + 25, height - 10);
}
class Snake {
public:
	Snake(int x, int y);
	~Snake();
	void insert(int x, int y);
	void extend();
	bool move();
	void display();
	void setDirection(char d) { direction = d; }
	bool eat(Food* f);
	char getDirect() { return direction; }
private:
	Point* head, * tail;
	int speed;
	char direction;
};
Snake::Snake(int x, int y)
{
	head = tail = new Point(x, y);
	speed = diameter;
	direction = 'U';
}
Snake::~Snake()
{
	Point* p = head, * tp = NULL;
	while (p != NULL) {
		tp = p->next;
		delete p;
		p = tp;
	}
}
void Snake::insert(int x, int y)
{
	tail->next = new Point(x, y);
	tail->next->pre = tail;
	tail = tail->next;
	//++speed;
}
void Snake::extend()
{
	if (tail->pre != NULL && isAdjacent(tail, tail->pre)) {
		if (tail->y == tail->pre->y)insert(tail->x + tail->x - tail->pre->x, tail->y);
		else insert(tail->x, tail->y + tail->y - tail->pre->y);
	}
	else {
		switch (direction) {
		case'U':
			insert(tail->x, tail->y + diameter);
			break;
		case'D':
			insert(tail->x, tail->y - diameter);
			break;
		case'L':
			insert(tail->x + diameter, tail->y);
			break;
		case'R':
			insert(tail->x - diameter, tail->y);
			break;
		default:
			break;
		}
	}
}
bool Snake::move()
{
	Point* p = tail;
	while (p != head) {
		p->x = p->pre->x;
		p->y = p->pre->y;
		p = p->pre;
	}
	switch (direction) {//p==head
	case 'U':
		p->y -= speed;
		break;
	case'D':
		p->y += speed;
		break;
	case'L':
		p->x -= speed;
		break;
	case 'R':
		p->x += speed;
		break;
	default:
		break;
	}
	if (head->x < 10 || head->x>width - 10 || head->y < 10 || head->y>height - 10)return false;//出界
	p = head->next;
	while (p != NULL) {//撞到自己
		if (dis(head, p) < (double)diameter)return false;
		p = p->next;
	}
	return true;
}
void Snake::display()
{
	setfillcolor(BLUE);
	fillcircle(head->x, head->y, diameter / 2);
	Point* p = head->next;
	while (p != NULL) {
		fillrectangle(p->x - diameter / 2, p->y + diameter / 2, p->x + diameter / 2, p->y - diameter / 2);
		p = p->next;
	}
}
bool Snake::eat(Food* f)
{
	bool res = dis(head, f->getPos()) <= (double)f->getDiamtr();
	return res;
}
int getRand(int min, int max)
{
	return (rand() % (max - min + 1) + min);
}
int main()
{
	initgraph(width, height);
	setbkcolor(RGB(192, 192, 192));
	cleardevice();
	Snake mySnake(width / 2, height / 2);
	ExMessage* msg = new ExMessage;
	Food* f = new Food(width / 2, height / 4);
	srand(time(0));
	while (1) {
		cleardevice();
		BeginBatchDraw();
		drawWall();
		//change();
		mySnake.display();
		f->display();
		EndBatchDraw();
		Sleep(slpTime);
		f->adjustETime(slpTime);
		if (peekmessage(msg, EM_KEY)) {
			char direction = mySnake.getDirect();
			switch (msg->vkcode) {
			case 0x41:
				if (direction != 'R')mySnake.setDirection('L');//禁止掉头，下同
				break;
			case 0x53:
				if (direction != 'U')mySnake.setDirection('D');
				break;
			case 0x44:
				if (direction != 'L')mySnake.setDirection('R');
				break;
			case 0x57:
				if (direction != 'D')mySnake.setDirection('U');
			default:
				break;
			}
		}
		if (!mySnake.move()) {
			//cleardevice();
			settextcolor(RED);
			outtextxy(width / 2 - 10, height / 2, L'失');
			outtextxy(width / 2 + 10, height / 2, L'败');
			Sleep(5000);
			break;
		}
		int rx = getRand(diameter / 2 + 10, width - diameter / 2 - 10),
			ry = getRand(diameter / 2 + 10, height - diameter / 2 - 10);
		if (mySnake.eat(f)) {
			++cnt;
			--slpTime;//吃的越多，速度越快
			f->adjustCurReward();
			for (int i = 0; i < f->getCurReward(); ++i)mySnake.extend();
			delete f;
			if (cnt == 4) {
				f = new BigFood(rx, ry);
				cnt = -1;
			}
			else f = new Food(rx, ry);
		}
		if (f->outOfTime()) {
			delete f;
			f = new Food(rx, ry);
			cnt = 0;
		}
	}
	delete f;
	delete msg;
}
