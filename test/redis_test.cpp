#include <protocol/redis.h>

#include <gtest/gtest.h>

TEST(RedisValue, Construct) {
    RedisValue integer = 10;
    RedisValue string = "abcd";
    RedisValue error = RedisError("Permission denied");
    RedisValue array = std::vector<RedisValue>(3);
    boost::get<std::vector<RedisValue>>(array)[0] = integer;
    boost::get<std::vector<RedisValue>>(array)[1] = error;
    boost::get<std::vector<RedisValue>>(array)[2] = string;
}

TEST(WriteRedisValue, Int) {
    RedisValue i = 10, j = -5;
    StringWriter writer(1024);

    WriteRedisValue(&writer, i);
    writer.flush();
    EXPECT_STREQ(":10\r\n", writer.result.c_str());
    writer.result.clear();

    WriteRedisValue(&writer, j);
    writer.flush();
    EXPECT_STREQ(":-5\r\n", writer.result.c_str());
    writer.result.clear();
}

TEST(WriteRedisValue, String) {
    RedisValue string = "abcd";
    StringWriter writer(1024);

    WriteRedisValue(&writer, string);
    writer.flush();
    EXPECT_STREQ("+abcd\r\n", writer.result.c_str());
    writer.result.clear();
}

TEST(WriteRedisValue, Null) {
    RedisValue null_value = RedisNull();
    StringWriter writer(1024);

    WriteRedisValue(&writer, null_value);
    writer.flush();
    EXPECT_STREQ("$-1\r\n", writer.result.c_str());
    writer.result.clear();
}

TEST(WriteRedisValue, Error) {
    RedisValue error = RedisError("error");
    StringWriter writer(1024);

    WriteRedisValue(&writer, error);
    writer.flush();
    EXPECT_STREQ("-error\r\n", writer.result.c_str());
    writer.result.clear();
}

TEST(WriteRedisValue, BulkString) {
    RedisValue bulk_string = RedisBulkString("abcd");
    StringWriter writer(1024);

    WriteRedisValue(&writer, bulk_string);
    writer.flush();
    EXPECT_STREQ("$4\r\nabcd\r\n", writer.result.c_str());
    writer.result.clear();
}

TEST(WriteRedisValue, Array) {
    RedisValue integer = 10;
    RedisValue string = "abcd";
    RedisValue error = RedisError("error");

    RedisValue array = std::vector<RedisValue>{integer, error, string};
    StringWriter writer(1024);

    WriteRedisValue(&writer, array);
    writer.flush();
    EXPECT_STREQ("*3\r\n:10\r\n-error\r\n+abcd\r\n", writer.result.c_str());
    writer.result.clear();

    RedisValue big_array = std::vector<RedisValue>{array, integer};

    WriteRedisValue(&writer, big_array);
    writer.flush();
    EXPECT_STREQ("*2\r\n*3\r\n:10\r\n-error\r\n+abcd\r\n:10\r\n", writer.result.c_str());
    writer.result.clear();
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
}

TEST(ReadRedisValue, String) {
    RedisValue val;
    StringReader reader;

    reader.input = "+abc.xyz\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ("abc.xyz", boost::get<std::string>(val));

    reader.input = "+example\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ("example", boost::get<std::string>(val));
}

TEST(ReadRedisValue, Error) {
    RedisValue val;
    StringReader reader;

    reader.input = "-example\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ("example", boost::get<RedisError>(val).msg);
}

TEST(ReadRedisValue, BulkString) {
    RedisValue val;
    StringReader reader;

    reader.input = "$7\r\nabc.xyz\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ("abc.xyz", boost::get<std::string>(val));

    reader.input = "$15\r\nexample\r\nstring\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ("example\r\nstring", boost::get<std::string>(val));

    reader.input = "$0\r\n\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ("", boost::get<std::string>(val));
}

TEST(ReadRedisValue, Null) {
    RedisValue val;
    StringReader reader;

    reader.input = "$-1\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(REDIS_NULL, val.which());
}

TEST(ReadRedisValue, Array) {
    RedisValue val;
    StringReader reader;

    reader.input = "*5\r\n"
            ":3\r\n"
            "+foobar\r\n"
            "$6\r\nfoobar\r\n"
            "-error\r\n"
            "$-1\r\n";
    ReadRedisValue(&reader, &val);
    EXPECT_EQ(3, boost::get<int64_t>(boost::get<std::vector<RedisValue>>(val)[0]));
    EXPECT_EQ("foobar", boost::get<std::string>(boost::get<std::vector<RedisValue>>(val)[1]));
    EXPECT_EQ("foobar", boost::get<std::string>(boost::get<std::vector<RedisValue>>(val)[2]));
    EXPECT_EQ("error", boost::get<RedisError>(boost::get<std::vector<RedisValue>>(val)[3]).msg);
    EXPECT_EQ(REDIS_NULL, boost::get<std::vector<RedisValue>>(val)[4].which());

}


