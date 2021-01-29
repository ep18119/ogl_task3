//GLUT使用の宣言
#include <GL/glut.h>
#include <math.h>

#define PI 3.141592

//横方向と縦方向の回転角
double er=5.5,ew=0.8;
//表示する立方体の状態
int stateB = 0;
//各立方体の中心の座標(x,y,z)
GLdouble dispX[27]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
GLdouble dispY[27]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
GLdouble dispZ[27]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//各立方体の天頂角(?)ra,方位角rb,中心からの距離rk,初期回転角ro,回転方向rd
GLdouble ra[27]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
GLdouble rb[27]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
GLdouble rk[27]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
GLdouble ro[27]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
int rd[27]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//再描画イベント発生用の関数
void idle(void)
{
	//glutDisplayFunc()で指定されている描画関数が実行される
	glutPostRedisplay();
}

//立方体の描画　中心の座標x, y, z, 一辺の長さn
void buttai(GLdouble x, GLdouble y, GLdouble z, GLdouble n)
{
	int i,j;
	GLdouble n1 = n/2.0;
	GLdouble n2 = n/2.0*0.9;

	//物体の各面
	GLdouble vertex[24][3] = {
		{x-n1, y-n2, z-n2},{x-n1, y-n2, z+n2},{x-n1, y+n2, z+n2},{x-n1, y+n2, z-n2},
		{x+n1, y-n2, z-n2},{x+n1, y+n2, z-n2},{x+n1, y+n2, z+n2},{x+n1, y-n2, z+n2},
		{x-n2, y-n1, z-n2},{x+n2, y-n1, z-n2},{x+n2, y-n1, z+n2},{x-n2, y-n1, z+n2},
		{x-n2, y+n1, z-n2},{x-n2, y+n1, z+n2},{x+n2, y+n1, z+n2},{x+n2, y+n1, z-n2},
		{x-n2, y-n2, z-n1},{x-n2, y+n2, z-n1},{x+n2, y+n2, z-n1},{x+n2, y-n2, z-n1},
		{x-n2, y-n2, z+n1},{x+n2, y-n2, z+n1},{x+n2, y+n2, z+n1},{x-n2, y+n2, z+n1}
	};
	//物体の各面　表に対して左回り
	int face[18][4] = {
		//立方体の面
		{0,1,2,3},{4,5,6,7},{8,9,10,11},{12,13,14,15},{16,17,18,19},{20,21,22,23},
		//立方体の辺
		{0,8,11,1},{3,17,16,0},{2,13,12,3},{1,20,23,2},
		{4,7,10,9},{5,4,19,18},{6,5,15,14},{7,6,22,21},
		{8,16,19,9},{10,11,20,21},{13,23,22,14},{12,15,18,17}
	};
	//立方体の角
	int face3[8][3] = {
		{0,16,8},{4,9,19},{5,18,15},{3,12,17},
		{1,11,20},{7,21,10},{6,14,22},{2,23,13}
	};
	//法線ベクトル
	GLdouble normal[26][3] = {
		//立方体の面
		{-1.0, 0.0, 0.0},{ 1.0, 0.0, 0.0},
		{ 0.0,-1.0, 0.0},{ 0.0, 1.0, 0.0},
		{ 0.0, 0.0,-1.0},{ 0.0, 0.0, 1.0},
		//立方体の辺
		{-0.7,-0.7, 0.0},{-0.7, 0.0,-0.7},{-0.7, 0.7, 0.0},{-0.7, 0.0, 0.7},
		{ 0.7,-0.7, 0.0},{ 0.7, 0.0,-0.7},{ 0.7, 0.7, 0.0},{ 0.7, 0.0, 0.7},
		{ 0.0,-0.7,-0.7},{ 0.0,-0.7, 0.7},{ 0.0, 0.7, 0.7},{ 0.7, 0.7,-0.7},
		//立方体の角
		{-0.577,-0.577,-0.577},{ 0.577,-0.577,-0.577},
		{ 0.577, 0.577,-0.577},{-0.577, 0.577,-0.577},
		{-0.577,-0.577, 0.577},{ 0.577,-0.577, 0.577},
		{ 0.577, 0.577, 0.577},{-0.577, 0.577, 0.577}
	};
	//面と辺の描画
	glBegin(GL_QUADS);
	//面の数j
	for(j=0; j<18; j++) {
		//法線ベクトルの設定
		glNormal3dv(normal[j]);
		//面の点の数i
		for(i=0; i<4; i++) {
			//4点を指定して面を描画
			glVertex3dv(vertex[face[j][i]]);
		}
	}
	//描画終了
	glEnd();
	//角の描画
	glBegin(GL_TRIANGLES);
	for(j=0; j<8; j++){
		glNormal3dv(normal[j+18]);
		for(i=0; i<3; i++){
			//3点を指定して面を描画
			glVertex3dv(vertex[face3[j][i]]);
		}
	}
	glEnd();
}

//各立方体の描画
void drawB(GLdouble n, GLdouble r)
{
	//立方体の数27
	for(int i=0; i<27; i++){
		GLdouble r0 = ro[i]+r*rd[i];
		GLdouble rk0 = rk[i]*cos(r0*PI/180);
		GLdouble rk1 = rk[i]*sin(r0*PI/180);
		//中心からの距離x,y,z
		GLdouble x0 = rk0*cos(ra[i]*PI/180)*cos((rb[i])*PI/180)-rk1*sin(rb[i]*PI/180);
		GLdouble y0 = rk0*sin(ra[i]*PI/180);
		GLdouble z0 = rk0*cos(ra[i]*PI/180)*sin((rb[i])*PI/180)+rk1*cos(rb[i]*PI/180);
		//立方体を描画
		buttai(dispX[i]+x0,dispY[i]+y0,dispZ[i]+z0,n);
	}
}

void display(void)
{
	//角度
	static double r = 0.0;
	//視点の角度など
	GLfloat lpos[4] = {15.0*cos(ew)*cos(er), 15.0*sin(ew), 15.0*cos(ew)*sin(er), 1.0};
	GLfloat lcol[4] = {0.5, 1.0, 0.5, 1.0};
	GLfloat mcol[4] = {1.0, 0.5, 0.5, 1.0};
	//ウィンドウを塗りつぶす(初期化) 白(引数の初期値)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	//変数行列の初期化
	glLoadIdentity();
	//視点の位置xyz、目標の位置xyz、ウィンドウに表示される画像の上方向を示すベクトル
	gluLookAt(15.0*cos(ew)*cos(er), 15.0*sin(ew), 15.0*cos(ew)*sin(er),
			0.0, 0.0, 0.0,
			-sin(ew)*cos(er), cos(ew), -sin(ew)*sin(er));
	//光源
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	//回転の行列　回転角(弧度法)、回転軸の方向ベクトルxyz
	glRotated(0.0, 0.0, 1.0, 0.0);

	//描画
	drawB(1.0, r);

	//初期化
	glFlush();
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	//描画面の逆転 (2枚の画像を交互に更新・描画している)
	glutSwapBuffers();
	//回転角の更新 尚且つ数値の制限
	r += 0.1;
	if(r >= 360.0) r = 0.0;
}

//ウィンドウサイズに合わせてリサイズ
void resize(int w, int h)
{
	//開いたウィンドウの中で描画が行われる領域　左下隅xy,幅,高さ　だから以下のように書けばいい
	glViewport(0, 0, w, h);
	//設定する変換行列の指定　投影変換行列を指定(GL_PROJECTION)
	glMatrixMode(GL_PROJECTION);
	//変数行列の初期化　ここでも
	glLoadIdentity();
	//透視変換の行列を乗じる　視野角(弧度法),画面の縦横比,奥行き範囲手前と後方の位置
	gluPerspective(30.0, 1.0, 1.0, 30.0);
	//設定する変換行列の指定　モデルビュー変換行列にする(GL_MODELVIEW)
	glMatrixMode(GL_MODELVIEW);
}

//キーボード入力時のイベント処理
void keyboard(unsigned char key, int x, int y)
{
	//Escキーで終了
	if (key == '\033') exit(0);
}

//矢印キー入力時のイベント処理
void specialkey(int key, int x, int y){
	if(key == GLUT_KEY_LEFT){
		er += .1;
		if(er >= 2*PI) er=0;
	}
	else if(key == GLUT_KEY_RIGHT){
		er -= .1;
		if(er <= 0) er = 2*PI;
	}
	else if(key == GLUT_KEY_UP){
		ew += .1;
		if(ew >= 2*PI) ew = 0;
	}
	else if(key == GLUT_KEY_DOWN){
		ew -= .1;
		if(ew <= 0) ew = 2*PI;
	} 
}

//立方体の情報更新
//dispX,dispY,dispZ : 立方体の中心の座標
//ra,rb,ro : 傾き
//rk : 中心から立方体までの距離
//rd : 回転する方向(整数)
void updateC(void){
	int i,j;
	//クリックした回数で表示を変更する
	switch(stateB){
	case 0:
		for(i=0;i<27;i++){
			dispX[i] = 0.0;
			dispY[i] = 0.0;
			dispZ[i] = 0.0;
			ra[i] = 0.0;
			rb[i] = 0.0;
			rk[i] = 0.0;
			ro[i] = 0.0;
			rd[i] = 0;
		}
		break;
	case 1:
		for(i=0;i<27;i++){
			if((i/3)%3 == 0) dispX[i] = 1.0;
			else if((i/3)%3 == 1) dispX[i] = 0.0;
			else dispX[i] = -1.0;
			if(i<9) dispY[i] = 1.0;
			else if(i<18) dispY[i] = 0.0;
			else dispY[i] = -1.0;
			if(i%3 == 0) dispZ[i] = 1.0;
			else if(i%3 == 1) dispZ[i] = 0.0;
			else dispZ[i] = -1.0;
			ra[i] = 0.0;
			rb[i] = 0.0;
			rk[i] = 0.0;
			ro[i] = 0.0;
			rd[i] = 0;
		}
		break;
	case 2:
		dispX[0] = 0.0;
		dispY[0] = 1.5;
		dispZ[0] = 0.0;
		for(i=0;i<4;i++){
			dispY[i+1] = 0.5;
			if(i/2 == 0) dispX[i+1] = 0.5;
			else dispX[i+1] = -0.5;
			if(i%2 == 0) dispZ[i+1] = 0.5;
			else dispZ[i+1] = -0.5;
		}
		for(i=0;i<9;i++){
			dispY[i+5] = -0.5;
			if(i/3 == 0) dispX[i+5] = 1.0;
			else if(i/3 == 1) dispX[i+5] = 0.0;
			else dispX[i+5] = -1.0;
			if(i%3 == 0) dispZ[i+5] = 1.0;
			else if(i%3 == 1) dispZ[i+5] = 0.0;
			else dispZ[i+5] = -1.0;
		}
		for(i=0;i<12;i++){
			dispY[i+14] = -1.5;
			if(i<4) dispX[i+14] = 1.5;
			else if(i<6) dispX[i+14] = 0.5;
			else if(i<8) dispX[i+14] = -0.5;
			else dispX[i+14] = -1.5;
			if(i%2 == 0){
				if(i == 2 || i == 10) dispZ[i+14] = -0.5;
				else dispZ[i+14] = 1.5;
			}else{
				if(i == 1 || i == 9) dispZ[i+14] = 0.5;
				else dispZ[i+14] = -1.5;
			}
		}
		dispX[26] = 0.0;
		dispY[26] = -1.5;
		dispZ[26] = 0.0;
		for(i=0;i<27;i++){
			ra[i] = 0.0;
			rb[i] = 0.0;
			rk[i] = 0.0;
			ro[i] = 0.0;
			rd[i] = 0;
		}
		break;
	case 3:
		for(i=0;i<27;i++){
			dispX[i] = 0.0;
			dispZ[i] = 0.0;
			if(i<9){
				dispY[i] = 1.0;
				rk[i] = 2.0;
				rd[i] = 1;
			}else if(i<18){
				dispY[i] = 0.0;
				rk[i] = 2.0;
				rd[i] = -1;
			}else{
				dispY[i] = -1.0;
				rk[i] = 2.0;
				rd[i] = 1;
			}
			ra[i] = 0.0;
			rb[i] = 0.0;
			ro[i] = 40.0*i;
		}
		break;
        case 4:
		for(i=0;i<6;i++){
			dispX[i] = 0.0;
			dispY[i] = 0.0;
			dispZ[i] = 0.0;
			rk[i] = 2.0;
		}
		ra[0] = 0.0;  rb[0] = 0.0;  ro[0] = 0.0;   rd[0] = 1;
		ra[1] = 0.0;  rb[1] = 0.0;  ro[1] = 180.0; rd[1] = 1;
		ra[2] = 90.0; rb[2] = 0.0;  ro[2] = 90.0;  rd[2] = 1;
		ra[3] = 90.0; rb[3] = 0.0;  ro[3] = 270.0; rd[3] = 1;
		ra[4] = 90.0; rb[4] = 90.0; ro[4] = 0.0;   rd[4] = 1;
		ra[5] = 90.0; rb[5] = 90.0; ro[5] = 180.0; rd[5] = 1;
		for(i=0;i<20;i++){
			if(i<10) dispX[i+6] = 3.0;
			else dispX[i+6] = -3.0;
			dispY[i+6] = 2.0-1.0*(i%5);
			if(i%10<5) dispZ[i+6] = 3.0;
			else dispZ[i+6] = -3.0;
			ra[i+6] = 0.0;
			rb[i+6] = 0.0;
			rk[i+6] = 0.0;
			ro[i+6] = 0.0;
			rd[i+6] = 0;
		}
		dispX[26] = 0.0; dispY[26] = 0.0; dispZ[26] = 0.0;
		ra[26] = 0.0; rb[26] = 0.0; rk[26] = 0.0; ro[26] = 0.0; rd[26] = 0;
		break;
	case 5:
		for(j=0;j<4;j++){
			for(i=0;i<6;i++){
				if(j/2==0) dispX[i+j*6] = 2.0;
				else dispX[i+j*6] = -2.0;
				dispY[i+j*6] = 0.0;
				if(j%2==0) dispZ[i+j*6] = 2.0;
				else dispZ[i+j*6] = -2.0;
				ra[i+j*6] = 0.0;
				rk[i+j*6] = 2.0;
				ro[i+j*6] = 60.0*i;
				if(j%3==0){
					rb[i+j*6] = 0.0;
					rd[i+j*6] = 1;
				}else{
					rb[i+j*6] = 90.0;
					rd[i+j*6] = -1;
				}
			}
		}
		for(i=24;i<27;i++){
			dispX[i] = -2.0;
			dispY[i] = 0.0;
			dispZ[i] = -2.0;
			ra[i] = 0.0;
			rb[i] = 0.0;
			rk[i] = 0.0;
			ro[i] = 0.0;
			rd[i] = 0;
		}
		dispX[24] = 2.0;
		dispZ[26] = 2.0;
		break;
	case 6:
		for(i=0;i<24;i++){
			dispX[i] = 0.0;
			dispY[i] = 2.0*cos(i*15*PI/180);
			dispZ[i] = 2.0*sin(i*15*PI/180);;
			ra[i] = 90.0;
			rb[i] = 0.0;
			rk[i] = 2.0;
			ro[i] = 15.0*i;
			rd[i] = 1;
		}
		for(i=24;i<27;i++){
			dispX[i] = 0.0;
			dispY[i] = 0.0;
			dispZ[i] = 0.0;
			ra[i] = 0.0;
			rb[i] = 0.0;
			rk[i] = 0.0;
			ro[i] = 0.0;
			rd[i] = 0;
		}
		dispX[24] = 1.0;
		dispX[26] = -1.0;
		break;
	case 7:
		for(i=0;i<24;i++){
			dispX[i] = 0.0;
			dispY[i] = 0.0;
			dispZ[i] = 0.0;
			if(i%2==0){
				ra[i] = 45.0;
				rd[i] = 1;
			}else{
				ra[i] = -45.0;
				rd[i] = -1;
			}
			rb[i] = 0;
			rk[i] = 3.0;
			ro[i] = 15.0*i;
		}
		for(i=24;i<27;i++){
			dispX[i] = 0.0;
			dispY[i] = 0.0;
			dispZ[i] = 0.0;
			ra[i] = 0.0;
			rb[i] = 0.0;
			rk[i] = 0.0;
			ro[i] = 0.0;
			rd[i] = 0;
		}
		dispY[24] = 1.5;
		dispY[26] = -1.5;
		break;
	case 8:
		for(i=0;i<27;i++){
			if((i/3)%3 == 0) dispX[i] = 2.5;
			else if((i/3)%3 == 1) dispX[i] = 0.0;
			else dispX[i] = -2.5;
			if(i<9) dispY[i] = 2.5;
			else if(i<18) dispY[i] = 0.0;
			else dispY[i] = -2.5;
			if(i%3 == 0) dispZ[i] = 2.5;
			else if(i%3 == 1) dispZ[i] = 0.0;
			else dispZ[i] = -2.5;
			ra[i] = 0.0;
			rb[i] = 0.0;
			rk[i] = 0.0;
			ro[i] = 0.0;
			rd[i] = 0;
		}
		break;
	default:
		break;
	}
}

//マウスクリック時のイベント処理
void mouse(int button, int state, int x, int y)
{
	switch(button){
	//左クリック時
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP){
			//状態更新
			if(++stateB > 8) stateB = 1;
			//情報更新
			updateC();
		}
		break;
	default:
		break;
	}
}

//main関数
int main(int argc, char *argv[])
{
	//OpenGL環境の初期化
	glutInit(&argc, argv);
	//ディスプレイの表示モード設定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH/*修正*/);
	//ウィンドウを開く 引数はウィンドウの名前
	glutCreateWindow(argv[0]);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	//開いたウィンドウ内に描画する関数
	glutDisplayFunc(display);
	//キーが押された時に実行する関数
	glutKeyboardFunc(keyboard);
	//(矢印)キーが押された時に実行する関数
	glutSpecialFunc(specialkey);
	//マウスイベント時に実行する関数
	glutMouseFunc(mouse);
	//何もすることがないときに実行する関数
	glutIdleFunc(idle);
	//リサイズされた時に実行する関数
	glutReshapeFunc(resize);
	//無限ループ
	glutMainLoop();
}
