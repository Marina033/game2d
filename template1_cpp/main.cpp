#include "common.h"

#include <GLFW/glfw3.h>
#include <iostream>

#include "Image.h"
#include "Player.h"
#include "Enemy.h"
#include "windows.h"

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int startPlayerX{}, startPlayerY{}, posExitX{}, posExitY{}, startEnemyX{}, startEnemyY{};
bool flag_stop{}, flag_fall{}, flag_exit{}, flag_final_exit{};
bool exU{}, exD{}, exR{}, exL{};
bool flag_right = true, flag_left{}, flag_up = true, flag_down{};
int curX = 0;
int curY = WINDOW_HEIGHT - TILE_HEIGHT;
double EPSILON = 0.00001;


struct InputState
{
	bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
	GLfloat lastX = 400, lastY = 300; //исходное положение мыши
	bool firstMouse = true;
	bool captureMouse = true;  // Мышка захвачена нашим приложением или нет?
	bool capturedMouseJustNow = false;
} static Input;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		if (action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_1:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		break;
	case GLFW_KEY_2:
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		break;
	default:
		if (action == GLFW_PRESS)
			Input.keys[key] = true;
		else if (action == GLFW_RELEASE)
			Input.keys[key] = false;
	}
}

void processPlayerMovement(Player& player, std::string& chars)
{
	if (Input.keys[GLFW_KEY_W])
		player.ProcessInput(MovementDir::UP, chars, flag_stop, flag_fall, flag_exit, flag_final_exit, exU, exD, exR, exL);
	else if (Input.keys[GLFW_KEY_S])
		player.ProcessInput(MovementDir::DOWN, chars, flag_stop, flag_fall, flag_exit, flag_final_exit, exU, exD, exR, exL);
	if (Input.keys[GLFW_KEY_A])
		player.ProcessInput(MovementDir::LEFT, chars, flag_stop, flag_fall, flag_exit, flag_final_exit, exU, exD, exR, exL);
	else if (Input.keys[GLFW_KEY_D])
		player.ProcessInput(MovementDir::RIGHT, chars, flag_stop, flag_fall, flag_exit, flag_final_exit, exU, exD, exR, exL);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
		Input.captureMouse = !Input.captureMouse;

	if (Input.captureMouse)
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		Input.capturedMouseJustNow = true;
	}
	else
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
	if (Input.firstMouse)
	{
		Input.lastX = float(xpos);
		Input.lastY = float(ypos);
		Input.firstMouse = false;
	}

	GLfloat xoffset = float(xpos) - Input.lastX;
	GLfloat yoffset = Input.lastY - float(ypos);

	Input.lastX = float(xpos);
	Input.lastY = float(ypos);
}

void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	// ...
}

int initGL()
{
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	std::cout << "Controls: " << std::endl;
	std::cout << "press right mouse button to capture/release mouse cursor  " << std::endl;
	std::cout << "W, A, S, D - movement aaaaaaaaaaaaaaaaaa " << std::endl;
	std::cout << "press ESC to exit" << std::endl;

	return 0;
}

void drawTile(const int curX, const int curY, Image& screenBuffer, Image& picture) {
	for (int i = 0; i < picture.Width(); ++i)
		for (int j = 0; j < picture.Height(); ++j)
			screenBuffer.PutPixel(curX + i, curY + j, picture.GetPixel(i, j));

}

void openFiles(const std::string& path, std::string& chars) {
	std::ifstream room_file(path);
	char c;
	if (room_file.is_open()) {
		while (room_file.get(c)) {
			chars.push_back(c);
		}
	}
	else { std::cout << "ERROR::COULDN'T OPEN FILE:" << path << std::endl; }
}

void drawRoom(std::string chars, Image& screenBuffer, Image& floor, Image& wall, Image& man, 
	Image& thorn, Image& exit, Image& finalExit, Image& monster) {

	curX = 0;
	curY = WINDOW_HEIGHT - TILE_HEIGHT;

	startPlayerX = 0;
	startPlayerY = posExitY;

	for (int i = 0; i < chars.size(); i++) {

		switch (chars[i]) {
		case ' ':
			break;
		case '.':
			drawTile(curX, curY, screenBuffer, floor);
			break;
		case '#':
			drawTile(curX, curY, screenBuffer, wall);
			break;
		case '@':
			startPlayerX = curX;
			startPlayerY = curY;
			break;
		case 'T':
			drawTile(curX, curY, screenBuffer, thorn);
			break;
		case 'x':
			posExitX = 0;
			posExitY = curY;
			drawTile(curX, curY, screenBuffer, exit);
			break;
		case 'Q':
			drawTile(curX, curY, screenBuffer, finalExit);
			break;
		case 'e':
			startEnemyX = curX;
			startEnemyY = curY;
			drawTile(curX, curY, screenBuffer, monster);
			break;
		default: break;
		}
		if (chars[i] == '\n') {
			curX = 0;
			curY -= wall.Height();
		}
		else {
			curX += wall.Width();

		}
	}
}

void restoreBackGround(Image& screenBuffer, Image& picture, Player& man) {

	int numY = (int) trunc(man.GetOldCoords().y / TILE_HEIGHT); // here should trunc all brackets
	int numX = (int) trunc(man.GetOldCoords().x / TILE_WIDTH); // here should trunc all brackets
	if (man.GetOldCoords().y % 32 == 0 && man.GetOldCoords().x % 32 == 0) {
		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
	}
	else if (man.GetOldCoords().y % 32 == 0) {
		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile((numX + 1) * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
	} 
	else if (man.GetOldCoords().x % 32 == 0) {
		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile(numX * TILE_WIDTH, (numY + 1) * TILE_HEIGHT, screenBuffer, picture);
	}
	else {

		drawTile(numX * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile((numX + 1) * TILE_WIDTH, numY * TILE_HEIGHT, screenBuffer, picture);
		drawTile(numX * TILE_WIDTH, (numY + 1) * TILE_HEIGHT, screenBuffer, picture);
		drawTile((numX + 1) * TILE_WIDTH, (numY + 1) * TILE_HEIGHT, screenBuffer, picture);
	}
}

int main(int argc, char** argv) {
	if (!glfwInit())
		return -1;
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, OnKeyboardPressed);
	glfwSetCursorPosCallback(window, OnMouseMove);
	glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
	glfwSetScrollCallback(window, OnMouseScroll);
	if (initGL() != 0)
		return -1;
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();



	Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);// 4 - number of channels
	Image floor("resources\\floor.png");
	Image wall("resources\\wall.png");
	Image man("resources\\man.png");
	Image thorn("resources\\thorn.png");
	Image exit("resources\\exit.png");
	Image finalExit("resources\\finalExit.jpg");
	Image gameOver("resources\\gameOver.jpg");
	Image gameWin("resources\\win.jpg");
	Image noExit("resources\\noExit.jpg");
	Image monster("resources\\monster.jpg");

	std::string chars1 = "", chars2 = "", chars3 = "", chars4 = "", arrOfTypes = "";
	openFiles("resources\\room1.txt", chars1);
	openFiles("resources\\room2.txt", chars2);
	openFiles("resources\\room3.txt", chars3);
	openFiles("resources\\room4.txt", chars4);
	openFiles("resources\\common_plan.txt", arrOfTypes);

	int indexOfTypes{};
	std::string charsCur;
	switch (arrOfTypes[indexOfTypes]) {
	case 'A':
		charsCur = chars1;
		break;
	case 'B':
		charsCur = chars2;
		break;
	case 'C':
		charsCur = chars3;
		break;
	case 'D':
		charsCur = chars4;
		break;
	}
	drawRoom(charsCur, screenBuffer, floor, wall, man, thorn, exit, finalExit, monster);

	Point startPlayerPos{ .x = startPlayerX, .y = startPlayerY };
	Point startEnemyPos{ .x = startEnemyX, .y = startEnemyY };

	Player player{ startPlayerPos };
	Enemy enemy{ startEnemyPos };
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);	GL_CHECK_ERRORS;

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		GLfloat currentFrame = (GLfloat)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
		glfwPollEvents();

		processPlayerMovement(player, charsCur);
		restoreBackGround(screenBuffer, floor, player);
		player.DrawOfPlayer(screenBuffer, floor, man);
		if (arrOfTypes[indexOfTypes] == 'A') {
			if (flag_right && enemy.GetCoords().x <= 400) enemy.ProcessInput(MovementDir::RIGHT);
			else { flag_right = false; flag_left = true; }
			if (flag_left && enemy.GetCoords().x >= 40) enemy.ProcessInput(MovementDir::LEFT);
			else { flag_right = true; flag_left = false; }
			enemy.restoreBackGroundEnemy(screenBuffer, floor);
			enemy.DrawOfEnemy(screenBuffer, floor, monster);
			Point a{ .x = player.GetCoords().x, .y = player.GetCoords().y };
			Point b{ .x = enemy.GetCoords().x, .y = enemy.GetCoords().y };
			int x1 = a.x, x2 = b.x, y1 = a.y, y2 = b.y;
			if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < 32) flag_fall = true;
		}

		if (arrOfTypes[indexOfTypes] == 'B') {
			if (flag_up && enemy.GetCoords().y <= 400) enemy.ProcessInput(MovementDir::UP);
			else { flag_up = false; flag_down = true; }
			if (flag_down && enemy.GetCoords().y >= 40) enemy.ProcessInput(MovementDir::DOWN);
			else { flag_up = true; flag_down = false; }
			enemy.restoreBackGroundEnemy(screenBuffer, floor);
			enemy.DrawOfEnemy(screenBuffer, floor, monster);
			Point a{ .x = player.GetCoords().x, .y = player.GetCoords().y };
			Point b{ .x = enemy.GetCoords().x, .y = enemy.GetCoords().y };
			int x1 = a.x, x2 = b.x, y1 = a.y, y2 = b.y;
			if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < 32) flag_fall = true;
		}

		

		if (flag_fall) {
			curX = 0;
			curY = 0;
			drawTile(curX, curY, screenBuffer, gameOver);
			glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
			glfwSwapBuffers(window);
			Sleep(2000);
			break;
		}

		if (flag_exit) {
			flag_stop = false;
			flag_fall = false;
			flag_exit = false;
			flag_final_exit = false;
			curX = 0;
			curY = 0;
			
			if (exR) { startPlayerX = 0; startPlayerY = player.GetCoords().y; indexOfTypes++; }
			if (exL) { startPlayerX = WINDOW_WIDTH - TILE_WIDTH; startPlayerY = player.GetCoords().y; indexOfTypes--; }
			if (exU) { startPlayerX = player.GetCoords().x; startPlayerY = 0; indexOfTypes -= 6; } // 5 - size of string and 1 '\n'
			if (exD) { startPlayerX = player.GetCoords().x; startPlayerY = WINDOW_HEIGHT - TILE_HEIGHT; indexOfTypes += 6; }
			
			player.PutCoords(startPlayerX, startPlayerY);
			if (indexOfTypes >= 0 && indexOfTypes < arrOfTypes.size()) {
				switch (arrOfTypes[indexOfTypes]) {
				case 'A':
					charsCur = chars1;
					break;
				case 'B':
					charsCur = chars2;
					break;
				case 'C':
					charsCur = chars3;
					break;
				case 'D':
					charsCur = chars4;
					break;
				}
				if (player.Elem(charsCur, startPlayerX / TILE_WIDTH, startPlayerY / TILE_HEIGHT) == 'x') {
					drawRoom(charsCur, screenBuffer, floor, wall, man, thorn, exit, finalExit, monster);
					enemy.PutCoords(startEnemyX, startEnemyY);
				}
				else {
					curX = 0;
					curY = 0;
					drawTile(curX, curY, screenBuffer, noExit);
					glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
					glfwSwapBuffers(window);
					Sleep(2000);
					break;
				}
			}
			else {
				curX = 0;
				curY = 0;
				drawTile(curX, curY, screenBuffer, noExit);
				glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
				glfwSwapBuffers(window);
				Sleep(2000);
				break;
				
			}
		}

		if (flag_final_exit) {
			curX = 0;
			curY = 0;
			drawTile(curX, curY, screenBuffer, gameWin);
			glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
			glfwSwapBuffers(window);
			Sleep(2000);
			break;
		}




		glClearColor(0.0f, 1.0f, 1.0f, 1.0f); GL_CHECK_ERRORS;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
		glDrawPixels(WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
