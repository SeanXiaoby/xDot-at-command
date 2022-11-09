#ifndef TESTCOMMANDTERMINAL_H
#define TESTCOMMANDTERMINAL_H

#include <string>
#include <vector>

#include "Commands.h"

class MockSerial
{

public:
    std::string read_buff;
    std::string write_buff;

    int read(char* buff, int len)
    {

        return 0;
    }

    int write(char* buff)
    {
        write_buff.append(buff);
        return 0;
    }

    int writef(char* buff)
    {
        printf("MockSerial writef\r\n");
        write_buff.append(buff);
        return 0;
    }
};

class TestCommandTerminal: public TestCollection
{
public:
    TestCommandTerminal();
    ~TestCommandTerminal();

    virtual void run();

};

TestCommandTerminal::TestCommandTerminal() :
        TestCollection("CommandTerminal")
{
}

TestCommandTerminal::~TestCommandTerminal()
{
}

void TestCommandTerminal::run()
{
    MockSerial test_serial;

    Test::start("Test AT");
    CmdAttention at_cmd;
    Test::assertTrue(std::string("AT").compare(at_cmd.text()) == 0);
    std::vector < std::string > args;
    args.push_back("AT");
    args.push_back("IGNORED");
    Test::assertTrue(at_cmd.verify(args));
    Test::assertTrue(at_cmd.action(args) == 0);
    Test::end();

//    Test::start("Test ATI");
//    printf("testing ati\r\n");
//    CmdIdentification ati_cmd((mts::MTSSerial&)test_serial);
//    printf("cmd created\r\n");
//    Test::assertTrue(at_cmd.verify(args));
//    printf("verified\r\n");
//    Test::assertTrue(at_cmd.action(args) == 0);
//    printf("actionied\r\n");
//    Test::assertTrue(test_serial.write_buff.find("MultiTech") == 0);
//    Test::end();

}

#endif /* TESTCOMMANDTERMINAL_H */
