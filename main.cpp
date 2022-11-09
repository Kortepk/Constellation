#include <SFML/Graphics.hpp>  
#include <iostream>
#include <cmath>

#define ANTI_ALIASING 8
#define NUM_STARS 150
#define MATH_PI 3.1415926535

using namespace sf;  
using namespace std;

int WIDTH = 1550, HEIGHT = 800;

RenderWindow window(VideoMode(WIDTH, HEIGHT), "Part", Style::Default, ContextSettings(0, 0, ANTI_ALIASING));

string Float_to_string(float x){
	string text = "", text2 = "";
	
	if(x < 0)
		text = "-";
	
	int a, b;
	a = x;
	x = x - a;
	a = abs(a);

	if(a == 0)
		text = "0";
		
	while(a != 0){
		b = a % 10;
		char ch(48 + b);
		text2 = ch + text2;
		a /= 10;
	}
	
	text += text2 + ".";
	text2 = "";
	
	a = abs(x * 100);
	while(a != 0){
		b = a % 10;
		char ch(48 + b);
		text2 = ch + text2;
		a /= 10;
	}
	text += text2;
	return text;
}

class Star{
	public:
		int radius = 10;
		float screen_pos[2]; // x y 
		float speed[2]; // speed x, speed y
		
		Star();
		void Draw();
		void Update();
};

Star::Star(){ 
	screen_pos[0] = rand() % (WIDTH + 1);	// от 0 до WIDTH включительно
	screen_pos[1] = rand() % (HEIGHT + 1);	
	
	speed[0] = 10 + rand() % 50;
	speed[0] /= 100;
	speed[1] = 10 + rand() % 50;
	speed[1] /= 100;
		
	if(rand() % 2 == 1)
		speed[0] = -speed[0];
	if(rand() % 2 == 1)
		speed[1] = -speed[1];
}

void Star::Update(){
	screen_pos[0] += speed[0];
	screen_pos[1] += speed[1];
	
	if(screen_pos[0] < -10)
		screen_pos[0] = WIDTH + 10;
	if(screen_pos[0] > WIDTH + 10)
		screen_pos[0] = -10;
	if(screen_pos[1] < -10)
		screen_pos[1] = HEIGHT + 10;
	if(screen_pos[1] > HEIGHT + 10)
		screen_pos[1] = -10;
}

void Star::Draw(){
	CircleShape circle(radius); 
	circle.setFillColor(Color(255, 0, 255, 10));  //Закрашиваем наш круг (RGBA)		
	circle.setPosition(screen_pos[0] - radius, screen_pos[1] - radius);
	
	for(int i = radius; i > 3; i--){
		circle.setRadius(i);
		window.draw(circle);  //Отрисовка круга
		circle.move(1, 1);
	}
	circle.setFillColor(Color(255, 255, 255, 40));
	circle.setRadius(3);
	window.draw(circle);
	
	circle.move(1, 1);

	circle.setRadius(2);
	circle.setFillColor(Color(255, 255, 255));
	window.draw(circle);
	/*
	Font font;
	font.loadFromFile("arkhip.ttf");
	Text text(Float_to_string(speed[0]) + "#" + Float_to_string(speed[1]), font, 12);

	text.setPosition(screen_pos[0] + 10, screen_pos[1] - 15);
	window.draw(text);*/
}

class Starfield{
	public:
		Star stars[NUM_STARS];
		void Run();
		void Line(int i, int j, float Rad);
		Starfield();
};

Starfield::Starfield(){
	stars[0].speed[0] = 0; // "звезда" пользователя
	stars[0].speed[1] = 0;
}

void Starfield::Line(int i, int j, float rad){
	float grad = 255 - (rad - 22) * 2;
	
	if(grad > 255)
		grad = 255;
		
	Vertex line_without_thickness[] =
	{
		Vertex(Vector2f(stars[i].screen_pos[0], stars[i].screen_pos[1])),  //Координата первой вершины
		Vertex(Vector2f(stars[j].screen_pos[0], stars[j].screen_pos[1]))  //Координата второй вершины
	};
	line_without_thickness[0].color = Color(255, 255, 255, grad);
	line_without_thickness[1].color = Color(255, 255, 255, grad);
	
	window.draw(line_without_thickness, 2, Lines);  //Отрисовка линии
	/*
	line_without_thickness[0] = Vertex(Vector2f(stars[i].screen_pos[0] + 1, stars[i].screen_pos[1] + 1));  //Координата первой вершины
	line_without_thickness[1] =	Vertex(Vector2f(stars[j].screen_pos[0] - 1, stars[j].screen_pos[1] - 1));  //Координата второй вершины
	line_without_thickness[0].color = Color(255, 255, 255, grad/2);
	line_without_thickness[1].color = Color(255, 255, 255, grad/2);

	window.draw(line_without_thickness, 2, Lines);  //Отрисовка линии
	
	line_without_thickness[0] = Vertex(Vector2f(stars[i].screen_pos[0] - 1, stars[i].screen_pos[1] - 1));  //Координата первой вершины
	line_without_thickness[1] =	Vertex(Vector2f(stars[j].screen_pos[0] + 1, stars[j].screen_pos[1] + 1));  //Координата второй вершины
	
	window.draw(line_without_thickness, 2, Lines);  //Отрисовка линии
*/
}

void Starfield::Run(){
	int i, j;
	//for(int i=0; i<NUM_STARS;i++)
	//	stars[i].update();
	
	for(i=0; i<NUM_STARS;i++)
		for(j=i+1; j<NUM_STARS;j++){	// если расстояние между точками < 90
			float Rad = sqrt(pow(stars[i].screen_pos[0] - stars[j].screen_pos[0], 2) + pow(stars[i].screen_pos[1] - stars[j].screen_pos[1], 2));
			if(Rad < 150)
				Starfield::Line(i, j, Rad);
		}
	
	for(i=0; i<NUM_STARS;i++){
		stars[i].Draw();
		stars[i].Update();
	}
}

int main()
{	
	window.setFramerateLimit(120);
	window.setKeyRepeatEnabled(true);
	
	Starfield starfield;
		
	while (window.isOpen())  
	{
		Event event;  
		while (window.pollEvent(event))  
		{
			if (event.type == Event::Closed)  
				window.close(); 
				
			if (event.type == Event::Resized)  
			{
	            WIDTH = event.size.width;
	            HEIGHT = event.size.height;        
	            window.setView(
	                View(
	                   Vector2f(WIDTH / 2.0, HEIGHT / 2.0), 
	                   Vector2f(WIDTH, HEIGHT)
	                )
	            );
	        }
	        
	        if (event.type == sf::Event::MouseMoved){
				starfield.stars[0].screen_pos[0] = event.mouseMove.x;
				starfield.stars[0].screen_pos[1] = event.mouseMove.y; 
			}
		}
		
		Vertex line_without_thickness_but[] =
		{
			Vertex(Vector2f(0, 0)),  
			Vertex(Vector2f(WIDTH, 0)),  
			Vertex(Vector2f(WIDTH, HEIGHT)), 
			Vertex(Vector2f(0, HEIGHT))
		};
	
		line_without_thickness_but[0].color = Color(100, 0, 100); 
		line_without_thickness_but[1].color = Color(80, 0, 85); 
		line_without_thickness_but[2].color = Color(12, 0, 43); 
		line_without_thickness_but[3].color = Color(10, 0, 10); 
		window.draw(line_without_thickness_but, 4, Quads);
		
		//Otris();
		starfield.Run();
		
		window.display();
	}
	return 0;
}