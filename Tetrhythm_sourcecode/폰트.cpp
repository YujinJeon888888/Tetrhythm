#include <SDL.h> //SDL 관련 함수들을 사용할 수 있게 된다.
#include <SDL_ttf.h> //이번에 추가한 SDL_TTF 라이브러리
#include <string> //wstring을 사용하기 위해 추가
#include <locale> //현지화 라이브러리

int main(int argc, char** argv) //int main()은 작동하지 않는 경우가 많다. 이렇게 매개변수를 넣어줘야한다.
{
	std::locale::global(std::locale("korean")); //한국어로 로케일 설정
	SDL_Init(SDL_INIT_VIDEO); // SDL 사용 시작
	TTF_Init(); // SDL_TTF 사용 시작
	SDL_Window* window; // 앞으로 만들 윈도우(화면이 그려지는 종이) 창이 들어갈 포인터 변수
	SDL_Renderer* renderer; // 렌더러(화면을 그리는 개체)가 들어갈 포인터 변수
	SDL_Event event;

	window = SDL_CreateWindow("windowName", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0); //윈도우를 만든다.
	renderer = SDL_CreateRenderer(window, -1, 0); //렌더러를 만든다.
	TTF_Font* font = TTF_OpenFont("DungGeunMo.ttf", 9); //해당 경로에 있는 폰트로 객체를 만들어 주소를 반환한다.

	while (1) //무한루프
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				break;
			}
		}

		SDL_Texture* texture;
		SDL_Surface* surface;
		SDL_Rect dst; //dst는 글자가 그려지는 사각형
		dst.x = 90; //글자는 x=90에 그려진다.
		dst.y = 200; //글자는 y=200에 그려진다.

		std::wstring text = L"테스트ABCD";

		Uint16 unicode[128] = { 0, }; //유니코드가 들어갈 배열, 넉넉하게 128칸으로 만들었다.
		for (int i = 0; i < text.size() && i <= 127; i++)
		{
			unicode[i] = text[i]; //wstring의 글자마다의 값들을 유니코드 배열에다가 입력해준다.
		}
		SDL_Color mint = { 0xaa,0xf0,0xd1 };
		surface = TTF_RenderUNICODE_Blended(font, unicode, mint); //해당 폰트로 유니코드 배열의 글자들을 그린다. 서피스로 반환됨
		texture = SDL_CreateTextureFromSurface(renderer, surface); //서피스를 텍스쳐로 변환
		SDL_FreeSurface(surface); //변환 후에 필요없어진 서피스 해제

		SDL_QueryTexture(texture, nullptr, nullptr, &dst.w, &dst.h); //만든 글자의 너비와 높이값을 자동으로 dst.w, dst.h로 옮긴다.
		SDL_RenderCopy(renderer, texture, nullptr, &dst); //dst만큼 글자를 그려낸다.

		SDL_Delay(1); //1밀리초 정지, 정지하는 시간이 짧아지면 게임의 프레임 수가 올라가고 길어지면 프레임 수가 짧아집니다.
		SDL_RenderPresent(renderer); //변경된 렌더러를 업데이트합니다.
	}

	SDL_DestroyRenderer(renderer); //렌더러 파괴
	SDL_DestroyWindow(window); //윈도우 파괴
	TTF_Quit(); //TTF 종료
	SDL_Quit(); //SDL 종료
	return 0;
}