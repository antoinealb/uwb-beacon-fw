#include <CppUTest/TestHarness.h>
#include <CppUTestExt/MockSupport.h>

#include "mocks.h"
#include "dw1000/dw1000.h"

TEST_GROUP(DW1000DriverTestGroup)
{
    dw1000_t dev;
    void setup()
    {
        dw1000_init(&dev, (void *)0x42,
                mock_cs_acquire, mock_write, mock_read);
    }
};

TEST(DW1000DriverTestGroup, CanInit)
{
    POINTERS_EQUAL((void *)0x42, dev.arg);
    POINTERS_EQUAL(mock_cs_acquire, dev.acquire_cs);
    POINTERS_EQUAL(mock_write, dev.write);
    POINTERS_EQUAL(mock_read, dev.read);
}

TEST(DW1000DriverTestGroup, CanReadID)
{
    /* Read from register 0x00. */
    uint8_t command[] = {0x00};
    uint8_t answer[] = {0x30, 0x01, 0xca, 0xde};

    mock("dw1000").expectOneCall("cs_acquire")
                  .withPointerParameter("arg", dev.arg)
                  .withIntParameter("acquired", true);

    mock("dw1000").expectOneCall("write")
                  .withPointerParameter("arg", dev.arg)
                  .withMemoryBufferParameter("buffer", command, sizeof(command));

    mock("dw1000").expectOneCall("read")
                  .withPointerParameter("arg", dev.arg)
                  .withOutputParameterReturning("buffer", answer, sizeof(answer));


    mock("dw1000").expectOneCall("cs_acquire")
                  .withPointerParameter("arg", dev.arg)
                  .withIntParameter("acquired", false);

    auto res = dw1000_id_read(&dev);
    CHECK_EQUAL(0xdeca0130, res);
}
