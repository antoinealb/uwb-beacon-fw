#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>
#include "dw1000/dw1000.h"
#include "mocks.h"

void mock_write(void *arg, uint8_t *buffer, size_t len)
{
    mock("dw1000").actualCall("write")
                  .withPointerParameter("arg", arg)
                  .withMemoryBufferParameter("buffer", buffer, len);
}

void mock_read(void *arg, uint8_t *buffer, size_t len)
{
    mock("dw1000").actualCall("read")
                  .withPointerParameter("arg", arg)
                  .withOutputParameter("buffer", buffer);
}

void mock_cs_acquire(void *arg, bool acquired)
{
    mock("dw1000").actualCall("cs_acquire")
                  .withPointerParameter("arg", arg)
                  .withIntParameter("acquired", acquired);
}

TEST_GROUP(DW1000MockTestGroup)
{
};

TEST(DW1000MockTestGroup, Write)
{
    uint8_t buffer[] = "foobar";

    mock("dw1000").expectOneCall("write")
                  .withPointerParameter("arg", (void *)0x42)
                  .withMemoryBufferParameter("buffer", buffer, sizeof(buffer));

    mock_write((void *)0x42, buffer, sizeof(buffer));
}

TEST(DW1000MockTestGroup, Read)
{
    uint8_t buffer[] = "foobar";
    uint8_t read_buffer[7];

    mock("dw1000").expectOneCall("read")
                  .withPointerParameter("arg", (void *)0x42)
                  .withOutputParameterReturning("buffer", buffer, sizeof(buffer));


    mock_read((void *)0x42, read_buffer, sizeof(read_buffer));
    STRCMP_EQUAL((char *)buffer, (char *)read_buffer);
}

TEST(DW1000MockTestGroup, AcquireChipSelect)
{
    mock("dw1000").expectOneCall("cs_acquire")
                  .withPointerParameter("arg", (void *)0x42)
                  .withIntParameter("acquired", true);

    mock_cs_acquire((void *)0x42, true);
}

