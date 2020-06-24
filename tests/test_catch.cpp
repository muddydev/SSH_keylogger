#include "catch.hpp"
#include "../includes/parsing.hpp"


TEST_CASE("Parse strace line", "[cmd parser]")
{
    boost::regex xRegEx("write\\(\\d+, \"(?<cmd>.*)\", 1\\)\\s+= 1");
    REQUIRE(parsing_utils::parse_strace_line("write(9, \"i\", 1)  = 1\n", xRegEx) == "i");
    REQUIRE(parsing_utils::parse_strace_line("write(9, \"\\177\", 1)  = 1\n", xRegEx) == "[BACKSPACE]");
    REQUIRE(parsing_utils::parse_strace_line("write(9, \"\\3\", 1)  = 1\n", xRegEx) == "[Ctrl+C]^C\n");
    REQUIRE(parsing_utils::parse_strace_line("write(9, \"\\4\", 1)  = 1\n", xRegEx) == "[Ctrl+D]^D\n");
    REQUIRE(parsing_utils::parse_strace_line("write(9, \"\\33[A\", 1)  = 1\n", xRegEx) == "[UP]");
    REQUIRE(parsing_utils::parse_strace_line("write(9, \"\\33OA\", 1)  = 1\n", xRegEx) == "[UP]");
    REQUIRE(parsing_utils::parse_strace_line("write(9, \"\\33[6~\", 1)  = 1\n", xRegEx) == "[PageDOWN]");
    REQUIRE(parsing_utils::parse_strace_line("write(9, \"\\33[5~\", 1)  = 1\n", xRegEx) == "[PageUP]");
}


TEST_CASE("Split output of ps -eaf with 'ssh' substring", "[ps -eaf parser]")
{
    auto splitted_proc_info = std::make_tuple("root", 116, "/usr/sshd", "-D");
    REQUIRE(parsing_utils::split_proc_info("root      116  0.0  0.0  7296   96 ?        Ss   янв27   0:00 /usr/sshd -D") == splitted_proc_info);
    splitted_proc_info = std::make_tuple("user", 183, "/usr/bin/ssh-agent", "/usr/bin/im-launch");
    REQUIRE(parsing_utils::split_proc_info("user   183  0.0  0.0  114    40 ?        Ss   янв27   0:01 /usr/bin/ssh-agent /usr/bin/im-launch env GNOME_SHELL_SESSION_MODE=ubuntu gnome-session --session=ubuntu") == splitted_proc_info);
}

TEST_CASE("Split strings", "[split]")
{
    std::vector<std::string> elems = {"Hello", "a", "nice", "world"};
    REQUIRE(parsing_utils::split_string("Hello a nice world", ' ') == elems);
    REQUIRE(parsing_utils::split_string("Hello a    nice     world", ' ') == elems);
    REQUIRE(parsing_utils::split_string("Helloa    nice     world", ' ') != elems);
    elems = {"user", "183", "0.0", "0.0", "114", "40", "?", "Ss", "янв27"};
    REQUIRE(parsing_utils::split_string("user   183  0.0  0.0  114    40 ?        Ss   янв27", ' ') == elems);
}
