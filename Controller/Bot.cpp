#include "Bot.hpp"
#include "Response.hpp"
#include <sys/socket.h>
#include <iostream>

Bot::Bot(Request req, int fd, std::string nickname) : user_fd(fd), req(req), nickname(nickname)
{
    this->menuInit();
    unbalancedCnt = menu.size();
}

void    Bot::execute()
{
    std::vector<std::string> param;
    param.push_back(nickname);
    std::string msg = Response::build(req.command().getCommand(), param, this->findmenu(), "BOT");
    send(user_fd, msg.c_str(), msg.size(), 0);
}

std::string Bot::findmenu()
{
    std::srand(std::time(NULL));

    int random = std::rand() % menu.size() - 1;
    std::string str = "드세요 제발";
    
    return (menu[random] += str);
}

void Bot::menuInit()
{
    menu.push_back("비빔밥");
    menu.push_back("불고기");
    menu.push_back("김치찌개");
    menu.push_back("된장찌개");
    menu.push_back("떡볶이");
    menu.push_back("불닭볶음면");
    menu.push_back("제육볶음");
    menu.push_back("김밥");
    menu.push_back("냉면");
    menu.push_back("갈비탕");
    menu.push_back("미역국");
    menu.push_back("돌솥비빔밥");
    menu.push_back("순두부찌개");
    menu.push_back("라면");
    menu.push_back("소고기국밥");
    menu.push_back("해물파전");
    menu.push_back("삼겹살");
    menu.push_back("미역줄기볶음");
    menu.push_back("잡채");
    menu.push_back("감자탕");
    menu.push_back("동파육");
    menu.push_back("보쌈");
    menu.push_back("계란국");
    menu.push_back("오징어볶음");
    menu.push_back("참치김밥");
    menu.push_back("오뎅탕");
    menu.push_back("감자전");
    menu.push_back("해물누룽지탕");
    menu.push_back("닭갈비");
    menu.push_back("돼지국밥");
    menu.push_back("장어구이");
    menu.push_back("찜닭");
    menu.push_back("바지락칼국수");
    menu.push_back("모듬전");
    menu.push_back("훈제오리");
    menu.push_back("불낙전골");
    menu.push_back("갈낙탕");
    menu.push_back("칼국수");
    menu.push_back("삼계탕");
    menu.push_back("육개장");
    menu.push_back("해물탕");
    menu.push_back("돼지불고기");
    menu.push_back("생선구이");
    menu.push_back("제육덮밥");
    menu.push_back("라볶이");
    menu.push_back("야채전");
    menu.push_back("소머리국밥");
    menu.push_back("삼색만두");
    menu.push_back("오징어순대");
    menu.push_back("부대찌개");
    menu.push_back("꽃게탕");
    menu.push_back("어묵");
    menu.push_back("약과");
    menu.push_back("오리주물럭");
    menu.push_back("돼지갈비");
    menu.push_back("떡갈비");
    menu.push_back("샤브샤브");
    menu.push_back("해물찜");
    menu.push_back("갈치구이");
    menu.push_back("코다리조림");
    menu.push_back("어패류전");
    menu.push_back("짜파게티");
    menu.push_back("해파리냉채");
    menu.push_back("토란탕");
    menu.push_back("홍어무침");
    menu.push_back("생선전");
    menu.push_back("소갈비");
    menu.push_back("치킨");
    menu.push_back("소라살");
    menu.push_back("참치회");
    menu.push_back("오이소박이");
    menu.push_back("모듬회");
    menu.push_back("홍어회");
    menu.push_back("아귀찜");
    menu.push_back("동태찌개");
    menu.push_back("팥빙수");
    menu.push_back("녹두전");
    menu.push_back("삼계전");
    menu.push_back("명란젓");
    menu.push_back("계란무침");
    menu.push_back("약식");
    menu.push_back("바베큐");
    menu.push_back("양념게장");
    menu.push_back("미트볼");
    menu.push_back("해초무침");
    menu.push_back("볶음밥");
    menu.push_back("어묵탕");
    menu.push_back("막국수");
    menu.push_back("곱창구이");
    menu.push_back("빈대떡");
    menu.push_back("닭계장");
    menu.push_back("꼬막찜");
    menu.push_back("김치전");
    menu.push_back("파인애플피자");
}