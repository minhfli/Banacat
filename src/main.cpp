#include <iostream>
#include <Main_Game.h>

#include <Common/Json_parse.h>

int main(int argc, char* argv[]) {

    readfile("Assets/test.json");
    sk_engine::Init();
    sk_engine::Run();
    sk_engine::Quit();

    return 0;
}