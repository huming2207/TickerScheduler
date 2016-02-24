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
};

class TickerScheduler {
  private:
    uint8_t size;
    TickerSchedulerItem *items = NULL;

    void handleTicker(tscallback_t, volatile bool *flag);

  public:
    TickerScheduler(uint8_t size);
    ~TickerScheduler();

    bool add(uint8_t i, uint32_t period, tscallback_t,
             bool shouldFireNow = false);

    bool remove(uint8_t i);

    void update();
};
