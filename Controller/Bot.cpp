#include "Bot.hpp"

#include <sys/socket.h>

#include "Response.hpp"
#include "Server.hpp"

Bot::Bot(Request req, int fd, std::string nickname)
    : user_fd(fd), req(req), nickname(nickname) {
  this->menuInit();
}

void Bot::execute() {
  std::vector<std::string> param;
  param.push_back(nickname);
  std::string msg =
      Response::build(req.command().getCommand(), param,
                      this->findmenu(req.parameter().getTrailer()), "BOT-42");
  fd_set fd_write;
  FD_ZERO(&fd_write);
  FD_SET(user_fd, &fd_write);
  Server::getInstance().bufferMessage(msg, 1, &fd_write);
}

std::string Bot::findmenu(std::string str) {
  int random = 0;
  std::string menu;
  std::srand(std::time(NULL));
  if (str == "아침") {
    random = std::rand() % breakfast.size();
    menu = breakfast[random];
  } else if (str == "점심") {
    random = std::rand() % lunch.size();
    menu = lunch[random];
  } else if (str == "저녁") {
    random = std::rand() % dinner.size();
    menu = dinner[random];
  } else {
    return ("'아침', '점심', '저녁' 메뉴만 추천해드립니다");
  }

  std::string plz = "드세요 제발";

  return (menu += plz);
}

void Bot::menuInit() {
  breakfast.push_back("비빔밥");
  breakfast.push_back("불고기");
  breakfast.push_back("김치찌개");
  breakfast.push_back("된장찌개");
  breakfast.push_back("떡볶이");
  breakfast.push_back("호밀빵에 아보카도");
  breakfast.push_back("그래놀라 요거트");
  breakfast.push_back("오트밀");
  breakfast.push_back("팬케이크");
  breakfast.push_back("와플");
  breakfast.push_back("베이글과 크림치즈");
  breakfast.push_back("프렌치 토스트");
  breakfast.push_back("에그 베네딕트");
  breakfast.push_back("스크램블 에그");
  breakfast.push_back("오믈렛");
  breakfast.push_back("프루트 샐러드");
  breakfast.push_back("스무디 볼");
  breakfast.push_back("아보카도 토스트");
  breakfast.push_back("콘플레이크");
  breakfast.push_back("블루베리 머핀");
  breakfast.push_back("크로와상");
  breakfast.push_back("시나몬 롤");
  breakfast.push_back("햄 치즈 샌드위치");
  breakfast.push_back("퀴노아 샐러드");
  breakfast.push_back("닭가슴살 샐러드");
  breakfast.push_back("브런치 플래터");
  breakfast.push_back("모닝 버거");
  breakfast.push_back("새우 아보카도 샐러드");
  breakfast.push_back("치즈 오믈렛");
  breakfast.push_back("초콜릿 크로와상");
  lunch.push_back("치킨 샐러드");
  lunch.push_back("햄버거");
  lunch.push_back("치즈버거");
  lunch.push_back("쌀국수");
  lunch.push_back("파스타");
  lunch.push_back("피자");
  lunch.push_back("스시");
  lunch.push_back("돈부리");
  lunch.push_back("카레라이스");
  lunch.push_back("빠에야");
  lunch.push_back("포케 볼");
  lunch.push_back("라멘");
  lunch.push_back("브리또");
  lunch.push_back("클럽 샌드위치");
  lunch.push_back("팔라펠 랩");
  lunch.push_back("타코");
  lunch.push_back("케밥");
  lunch.push_back("비프 스튜");
  lunch.push_back("콥 샐러드");
  lunch.push_back("시저 샐러드");
  lunch.push_back("비비큐 리브");
  lunch.push_back("스테이크");
  lunch.push_back("감바스 알 아히요");
  lunch.push_back("짜장면");
  lunch.push_back("짬뽕");
  lunch.push_back("볶음밥");
  lunch.push_back("김밥");
  lunch.push_back("불고기");
  lunch.push_back("돌솥비빔밥");
  lunch.push_back("회덮밥");
  dinner.push_back("로스트 치킨");
  dinner.push_back("해산물 파스타");
  dinner.push_back("그릴드 살몬");
  dinner.push_back("비프 웰링턴");
  dinner.push_back("라자냐");
  dinner.push_back("순두부 찌개");
  dinner.push_back("갈비찜");
  dinner.push_back("삼겹살 구이");
  dinner.push_back("닭볶음탕");
  dinner.push_back("멕시칸 타코");
  dinner.push_back("페퍼 스테이크");
  dinner.push_back("토마토 스파게티");
  dinner.push_back("쉬림프 리조또");
  dinner.push_back("덕 컨핏");
  dinner.push_back("팟타이");
  dinner.push_back("마르게리타 피자");
  dinner.push_back("치킨 티카 마살라");
  dinner.push_back("잡채");
  dinner.push_back("냉면");
  dinner.push_back("스페인식 오믈렛");
  dinner.push_back("바비큐 포크");
  dinner.push_back("닭 갈비");
  dinner.push_back("오소 부코");
  dinner.push_back("연어 타르타르");
  dinner.push_back("허니 갈릭 치킨");
  dinner.push_back("코코뱅");
  dinner.push_back("탄두리 치킨");
  dinner.push_back("가지 파르마");
  dinner.push_back("무스카카");
  dinner.push_back("킹 프론 알 아히요");
}