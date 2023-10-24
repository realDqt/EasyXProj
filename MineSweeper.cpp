#include<easyx.h>
#include<conio.h>
#include<math.h>
#include<time.h>
#include<string.h>
int getRand(int min, int max)//[min,max]
{
	return rand() % (max - min + 1) + min;
}
int dx[4] = { 0,0,-1,1 }, dy[4] = { -1,1,0,0 };
class Mine {
public:
	Mine(int _numOfMine,int _weidth,int _height);
	~Mine();
	void display();
	int aroundMine(int x, int y);
	bool win();
	bool lose();
	void dealMessage(ExMessage* msg);
	int getWidth() { return width; }
	int getHeight() { return height; }
	void dfs(int x, int y);
private:
	int** board;//Î´µã»÷£¬0£»ÓÒ¼ü£¬1,£»×ó¼üÎŞÀ×£¬2£»×ó¼üÓĞÀ×£¬3
	bool** mine, ** vis;
	int numOfMine,width,height;
};
Mine::Mine(int _numOfMine,int _width,int _height):numOfMine(_numOfMine),width(_width),height(_height)
{
	board = new int* [width / 20];
	mine = new bool* [width / 20];
	vis = new bool* [width / 20];
	for (int i = 0; i < width / 20; ++i) {
		board[i] = new int[height / 20];
		mine[i] = new bool[height / 20];
		vis[i] = new bool[height / 20];
	}
	for (int i = 0; i < width / 20; ++i) {
		for (int j = 0; j < height / 20; ++j) {
			board[i][j] = 0;
			vis[i][j]=mine[i][j] = false;
		}
	}
	int cnt = 0;
	while (cnt < numOfMine) {
		int x = getRand(0, width / 20 - 1), y = getRand(0, height / 20 - 1);
		if (mine[x][y])continue;
		mine[x][y] = true;
		++cnt;
	}
}
Mine::~Mine()
{
	for (int i = 0; i < width / 20; ++i)delete[]board[i],delete[]mine[i],delete[]vis;
	delete board,delete mine,delete vis;
}
void Mine::display()
{
	for (int x = 0; x < width / 20; ++x) {
		for (int y = 0; y < height / 20; ++y) {
			switch (board[x][y]) {
			case 0:
				setfillcolor(RGB(192, 192, 192));
				fillrectangle(x * 20, y * 20, (x + 1) * 20, (y + 1) * 20);
				break;
			case 1:
				setfillcolor(BLUE);
				fillrectangle(x * 20, y * 20, (x + 1) * 20, (y + 1) * 20);
				break;
			case 2:
				setfillcolor(GREEN);
				fillrectangle(x * 20, y * 20, (x + 1) * 20, (y + 1) * 20);
				if (aroundMine(x, y)) {
					settextcolor(RED);
					outtextxy(x * 20 + 8, y * 20 + 4, aroundMine(x, y) + 48);
				}
				break;
			case 3:
				setfillcolor(RED);
				fillrectangle(x * 20, y * 20, (x + 1) * 20, (y + 1) * 20);
				break;
			default:
				break;
			}
		}
	}
}
int Mine::aroundMine(int x, int y)
{
	int res = 0;
	for (int i = x - 1; i <= x + 1; ++i) {
		for (int j = y - 1; j <= y + 1; ++j) {
			if (i >= 0 && i < width / 20 && j >= 0 && j < height / 20 && mine[i][j])
				++res;
		}
	}
	return res;
}
bool Mine::win()
{
	int cnt = 0;
	for (int i = 0; i < width / 20; ++i) {
		for (int j = 0; j < height / 20; ++j) {
			if (board[i][j] == 2)++cnt;
		}
	}
	return cnt == ((width / 20) * (height / 20) - numOfMine);
}
bool Mine::lose()
{
	for (int i = 0; i < width / 20; ++i) {
		for (int j = 0; j < height / 20; ++j) {
			if (board[i][j] == 3)return true;
		}
	}
	return false;
}
void Mine::dealMessage(ExMessage* msg)
{
	if (!msg->lbutton && !msg->rbutton)return;
	int x = msg->x / 20, y = msg->y / 20;
	if (msg->lbutton) {
		if (mine[x][y])board[x][y] = 3;//×ó¼üÓĞÀ×
		else dfs(x,y);//×ó¼üÎŞÀ×
	}
	else {
		if (board[x][y] == 0)board[x][y] = 1;//ÓÒ¼ü
		else if (board[x][y] == 1)board[x][y] = 0;//ÖØ¸´ÓÒ¼ü£¬»Ö¸´
	};
}
void Mine::dfs(int x, int y)
{
	//setfillcolor(GREEN);
	//fillrectangle(x * 20, y * 20, (x + 1) * 20, (y + 1) * 20);
	board[x][y] = 2;
	if (aroundMine(x, y))return;//ÖÜÎ§ÓĞÀ×Ôò²»ÔÙÀ©Õ¹
	for (int i = 0; i < 4; ++i) {
		int nx = x + dx[i], ny = y + dy[i];
		if (nx >= 0 && nx < width / 20 && ny >= 0 && ny < height / 20) {
			if (!vis[nx][ny] && !mine[nx][ny]) {
				vis[nx][ny] = true;
				dfs(nx, ny);
			}
		}
	}
}
void draw(Mine& game,bool end=false,bool win=true)
{
	int width = game.getWidth(), height = game.getHeight();
	BeginBatchDraw();
	game.display();
	settextcolor(RED);
	if (end&&win) outtextxy(width / 2 - 10, height / 2 - 10, L'Ó®');
	if (end&&!win) outtextxy(width / 2 - 10, height / 2 - 10, L'Êä');
	EndBatchDraw();
	if (end)Sleep(5000);
}
int main()
{
	initgraph(400, 400);
	setbkcolor(GREEN);
	cleardevice();
	Mine myMine(399, 400, 400);
	ExMessage* msg = new ExMessage;
	while (1) {
		draw(myMine);
		if (peekmessage(msg,EM_MOUSE)){
			myMine.dealMessage(msg);
			if (myMine.win()) {
				draw(myMine, true, true);
				break;
			}
			if (myMine.lose()) {
				draw(myMine, true, false);
				break;
			}
		}
	}
	_getch();
	delete msg;
}