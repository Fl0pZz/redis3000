#include "redis.h"

#include <gtest/gtest.h>

TEST(RedisValue, Construct) {
    RedisValue integer = 10;
    RedisValue string = "abcd";
    RedisValue error = RedisError("Permission denied");
    RedisValue null = RedisNull();

    RedisValue array = std::vector<RedisValue>{integer, string, error, null};
}

TEST(WriteRedisValue, Int) {
    StringWriter writer(1024);
    RedisValue i = 10;
    WriteRedisValue(&writer, i);
    writer.flush();

    EXPECT_STREQ(":10\r\n", writer.result.c_str());

    writer.result.clear();
    RedisValue j = -5;
    WriteRedisValue(&writer, j);
    writer.flush();

    EXPECT_STREQ(":-5\r\n", writer.result.c_str());

    writer.result.clear();
    RedisValue max = 9223372036854775807;
    WriteRedisValue(&writer, max);
    writer.flush();

    EXPECT_STREQ(":9223372036854775807\r\n", writer.result.c_str());

    writer.result.clear();
    RedisValue min = -9223372036854775807;
    WriteRedisValue(&writer, min);
    writer.flush();

    EXPECT_STREQ(":-9223372036854775807\r\n", writer.result.c_str());
}

TEST(WriteRedisValue, String) {
    StringWriter writer(1024);
    RedisValue str = "abcd";
    WriteRedisValue(&writer, str);
    writer.flush();

    EXPECT_STREQ("+abcd\r\n", writer.result.c_str());

    writer.result.clear();
    str = "123";
    WriteRedisValue(&writer, str);
    writer.flush();

    EXPECT_STREQ("+123\r\n", writer.result.c_str());
}

TEST(WriteRedisValue, Error) {
    StringWriter writer(1024);
    RedisValue str = RedisError("Error message");
    WriteRedisValue(&writer, str);
    writer.flush();

    EXPECT_STREQ("-Error message\r\n", writer.result.c_str());
}

TEST(WriteRedisValue, Null) {
    StringWriter writer(1024);
    RedisValue null = RedisNull();
    WriteRedisValue(&writer, null);
    writer.flush();

    EXPECT_STREQ("$-1\r\n", writer.result.c_str());
}

TEST(WriteRedisValue, Array) {
    StringWriter write(1024);
    RedisValue integer = 10;
    RedisValue string = "abcd";
    RedisValue error = RedisError("Permission denied");
    RedisValue null = RedisNull();

    RedisValue array = std::vector<RedisValue>{integer, string, error, null};
    WriteRedisValue(&write, array);
    write.flush();

    EXPECT_STREQ("*4\r\n:10\r\n+abcd\r\n-Permission denied\r\n$-1\r\n", write.result.c_str());
}


TEST(ReadRedisValue, Int) {
    RedisValue val;

    StringReader reader;

    reader.input = ":10\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(10, boost::get<int64_t>(val));

    reader.input = ":-5\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(-5, boost::get<int64_t>(val));

    reader.input = ":9223372036854775807\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(9223372036854775807, boost::get<int64_t>(val));

    reader.input = ":-9223372036854775807\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(-9223372036854775807, boost::get<int64_t>(val));
}
/*
TEST(ReadRedisValue, Null) {
    RedisValue val;

    StringReader reader;

    reader.input = "$-1\r\n";
    EXPECT_EQ(, boost::get<RedisNull>(val));
}
*/
/*
TEST(ReadRedisValue, String) {
    RedisValue val;

    StringReader reader;

    reader.input = ":OK\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_STREQ("OK", boost::get<int64_t>(val));
}
*/