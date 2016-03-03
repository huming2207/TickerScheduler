#include <Ticker.h>
#include <functional>
#include <stdint.h>

void tickerFlagHandle(volatile bool *flag);

typedef std::function<void(void)> tscallback_t;

struct TickerSchedulerItem {
    Ticker t;
    volatile bool flag = false;
    tscallback_t cb;
    volatile bool is_used = false;
    bool paused = false;
    uint32_t period = 0;
    uint8_t noOfRuns = 0;
    bool limited = false;
};

class TickerScheduler {
  private:
    uint8_t size;
    TickerSchedulerItem *items = NULL;
    void handleTicker(uint8_t i);

  public:
    TickerScheduler(uint8_t size);
    ~TickerScheduler();

    bool add(uint8_t i, uint32_t period, tscallback_t, uint8_t noOfRuns = 0,
             bool shouldFireNow = false);
    bool remove(uint8_t i);
    bool pause(uint8_t i);
    bool unpause(uint8_t i);
    void update();
};
