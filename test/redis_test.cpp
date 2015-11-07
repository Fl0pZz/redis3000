#include "protocol/redis.h"

#include <gtest/gtest.h>

TEST(RedisValue, Construct) {
    RedisValue integer = 10;
    RedisValue string = "abcd";
    RedisValue error = RedisError("Permission denied");
    RedisValue null = RedisNull();
    RedisValue bulk = RedisBulkString("1\0\\1");

    RedisValue array = std::vector<RedisValue>{integer, string, error, null, bulk};
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

TEST(WriteRedisValue, BulkString) {
    StringWriter write(1024);

    RedisValue bulk = RedisBulkString("\t\r\n");
    WriteRedisValue(&write, bulk);
    write.flush();

    EXPECT_STREQ("$3\r\n\t\r\n\r\n", write.result.c_str());
}

TEST(WriteRedisValue, BufOverflow) {
    StringWriter writer(3);

    RedisValue data = RedisBulkString("123456");
    WriteRedisValue(&writer, data);

    EXPECT_STREQ("$6\r\n123456\r\n", writer.result.c_str());
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

TEST(ReadRedisValue, Null) {
    RedisValue val;

    StringReader reader;

    reader.input = "$-1\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(REDIS_NULL, val.which());
}

TEST(ReadRedisValue, String) {
    RedisValue val;

    StringReader reader;

    reader.input = "+OK\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_STREQ("OK", boost::get<std::string>(val).c_str());
}

TEST(ReadRedisValue, Error) {
    RedisValue val;

    StringReader reader;

    reader.input = "-Error message\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_STREQ("Error message", boost::get<RedisError>(val).msg.c_str());

    reader.input = "-123\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_STREQ("123", boost::get<RedisError>(val).msg.c_str());
}

TEST(ReadRedisValue, Array) {
    RedisValue val;

    StringReader reader;

    reader.input = "*4\r\n:10\r\n+abcd\r\n-Permission denied\r\n$-1\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(10, boost::get<int64_t>(boost::get<std::vector<RedisValue>>(val)[0]));
    EXPECT_STREQ("abcd", boost::get<std::string>(boost::get<std::vector<RedisValue>>(val)[1]).c_str());
    EXPECT_STREQ("Permission denied", boost::get<RedisError>(boost::get<std::vector<RedisValue>>(val)[2]).msg.c_str());
    EXPECT_EQ(REDIS_NULL, boost::get<std::vector<RedisValue>>(val)[3].which());
}


TEST(ReadRedisValue, BulkString) {
    RedisValue val;

    StringReader reader;

    reader.input = "$3\r\n\t\r\n\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_STREQ("\t\r\n", boost::get<RedisBulkString>(val).data.c_str());
}
