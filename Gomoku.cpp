#include<easyx.h>
#include<conio.h>
#include<string.h>
#include<math.h>
const int width = 600, height = 500, del = 6;
void drawChessBoard()//21×21，居中的棋盘
{
	setfillcolor(RGB(255, 128, 64));
	fillrectangle(width / 2 - 200, height / 2 - 200, width / 2 + 200, height / 2 + 200);
	setlinecolor(BLACK);
	for (int i = height / 2 - 200; i <= height / 2 + 200; i += 20)
		line(width / 2 - 200, i, width / 2 + 200, i);
	for (int i = width / 2 - 200; i <= width / 2 + 200; i += 20)
		line(i, height / 2 - 200, i, height / 2 + 200);
}
void drawButton()
{
	setfillcolor(WHITE);
	fillrectangle(width - 75, height / 2 - 50, width - 25, height / 2 + 50);
	settextcolor(BLACK);
	outtextxy(width - 55, height / 2 - 20, L'悔');
	outtextxy(width - 55, height / 2 + 20, L'棋');
}
void drawPlayer(int cnt)
{
	settextcolor(BLACK);
	if (cnt & 1)outtextxy(45, height / 2 - 40, L'黑');
	else outtextxy(45, height / 2 - 40, L'白');
	outtextxy(45, height / 2 - 20, L'棋');
	outtextxy(45, height / 2 - 0, L'的');
	outtextxy(45, height / 2 + 20, L'回');
	outtextxy(45, height / 2 + 40, L'合');
}
bool isAtButton(int x, int y)
{
	return x >= width - 75 && x <= width - 25 && y >= height / 2 - 50 && y <= height / 2 + 50;
}
int ceil(int a, int b)//a/b向上取整
{
	return a % b ? a / b + 1 : a / b;
}
double dis(int a, int b, int c, int d)
{
	int delx = a - c, dely = b - d;
	return sqrt(delx * delx + dely * dely);
}
bool isAtTheIntersection(int& x, int& y)
{
	int delx = x - (width / 2 - 200), dely = y - (height / 2 - 200);
	if (!(delx >= 0 && delx <= 400 && dely >= 0 && dely <= 400))return false;
	bool f1 = delx % 20 < del || delx % 20 >= 20 - del, f2 = dely % 20 < del || dely % 20 >= 20 - del;
	if (delx % 20 >= 20 - del)x = ((x - (width / 2 - 200)) / 20 + 1) * 20 + width / 2 - 200;
	if (dely % 20 >= 20 - del)y = ((y - (height / 2 - 200)) / 20 + 1) * 20 + height / 2 - 200;
	return f1 && f2;//降低要求，提高舒适度
}
int toChessBoardX(int x)
{
	int tem = x - (width / 2 - 200);
	//return x%20<del?tem/20:ceil(tem,20);
	return tem / 20;
}
int toChessBoardY(int y)
{
	int tem = y - (height / 2 - 200);
	//return y%20<del?tem/20:ceil(tem,20);
	return tem / 20;
}
int toScreenX(int x)
{
	return x * 20 + (width / 2 - 200);
}
int toScreenY(int y)
{
	return y * 20 + (height / 2 - 200);
}
class Player {
public:
	Player(bool _isBlack) :isBlack(_isBlack), top(0), leftRegretTimes(3) { memset(chess, 0, sizeof(chess)); }
	~Player() {}
	void addChess(int x, int y);
	bool regret(Player& anthor);
	bool existChess(int x, int y);
	void display();
	bool win(int x, int y);
private:
	bool chess[21][21], isBlack;
	int skX[500], skY[500], top, leftRegretTimes;
};
void Player::addChess(int x, int y)
{
	x = toChessBoardX(x), y = toChessBoardY(y);
	skX[top] = x, skY[top] = y;
	++top;
	chess[x][y] = 1;
}
bool Player::existChess(int x, int y)
{
	x = toChessBoardX(x), y = toChessBoardY(y);
	return chess[x][y];
}
void Player::display()
{
	for (int x = 0; x < 21; ++x) {
		for (int y = 0; y < 21; ++y) {
			if (chess[x][y]) {
				int tx = toScreenX(x), ty = toScreenY(y);
				if (isBlack)setfillcolor(BLACK);
				else setfillcolor(WHITE);
				fillcircle(tx, ty, 8);
			}
		}
	}
}
void displayAll(Player& p1, Player& p2, int cnt, bool end = false, bool blackWin = false, bool regretFail = false)
{
	BeginBatchDraw();
	drawChessBoard();
	p1.display();
	p2.display();
	drawPlayer(cnt);
	drawButton();
	if (end) {
		settextcolor(RED);
		if (blackWin)outtextxy(width / 2 - 10, height / 2, L'黑');
		else outtextxy(width / 2 - 10, height / 2, L'白');
		outtextxy(width / 2 + 10, height / 2, L'胜');
	}
	if (regretFail) {
		settextcolor(RED);
		if (cnt & 1)outtextxy(width / 2 - 80, height / 2 - 20, L'白');
		else outtextxy(width / 2 - 80, height / 2 - 20, L'黑');
		outtextxy(width / 2 - 60, height / 2 - 20, L'棋');
		outtextxy(width / 2 - 40, height / 2 - 20, L'悔');
		outtextxy(width / 2 - 20, height / 2 - 20, L'棋');
		outtextxy(width / 2 - 0, height / 2 - 20, L'次');
		outtextxy(width / 2 + 20, height / 2 - 20, L'数');
		outtextxy(width / 2 + 40, height / 2 - 20, L'已');
		outtextxy(width / 2 + 60, height / 2 - 20, L'用');
		outtextxy(width / 2 + 80, height / 2 - 20, L'完');
		outtextxy(width / 2 - 40, height / 2 + 20, L'按');
		outtextxy(width / 2 - 20, height / 2 + 20, L'C');
		outtextxy(width / 2 - 5, height / 2 + 20, L'键');
		outtextxy(width / 2 + 20, height / 2 + 20, L'继');
		outtextxy(width / 2 + 40, height / 2 + 20, L'续');
	}
	EndBatchDraw();
	if (end)Sleep(5000);
}
bool Player::regret(Player& anthor)
{
	if (top == 0)return false;
	if (leftRegretTimes == 0) {
		displayAll(*this, anthor, !this->isBlack, false, false, true);
		ExMessage* msg = new ExMessage;
		while (!peekmessage(msg, EM_KEY) || msg->vkcode != 0x43);
		return false;
	}
	int topX = skX[top - 1], topY = skY[top - 1];
	chess[topX][topY] = 0;
	--top, --leftRegretTimes;
	return true;
}
bool Player::win(int x, int y)
{
	x = toChessBoardX(x), y = toChessBoardY(y);
	int tx = x, ty = y, cnt = 0;
	while (tx >= 0 && chess[tx][y])--tx, ++cnt;
	tx = x;
	while (tx < 21 && chess[tx][y])++tx, ++cnt;
	if (cnt >= 6)return true;//(x,y)处棋子算了两次
	cnt = 0;
	while (ty >= 0 && chess[x][ty])--ty, ++cnt;
	ty = y;
	while (ty < 21 && chess[x][ty])++ty, ++cnt;
	if (cnt >= 6)return true;
	tx = x, ty = y, cnt = 0;
	while (tx >= 0 && ty >= 0 && chess[tx][ty])--tx, --ty, ++cnt;
	tx = x, ty = y;
	while (tx < 21 && ty < 21 && chess[tx][ty])++tx, ++ty, ++cnt;
	if (cnt >= 6)return true;
	tx = x, ty = y, cnt = 0;
	while (tx < 21 && ty >= 0 && chess[tx][ty])++tx, --ty, ++cnt;
	tx = x, ty = y;
	while (tx >= 0 && ty < 21 && chess[tx][ty])--tx, ++ty, ++cnt;
	if (cnt >= 6)return true;
	return false;
}
int main()
{
	initgraph(width, height);
	setbkcolor(WHITE);
	cleardevice();
	Player Black(true), White(false);
	int cnt = 1;
	ExMessage* msg = new ExMessage;
	while (1) {
		if (peekmessage(msg, EM_MOUSE)) {
			int x = msg->x, y = msg->y;
			if (isAtButton(x, y) && msg->lbutton && cnt > 1) {
				bool suc = false;
				if (cnt & 1)suc = White.regret(Black);
				else suc = Black.regret(White);
				if (suc)--cnt;
			}
			if (isAtTheIntersection(x, y) && msg->lbutton) {
				if (cnt & 1) {
					if (!White.existChess(x, y) && !Black.existChess(x, y)) {
						++cnt;
						Black.addChess(x, y);
						if (Black.win(x, y)) {
							displayAll(Black, White, cnt, true, true);
							break;
						}
					}
				}
				else {
					if (!Black.existChess(x, y) && !White.existChess(x, y)) {
						++cnt;
						White.addChess(x, y);
						if (White.win(x, y)) {
							displayAll(Black, White, cnt, true, false);
							break;
						}
					}
				}
			}
		}
		displayAll(Black, White, cnt);
	}
	_getch();
	delete msg;
}
